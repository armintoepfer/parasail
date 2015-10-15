/**
 * @file
 *
 * @author jeff.daily@pnnl.gov
 *
 * Copyright (c) 2015 Battelle Memorial Institute.
 */
#include "config.h"

#include <stdint.h>
#include <stdlib.h>

#include <immintrin.h>

#include "parasail.h"
#include "parasail/memory.h"
#include "parasail/internal_avx.h"

#define NEG_INF (INT32_MIN/(int32_t)(2))

#if HAVE_AVX2_MM256_INSERT_EPI32
#define _mm256_insert_epi32_rpl _mm256_insert_epi32
#else
static inline __m256i _mm256_insert_epi32_rpl(__m256i a, int32_t i, int imm) {
    __m256i_32_t A;
    A.m = a;
    A.v[imm] = i;
    return A.m;
}
#endif

#if HAVE_AVX2_MM256_EXTRACT_EPI32
#define _mm256_extract_epi32_rpl _mm256_extract_epi32
#else
static inline int32_t _mm256_extract_epi32_rpl(__m256i a, int imm) {
    __m256i_32_t A;
    A.m = a;
    return A.v[imm];
}
#endif

#define _mm256_rlli_si256_rpl(a,imm) _mm256_alignr_epi8(a, _mm256_permute2x128_si256(a, a, _MM_SHUFFLE(0,0,0,1)), 16-imm)

#define _mm256_slli_si256_rpl(a,imm) _mm256_alignr_epi8(a, _mm256_permute2x128_si256(a, a, _MM_SHUFFLE(0,0,3,0)), 16-imm)


static inline void arr_store_si256(
        int *array,
        __m256i vH,
        int32_t t,
        int32_t seglen,
        int32_t d,
        int32_t dlen)
{
    array[(0*seglen+t)*dlen + d] = (int32_t)_mm256_extract_epi32_rpl(vH, 0);
    array[(1*seglen+t)*dlen + d] = (int32_t)_mm256_extract_epi32_rpl(vH, 1);
    array[(2*seglen+t)*dlen + d] = (int32_t)_mm256_extract_epi32_rpl(vH, 2);
    array[(3*seglen+t)*dlen + d] = (int32_t)_mm256_extract_epi32_rpl(vH, 3);
    array[(4*seglen+t)*dlen + d] = (int32_t)_mm256_extract_epi32_rpl(vH, 4);
    array[(5*seglen+t)*dlen + d] = (int32_t)_mm256_extract_epi32_rpl(vH, 5);
    array[(6*seglen+t)*dlen + d] = (int32_t)_mm256_extract_epi32_rpl(vH, 6);
    array[(7*seglen+t)*dlen + d] = (int32_t)_mm256_extract_epi32_rpl(vH, 7);
}

#define FNAME parasail_nw_trace_scan_avx2_256_32
#define PNAME parasail_nw_trace_scan_profile_avx2_256_32

parasail_result_t* FNAME(
        const char * const restrict s1, const int s1Len,
        const char * const restrict s2, const int s2Len,
        const int open, const int gap, const parasail_matrix_t *matrix)
{
    parasail_profile_t *profile = parasail_profile_create_avx_256_32(s1, s1Len, matrix);
    parasail_result_t *result = PNAME(profile, s2, s2Len, open, gap);
    parasail_profile_free(profile);
    return result;
}

