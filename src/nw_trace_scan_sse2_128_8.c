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

#include <emmintrin.h>

#include "parasail.h"
#include "parasail/memory.h"
#include "parasail/internal_sse.h"

#define NEG_INF INT8_MIN

static inline __m128i _mm_blendv_epi8_rpl(__m128i a, __m128i b, __m128i mask) {
    a = _mm_andnot_si128(mask, a);
    a = _mm_or_si128(a, _mm_and_si128(mask, b));
    return a;
}

static inline __m128i _mm_insert_epi8_rpl(__m128i a, int8_t i, const int imm) {
    __m128i_8_t A;
    A.m = a;
    A.v[imm] = i;
    return A.m;
}

static inline __m128i _mm_max_epi8_rpl(__m128i a, __m128i b) {
    __m128i mask = _mm_cmpgt_epi8(a, b);
    a = _mm_and_si128(a, mask);
    b = _mm_andnot_si128(mask, b);
    return _mm_or_si128(a, b);
}

static inline int8_t _mm_extract_epi8_rpl(__m128i a, const int imm) {
    __m128i_8_t A;
    A.m = a;
    return A.v[imm];
}

#define _mm_rlli_si128_rpl(a,imm) _mm_or_si128(_mm_slli_si128(a,imm),_mm_srli_si128(a,16-imm))

static inline __m128i _mm_min_epi8_rpl(__m128i a, __m128i b) {
    __m128i mask = _mm_cmpgt_epi8(b, a);
    a = _mm_and_si128(a, mask);
    b = _mm_andnot_si128(mask, b);
    return _mm_or_si128(a, b);
}


static inline void arr_store_si128(
        int *array,
        __m128i vH,
        int32_t t,
        int32_t seglen,
        int32_t d,
        int32_t dlen)
{
    array[( 0*seglen+t)*dlen + d] = (int8_t)_mm_extract_epi8_rpl(vH,  0);
    array[( 1*seglen+t)*dlen + d] = (int8_t)_mm_extract_epi8_rpl(vH,  1);
    array[( 2*seglen+t)*dlen + d] = (int8_t)_mm_extract_epi8_rpl(vH,  2);
    array[( 3*seglen+t)*dlen + d] = (int8_t)_mm_extract_epi8_rpl(vH,  3);
    array[( 4*seglen+t)*dlen + d] = (int8_t)_mm_extract_epi8_rpl(vH,  4);
    array[( 5*seglen+t)*dlen + d] = (int8_t)_mm_extract_epi8_rpl(vH,  5);
    array[( 6*seglen+t)*dlen + d] = (int8_t)_mm_extract_epi8_rpl(vH,  6);
    array[( 7*seglen+t)*dlen + d] = (int8_t)_mm_extract_epi8_rpl(vH,  7);
    array[( 8*seglen+t)*dlen + d] = (int8_t)_mm_extract_epi8_rpl(vH,  8);
    array[( 9*seglen+t)*dlen + d] = (int8_t)_mm_extract_epi8_rpl(vH,  9);
    array[(10*seglen+t)*dlen + d] = (int8_t)_mm_extract_epi8_rpl(vH, 10);
    array[(11*seglen+t)*dlen + d] = (int8_t)_mm_extract_epi8_rpl(vH, 11);
    array[(12*seglen+t)*dlen + d] = (int8_t)_mm_extract_epi8_rpl(vH, 12);
    array[(13*seglen+t)*dlen + d] = (int8_t)_mm_extract_epi8_rpl(vH, 13);
    array[(14*seglen+t)*dlen + d] = (int8_t)_mm_extract_epi8_rpl(vH, 14);
    array[(15*seglen+t)*dlen + d] = (int8_t)_mm_extract_epi8_rpl(vH, 15);
}

#define FNAME parasail_nw_trace_scan_sse2_128_8
#define PNAME parasail_nw_trace_scan_profile_sse2_128_8

