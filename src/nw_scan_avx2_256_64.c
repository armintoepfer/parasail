/**
 * @file
 *
 * @author jeff.daily@pnnl.gov
 *
 * Copyright (c) 2014 Battelle Memorial Institute.
 *
 * All rights reserved. No warranty, explicit or implicit, provided.
 */
#include "config.h"

#include <stdint.h>
#include <stdlib.h>

#include <immintrin.h>

#include "parasail.h"
#include "parasail/memory.h"
#include "parasail/internal_avx.h"
#include "parasail/matrices/blosum_map.h"

#define NEG_INF (INT64_MIN/(int64_t)(2))

static inline __m256i _mm256_max_epi64_rpl(__m256i a, __m256i b) {
    __m256i_64_t A;
    __m256i_64_t B;
    A.m = a;
    B.m = b;
    A.v[0] = (A.v[0]>B.v[0]) ? A.v[0] : B.v[0];
    A.v[1] = (A.v[1]>B.v[1]) ? A.v[1] : B.v[1];
    A.v[2] = (A.v[2]>B.v[2]) ? A.v[2] : B.v[2];
    A.v[3] = (A.v[3]>B.v[3]) ? A.v[3] : B.v[3];
    return A.m;
}

#define _mm256_rlli_si256_rpl(a,imm) _mm256_alignr_epi8(a, _mm256_permute2x128_si256(a, a, _MM_SHUFFLE(0,0,0,1)), 16-imm)

#define _mm256_slli_si256_rpl(a,imm) _mm256_alignr_epi8(a, _mm256_permute2x128_si256(a, a, _MM_SHUFFLE(0,0,3,0)), 16-imm)


#ifdef PARASAIL_TABLE
static inline void arr_store_si256(
        int *array,
        __m256i vH,
        int32_t t,
        int32_t seglen,
        int32_t d,
        int32_t dlen)
{
    array[(0*seglen+t)*dlen + d] = (int64_t)_mm256_extract_epi64(vH, 0);
    array[(1*seglen+t)*dlen + d] = (int64_t)_mm256_extract_epi64(vH, 1);
    array[(2*seglen+t)*dlen + d] = (int64_t)_mm256_extract_epi64(vH, 2);
    array[(3*seglen+t)*dlen + d] = (int64_t)_mm256_extract_epi64(vH, 3);
}
#endif

#ifdef PARASAIL_TABLE
#define FNAME nw_table_scan_avx2_256_64
#else
#define FNAME nw_scan_avx2_256_64
#endif