parasail_result_t* PNAME(
        const parasail_profile_t * const restrict profile,
        const char * const restrict s2, const int s2Len,
        const int open, const int gap)
{
    int32_t i = 0;
    int32_t j = 0;
    int32_t k = 0;
    int32_t segNum = 0;
    const int s1Len = profile->s1Len;
    int32_t end_query = s1Len-1;
    int32_t end_ref = s2Len-1;
    const parasail_matrix_t *matrix = profile->matrix;
    const int32_t segWidth = 8; /* number of values in vector unit */
    const int32_t segLen = (s1Len + segWidth - 1) / segWidth;
    const int32_t offset = (s1Len - 1) % segLen;
    const int32_t position = (segWidth - 1) - (s1Len - 1) / segLen;
    __m256i* const restrict pvP = (__m256i*)profile->profile32.score;
    __m256i* const restrict pvE = parasail_memalign___m256i(32, segLen);
    int32_t* const restrict boundary = parasail_memalign_int32_t(32, s2Len+1);
    __m256i* const restrict pvHt= parasail_memalign___m256i(32, segLen);
    __m256i* const restrict pvH = parasail_memalign___m256i(32, segLen);
    __m256i vGapO = _mm256_set1_epi32(open);
    __m256i vGapE = _mm256_set1_epi32(gap);
    __m256i vNegInf = _mm256_set1_epi32(NEG_INF);
    int32_t score = NEG_INF;
    const int32_t segLenXgap = -segLen*gap;
    __m256i insert_mask = _mm256_cmpeq_epi32(_mm256_setzero_si256(),
            _mm256_set_epi32(1,0,0,0,0,0,0,0));
    __m256i vSegLenXgap1 = _mm256_set1_epi32((segLen-1)*gap);
    __m256i vSegLenXgap = _mm256_blendv_epi8(vNegInf,
            _mm256_set1_epi32(segLenXgap),
            insert_mask);
    
    parasail_result_t *result = parasail_result_new_trace(segLen*segWidth, s2Len, 4);

    /* initialize H and E */
    {
        int32_t index = 0;
        for (i=0; i<segLen; ++i) {
            __m256i_32_t h;
            __m256i_32_t e;
            for (segNum=0; segNum<segWidth; ++segNum) {
                int64_t tmp = -open-gap*(segNum*segLen+i);
                h.v[segNum] = tmp < INT32_MIN ? INT32_MIN : tmp;
                tmp = tmp - open;
                e.v[segNum] = tmp < INT32_MIN ? INT32_MIN : tmp;
            }
            _mm256_store_si256(&pvH[index], h.m);
            _mm256_store_si256(&pvE[index], e.m);
            ++index;
        }
    }

    /* initialize uppder boundary */
    {
        boundary[0] = 0;
        for (i=1; i<=s2Len; ++i) {
            int64_t tmp = -open-gap*(i-1);
            boundary[i] = tmp < INT32_MIN ? INT32_MIN : tmp;
        }
    }

    /* outer loop over database sequence */
    for (j=0; j<s2Len; ++j) {
        __m256i vE;
        __m256i vHt;
        __m256i vFt;
        __m256i vH;
        __m256i vHp;
        __m256i *pvW;
        __m256i vW;

        /* calculate E */
        /* calculate Ht */
        /* calculate Ft first pass */
        vHp = _mm256_load_si256(pvH+(segLen-1));
        vHp = _mm256_slli_si256_rpl(vHp, 4);
        vHp = _mm256_insert_epi32_rpl(vHp, boundary[j], 0);
        pvW = pvP + matrix->mapper[(unsigned char)s2[j]]*segLen;
        vHt = vNegInf;
        vFt = vNegInf;
        for (i=0; i<segLen; ++i) {
            vH = _mm256_load_si256(pvH+i);
            vE = _mm256_load_si256(pvE+i);
            vW = _mm256_load_si256(pvW+i);
            vE = _mm256_max_epi32(
                    _mm256_sub_epi32(vE, vGapE),
                    _mm256_sub_epi32(vH, vGapO));
            vFt = _mm256_sub_epi32(vFt, vGapE);
            vFt = _mm256_max_epi32(vFt, vHt);
            vHt = _mm256_max_epi32(
                    _mm256_add_epi32(vHp, vW),
                    vE);
            _mm256_store_si256(pvE+i, vE);
            _mm256_store_si256(pvHt+i, vHt);
            vHp = vH;
        }

        /* adjust Ft before local prefix scan */
        vHt = _mm256_slli_si256_rpl(vHt, 4);
        vHt = _mm256_insert_epi32_rpl(vHt, boundary[j+1], 0);
        vFt = _mm256_max_epi32(vFt,
                _mm256_sub_epi32(vHt, vSegLenXgap1));
        /* local prefix scan */
        vFt = _mm256_blendv_epi8(vNegInf, vFt, insert_mask);
            for (i=0; i<segWidth-1; ++i) {
                __m256i vFtt = _mm256_rlli_si256_rpl(vFt, 4);
                vFtt = _mm256_add_epi32(vFtt, vSegLenXgap);
                vFt = _mm256_max_epi32(vFt, vFtt);
            }
        vFt = _mm256_rlli_si256_rpl(vFt, 4);

        /* second Ft pass */
        /* calculate vH */
        for (i=0; i<segLen; ++i) {
            vFt = _mm256_sub_epi32(vFt, vGapE);
            vFt = _mm256_max_epi32(vFt, vHt);
            vHt = _mm256_load_si256(pvHt+i);
            vH = _mm256_max_epi32(vHt, _mm256_sub_epi32(vFt, vGapO));
            _mm256_store_si256(pvH+i, vH);
            
        }
    }

    /* extract last value from the last column */
    {
        __m256i vH = _mm256_load_si256(pvH + offset);
        for (k=0; k<position; ++k) {
            vH = _mm256_slli_si256_rpl(vH, 4);
        }
        score = (int32_t) _mm256_extract_epi32_rpl (vH, 7);
    }

    

    result->score = score;
    result->end_query = end_query;
    result->end_ref = end_ref;
    result->flag = PARASAIL_FLAG_NW | PARASAIL_FLAG_SCAN
        | PARASAIL_FLAG_TRACE
        | PARASAIL_FLAG_BITS_32 | PARASAIL_FLAG_LANES_8;

    parasail_free(boundary);
    parasail_free(pvH);
    parasail_free(pvHt);
    parasail_free(pvE);

    return result;
}