parasail_result_t* FNAME(
        const char * const restrict s1, const int s1Len,
        const char * const restrict s2, const int s2Len,
        const int open, const int gap, const parasail_matrix_t *matrix)
{
    parasail_profile_t *profile = parasail_profile_create_sse_128_8(s1, s1Len, matrix);
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
    const int32_t segWidth = 16; /* number of values in vector unit */
    const int32_t segLen = (s1Len + segWidth - 1) / segWidth;
    const int32_t offset = (s1Len - 1) % segLen;
    const int32_t position = (segWidth - 1) - (s1Len - 1) / segLen;
    __m128i* const restrict pvP = (__m128i*)profile->profile8.score;
    __m128i* const restrict pvE = parasail_memalign___m128i(16, segLen);
    int8_t* const restrict boundary = parasail_memalign_int8_t(16, s2Len+1);
    __m128i* const restrict pvHt= parasail_memalign___m128i(16, segLen);
    __m128i* const restrict pvH = parasail_memalign___m128i(16, segLen);
    __m128i vGapO = _mm_set1_epi8(open);
    __m128i vGapE = _mm_set1_epi8(gap);
    __m128i vNegInf = _mm_set1_epi8(NEG_INF);
    int8_t score = NEG_INF;
    const int8_t segLenXgap = -segLen*gap;
    __m128i insert_mask = _mm_cmpeq_epi8(_mm_setzero_si128(),
            _mm_set_epi8(1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0));
    __m128i vSegLenXgap1 = _mm_set1_epi8((segLen-1)*gap);
    __m128i vSegLenXgap = _mm_blendv_epi8_rpl(vNegInf,
            _mm_set1_epi8(segLenXgap),
            insert_mask);
    __m128i vNegLimit = _mm_set1_epi8(INT8_MIN);
    __m128i vPosLimit = _mm_set1_epi8(INT8_MAX);
    __m128i vSaturationCheckMin = vPosLimit;
    __m128i vSaturationCheckMax = vNegLimit;
    parasail_result_t *result = parasail_result_new_trace(segLen*segWidth, s2Len, 1);

    /* initialize H and E */
    {
        int32_t index = 0;
        for (i=0; i<segLen; ++i) {
            __m128i_8_t h;
            __m128i_8_t e;
            for (segNum=0; segNum<segWidth; ++segNum) {
                int64_t tmp = -open-gap*(segNum*segLen+i);
                h.v[segNum] = tmp < INT8_MIN ? INT8_MIN : tmp;
                tmp = tmp - open;
                e.v[segNum] = tmp < INT8_MIN ? INT8_MIN : tmp;
            }
            _mm_store_si128(&pvH[index], h.m);
            _mm_store_si128(&pvE[index], e.m);
            ++index;
        }
    }

    /* initialize uppder boundary */
    {
        boundary[0] = 0;
        for (i=1; i<=s2Len; ++i) {
            int64_t tmp = -open-gap*(i-1);
            boundary[i] = tmp < INT8_MIN ? INT8_MIN : tmp;
        }
    }

    /* outer loop over database sequence */
    for (j=0; j<s2Len; ++j) {
        __m128i vE;
        __m128i vHt;
        __m128i vFt;
        __m128i vH;
        __m128i vHp;
        __m128i *pvW;
        __m128i vW;

        /* calculate E */
        /* calculate Ht */
        /* calculate Ft first pass */
        vHp = _mm_load_si128(pvH+(segLen-1));
        vHp = _mm_slli_si128(vHp, 1);
        vHp = _mm_insert_epi8_rpl(vHp, boundary[j], 0);
        pvW = pvP + matrix->mapper[(unsigned char)s2[j]]*segLen;
        vHt = vNegInf;
        vFt = vNegInf;
        for (i=0; i<segLen; ++i) {
            vH = _mm_load_si128(pvH+i);
            vE = _mm_load_si128(pvE+i);
            vW = _mm_load_si128(pvW+i);
            vE = _mm_max_epi8_rpl(
                    _mm_subs_epi8(vE, vGapE),
                    _mm_subs_epi8(vH, vGapO));
            vFt = _mm_subs_epi8(vFt, vGapE);
            vFt = _mm_max_epi8_rpl(vFt, vHt);
            vHt = _mm_max_epi8_rpl(
                    _mm_adds_epi8(vHp, vW),
                    vE);
            _mm_store_si128(pvE+i, vE);
            _mm_store_si128(pvHt+i, vHt);
            vHp = vH;
        }

        /* adjust Ft before local prefix scan */
        vHt = _mm_slli_si128(vHt, 1);
        vHt = _mm_insert_epi8_rpl(vHt, boundary[j+1], 0);
        vFt = _mm_max_epi8_rpl(vFt,
                _mm_subs_epi8(vHt, vSegLenXgap1));
        /* local prefix scan */
        vFt = _mm_blendv_epi8_rpl(vNegInf, vFt, insert_mask);
            for (i=0; i<segWidth-1; ++i) {
                __m128i vFtt = _mm_rlli_si128_rpl(vFt, 1);
                vFtt = _mm_adds_epi8(vFtt, vSegLenXgap);
                vFt = _mm_max_epi8_rpl(vFt, vFtt);
            }
        vFt = _mm_rlli_si128_rpl(vFt, 1);

        /* second Ft pass */
        /* calculate vH */
        for (i=0; i<segLen; ++i) {
            vFt = _mm_subs_epi8(vFt, vGapE);
            vFt = _mm_max_epi8_rpl(vFt, vHt);
            vHt = _mm_load_si128(pvHt+i);
            vH = _mm_max_epi8_rpl(vHt, _mm_subs_epi8(vFt, vGapO));
            _mm_store_si128(pvH+i, vH);
            /* check for saturation */
            {
                vSaturationCheckMax = _mm_max_epi8_rpl(vSaturationCheckMax, vH);
                vSaturationCheckMin = _mm_min_epi8_rpl(vSaturationCheckMin, vH);
            }
        }
    }

    /* extract last value from the last column */
    {
        __m128i vH = _mm_load_si128(pvH + offset);
        for (k=0; k<position; ++k) {
            vH = _mm_slli_si128(vH, 1);
        }
        score = (int8_t) _mm_extract_epi8_rpl (vH, 15);
    }

    if (_mm_movemask_epi8(_mm_or_si128(
            _mm_cmpeq_epi8(vSaturationCheckMin, vNegLimit),
            _mm_cmpeq_epi8(vSaturationCheckMax, vPosLimit)))) {
        result->saturated = 1;
        score = INT8_MAX;
        end_query = 0;
        end_ref = 0;
    }

    result->score = score;
    result->end_query = end_query;
    result->end_ref = end_ref;
    result->flag = PARASAIL_FLAG_NW | PARASAIL_FLAG_SCAN
        | PARASAIL_FLAG_TRACE
        | PARASAIL_FLAG_BITS_8 | PARASAIL_FLAG_LANES_16;

    parasail_free(boundary);
    parasail_free(pvH);
    parasail_free(pvHt);
    parasail_free(pvE);

    return result;
}