parasail_result_t* FNAME(
        const char * const restrict s1, const int s1Len,
        const char * const restrict s2, const int s2Len,
        const int open, const int gap, const parasail_matrix_t *matrix)
{
    int32_t i = 0;
    int32_t j = 0;
    int32_t k = 0;
    int32_t segNum = 0;
    const int32_t n = 24; /* number of amino acids in table */
    const int32_t segWidth = 4; /* number of values in vector unit */
    const int32_t segLen = (s1Len + segWidth - 1) / segWidth;
    const int32_t offset = (s1Len - 1) % segLen;
    const int32_t position = (segWidth - 1) - (s1Len - 1) / segLen;
    __m256i* const restrict pvP = parasail_memalign___m256i(32, n * segLen);
    __m256i* const restrict pvE = parasail_memalign___m256i(32, segLen);
    int64_t* const restrict boundary = parasail_memalign_int64_t(32, s2Len+1);
    __m256i* const restrict pvHt= parasail_memalign___m256i(32, segLen);
    __m256i* const restrict pvH = parasail_memalign___m256i(32, segLen);
    __m256i vGapO = _mm256_set1_epi64x(open);
    __m256i vGapE = _mm256_set1_epi64x(gap);
    __m256i vNegInf = _mm256_set1_epi64x(NEG_INF);
    int64_t score = NEG_INF;
    const int64_t segLenXgap = -segLen*gap;
    __m256i insert_mask = _mm256_cmpeq_epi64(_mm256_setzero_si256(),
            _mm256_set_epi64x(1,0,0,0));
    __m256i vSegLenXgap1 = _mm256_set1_epi64x((segLen-1)*gap);
    __m256i vSegLenXgap = _mm256_blendv_epi8(vNegInf,
            _mm256_set1_epi64x(segLenXgap),
            insert_mask);
    
#ifdef PARASAIL_TABLE
    parasail_result_t *result = parasail_result_new_table1(segLen*segWidth, s2Len);
#else
    parasail_result_t *result = parasail_result_new();
#endif

    /* Generate query profile.
     * Rearrange query sequence & calculate the weight of match/mismatch.
     * Don't alias. */
    {
        int32_t index = 0;
        for (k=0; k<n; ++k) {
            for (i=0; i<segLen; ++i) {
                __m256i_64_t t;
                j = i;
                for (segNum=0; segNum<segWidth; ++segNum) {
                    t.v[segNum] = j >= s1Len ? 0 : matrix->matrix[matrix->size*k+matrix->mapper[(unsigned char)s1[j]]];
                    j += segLen;
                }
                _mm256_store_si256(&pvP[index], t.m);
                ++index;
            }
        }
    }

    /* initialize H and E */
    {
        int32_t index = 0;
        for (i=0; i<segLen; ++i) {
            __m256i_64_t h;
            __m256i_64_t e;
            for (segNum=0; segNum<segWidth; ++segNum) {
                int64_t tmp = -open-gap*(segNum*segLen+i);
                h.v[segNum] = tmp < INT64_MIN ? INT64_MIN : tmp;
                tmp = tmp - open;
                e.v[segNum] = tmp < INT64_MIN ? INT64_MIN : tmp;
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
            boundary[i] = tmp < INT64_MIN ? INT64_MIN : tmp;
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
        vHp = _mm256_slli_si256_rpl(vHp, 8);
        vHp = _mm256_insert_epi64(vHp, boundary[j], 0);
        pvW = pvP + matrix->mapper[(unsigned char)s2[j]]*segLen;
        vHt = vNegInf;
        vFt = vNegInf;
        for (i=0; i<segLen; ++i) {
            vH = _mm256_load_si256(pvH+i);
            vE = _mm256_load_si256(pvE+i);
            vW = _mm256_load_si256(pvW+i);
            vE = _mm256_max_epi64_rpl(
                    _mm256_sub_epi64(vE, vGapE),
                    _mm256_sub_epi64(vH, vGapO));
            vFt = _mm256_sub_epi64(vFt, vGapE);
            vFt = _mm256_max_epi64_rpl(vFt, vHt);
            vHt = _mm256_max_epi64_rpl(
                    _mm256_add_epi64(vHp, vW),
                    vE);
            _mm256_store_si256(pvE+i, vE);
            _mm256_store_si256(pvHt+i, vHt);
            vHp = vH;
        }

        /* adjust Ft before local prefix scan */
        vHt = _mm256_slli_si256_rpl(vHt, 8);
        vHt = _mm256_insert_epi64(vHt, boundary[j+1], 0);
        vFt = _mm256_max_epi64_rpl(vFt,
                _mm256_sub_epi64(vHt, vSegLenXgap1));
        /* local prefix scan */
        vFt = _mm256_blendv_epi8(vNegInf, vFt, insert_mask);
            for (i=0; i<segWidth-1; ++i) {
                __m256i vFtt = _mm256_rlli_si256_rpl(vFt, 8);
                vFtt = _mm256_add_epi64(vFtt, vSegLenXgap);
                vFt = _mm256_max_epi64_rpl(vFt, vFtt);
            }
        vFt = _mm256_rlli_si256_rpl(vFt, 8);

        /* second Ft pass */
        /* calculate vH */
        for (i=0; i<segLen; ++i) {
            vFt = _mm256_sub_epi64(vFt, vGapE);
            vFt = _mm256_max_epi64_rpl(vFt, vHt);
            vHt = _mm256_load_si256(pvHt+i);
            vH = _mm256_max_epi64_rpl(vHt, _mm256_sub_epi64(vFt, vGapO));
            _mm256_store_si256(pvH+i, vH);
            
#ifdef PARASAIL_TABLE
            arr_store_si256(result->score_table, vH, i, segLen, j, s2Len);
#endif
        }
    }

    /* extract last value from the last column */
    {
        __m256i vH = _mm256_load_si256(pvH + offset);
        for (k=0; k<position; ++k) {
            vH = _mm256_slli_si256_rpl(vH, 8);
        }
        score = (int64_t) _mm256_extract_epi64 (vH, 3);
    }

    

    result->score = score;

    parasail_free(boundary);
    parasail_free(pvH);
    parasail_free(pvHt);
    parasail_free(pvE);
    parasail_free(pvP);

    return result;
}


