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

static inline int8_t _mm_hmax_epi8_rpl(__m128i a) {
    a = _mm_max_epi8_rpl(a, _mm_srli_si128(a, 8));
    a = _mm_max_epi8_rpl(a, _mm_srli_si128(a, 4));
    a = _mm_max_epi8_rpl(a, _mm_srli_si128(a, 2));
    a = _mm_max_epi8_rpl(a, _mm_srli_si128(a, 1));
    return _mm_extract_epi8_rpl(a, 0);
}

static inline __m128i _mm_min_epi8_rpl(__m128i a, __m128i b) {
    __m128i mask = _mm_cmpgt_epi8(b, a);
    a = _mm_and_si128(a, mask);
    b = _mm_andnot_si128(mask, b);
    return _mm_or_si128(a, b);
}


#ifdef PARASAIL_TABLE
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
#endif

#ifdef PARASAIL_ROWCOL
static inline void arr_store_col(
        int *col,
        __m128i vH,
        int32_t t,
        int32_t seglen)
{
    col[ 0*seglen+t] = (int8_t)_mm_extract_epi8_rpl(vH,  0);
    col[ 1*seglen+t] = (int8_t)_mm_extract_epi8_rpl(vH,  1);
    col[ 2*seglen+t] = (int8_t)_mm_extract_epi8_rpl(vH,  2);
    col[ 3*seglen+t] = (int8_t)_mm_extract_epi8_rpl(vH,  3);
    col[ 4*seglen+t] = (int8_t)_mm_extract_epi8_rpl(vH,  4);
    col[ 5*seglen+t] = (int8_t)_mm_extract_epi8_rpl(vH,  5);
    col[ 6*seglen+t] = (int8_t)_mm_extract_epi8_rpl(vH,  6);
    col[ 7*seglen+t] = (int8_t)_mm_extract_epi8_rpl(vH,  7);
    col[ 8*seglen+t] = (int8_t)_mm_extract_epi8_rpl(vH,  8);
    col[ 9*seglen+t] = (int8_t)_mm_extract_epi8_rpl(vH,  9);
    col[10*seglen+t] = (int8_t)_mm_extract_epi8_rpl(vH, 10);
    col[11*seglen+t] = (int8_t)_mm_extract_epi8_rpl(vH, 11);
    col[12*seglen+t] = (int8_t)_mm_extract_epi8_rpl(vH, 12);
    col[13*seglen+t] = (int8_t)_mm_extract_epi8_rpl(vH, 13);
    col[14*seglen+t] = (int8_t)_mm_extract_epi8_rpl(vH, 14);
    col[15*seglen+t] = (int8_t)_mm_extract_epi8_rpl(vH, 15);
}
#endif

#ifdef PARASAIL_TABLE
#define FNAME parasail_sg_table_scan_sse2_128_8
#define PNAME parasail_sg_table_scan_profile_sse2_128_8
#else
#ifdef PARASAIL_ROWCOL
#define FNAME parasail_sg_rowcol_scan_sse2_128_8
#define PNAME parasail_sg_rowcol_scan_profile_sse2_128_8
#else
#define FNAME parasail_sg_scan_sse2_128_8
#define PNAME parasail_sg_scan_profile_sse2_128_8
#endif
#endif

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
    int32_t end_query = 0;
    int32_t end_ref = 0;
    const int s1Len = profile->s1Len;
    const parasail_matrix_t *matrix = profile->matrix;
    const int32_t segWidth = 16; /* number of values in vector unit */
    const int32_t segLen = (s1Len + segWidth - 1) / segWidth;
    const int32_t offset = (s1Len - 1) % segLen;
    const int32_t position = (segWidth - 1) - (s1Len - 1) / segLen;
    __m128i* const restrict pvP = (__m128i*)profile->profile8.score;
    __m128i* const restrict pvE = parasail_memalign___m128i(16, segLen);
    __m128i* const restrict pvHt= parasail_memalign___m128i(16, segLen);
    __m128i* const restrict pvH = parasail_memalign___m128i(16, segLen);
    __m128i* const restrict pvGapper = parasail_memalign___m128i(16, segLen);
    __m128i vGapO = _mm_set1_epi8(open);
    __m128i vGapE = _mm_set1_epi8(gap);
    __m128i vNegInf = _mm_set1_epi8(NEG_INF);
    __m128i vZero = _mm_setzero_si128();
    int8_t score = NEG_INF;
    __m128i vMaxH = vNegInf;
    __m128i vPosMask = _mm_cmpeq_epi8(_mm_set1_epi8(position),
            _mm_set_epi8(0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15));
    __m128i vNegInfFront = _mm_set_epi8(0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,NEG_INF);
    __m128i vSegLenXgap = _mm_adds_epi8(vNegInfFront,
            _mm_slli_si128(_mm_set1_epi8(-segLen*gap), 1));
    __m128i vNegLimit = _mm_set1_epi8(INT8_MIN);
    __m128i vPosLimit = _mm_set1_epi8(INT8_MAX);
    __m128i vSaturationCheckMin = vPosLimit;
    __m128i vSaturationCheckMax = vNegLimit;
#ifdef PARASAIL_TABLE
    parasail_result_t *result = parasail_result_new_table1(segLen*segWidth, s2Len);
#else
#ifdef PARASAIL_ROWCOL
    parasail_result_t *result = parasail_result_new_rowcol1(segLen*segWidth, s2Len);
#else
    parasail_result_t *result = parasail_result_new();
#endif
#endif

    /* initialize H and E */
    parasail_memset___m128i(pvH, vZero, segLen);
    parasail_memset___m128i(pvE, vNegInf, segLen);
    {
        __m128i vGapper = _mm_subs_epi8(vZero,vGapO);
        for (i=segLen-1; i>=0; --i) {
            _mm_store_si128(pvGapper+i, vGapper);
            vGapper = _mm_subs_epi8(vGapper, vGapE);
        }
    }

    /* outer loop over database sequence */
    for (j=0; j<s2Len; ++j) {
        __m128i vE;
        __m128i vHt;
        __m128i vF;
        __m128i vH;
        __m128i vHp;
        __m128i *pvW;
        __m128i vW;

        /* calculate E */
        /* calculate Ht */
        /* calculate F and H first pass */
        vHp = _mm_load_si128(pvH+(segLen-1));
        vHp = _mm_slli_si128(vHp, 1);
        pvW = pvP + matrix->mapper[(unsigned char)s2[j]]*segLen;
        vHt = vNegInf;
        vF = vNegInf;
        for (i=0; i<segLen; ++i) {
            vH = _mm_load_si128(pvH+i);
            vE = _mm_load_si128(pvE+i);
            vW = _mm_load_si128(pvW+i);
            vE = _mm_max_epi8_rpl(
                    _mm_subs_epi8(vE, vGapE),
                    _mm_subs_epi8(vH, vGapO));
            vHp = _mm_adds_epi8(vHp, vW);
            vF = _mm_max_epi8_rpl(vF, _mm_adds_epi8(vHt, pvGapper[i]));
            vHt = _mm_max_epi8_rpl(vE, vHp);
            _mm_store_si128(pvE+i, vE);
            _mm_store_si128(pvHt+i, vHt);
            vHp = vH;
        }

        /* pseudo prefix scan on F and H */
        vHt = _mm_slli_si128(vHt, 1);
        vF = _mm_max_epi8_rpl(vF, _mm_adds_epi8(vHt, pvGapper[0]));
        for (i=0; i<segWidth-2; ++i) {
            __m128i vFt = _mm_slli_si128(vF, 1);
            vFt = _mm_adds_epi8(vFt, vSegLenXgap);
            vF = _mm_max_epi8_rpl(vF, vFt);
        }

        /* calculate final H */
        vF = _mm_slli_si128(vF, 1);
        vF = _mm_adds_epi8(vF, vNegInfFront);
        vH = _mm_max_epi8_rpl(vHt, vF);
        for (i=0; i<segLen; ++i) {
            vHt = _mm_load_si128(pvHt+i);
            vF = _mm_max_epi8_rpl(
                    _mm_subs_epi8(vF, vGapE),
                    _mm_subs_epi8(vH, vGapO));
            vH = _mm_max_epi8_rpl(vHt, vF);
            _mm_store_si128(pvH+i, vH);
            /* check for saturation */
            {
                vSaturationCheckMax = _mm_max_epi8_rpl(vSaturationCheckMax, vH);
                vSaturationCheckMin = _mm_min_epi8_rpl(vSaturationCheckMin, vH);
            }
#ifdef PARASAIL_TABLE
            arr_store_si128(result->score_table, vH, i, segLen, j, s2Len);
#endif
        } 

        /* extract vector containing last value from column */
        {
            __m128i vCompare;
            vH = _mm_load_si128(pvH + offset);
            vCompare = _mm_and_si128(vPosMask, _mm_cmpgt_epi8(vH, vMaxH));
            vMaxH = _mm_max_epi8_rpl(vH, vMaxH);
            if (_mm_movemask_epi8(vCompare)) {
                end_ref = j;
                end_query = s1Len - 1;
            }
#ifdef PARASAIL_ROWCOL
            for (k=0; k<position; ++k) {
                vH = _mm_slli_si128(vH, 1);
            }
            result->score_row[j] = (int8_t) _mm_extract_epi8_rpl (vH, 15);
#endif
        }
    }

    /* max last value from all columns */
    {
        int8_t value;
        for (k=0; k<position; ++k) {
            vMaxH = _mm_slli_si128(vMaxH, 1);
        }
        value = (int8_t) _mm_extract_epi8_rpl(vMaxH, 15);
        if (value > score) {
            score = value;
        }
    }

    /* max of last column */
    {
        int8_t score_last;
        vMaxH = vNegInf;

        for (i=0; i<segLen; ++i) {
            __m128i vH = _mm_load_si128(pvH + i);
            vMaxH = _mm_max_epi8_rpl(vH, vMaxH);
#ifdef PARASAIL_ROWCOL
            arr_store_col(result->score_col, vH, i, segLen);
#endif
        }

        /* max in vec */
        score_last = _mm_hmax_epi8_rpl(vMaxH);
        if (score_last > score) {
            score = score_last;
            end_ref = s2Len - 1;
            end_query = s1Len;
            /* Trace the alignment ending position on read. */
            {
                int8_t *t = (int8_t*)pvH;
                int32_t column_len = segLen * segWidth;
                for (i = 0; i<column_len; ++i, ++t) {
                    if (*t == score) {
                        int32_t temp = i / segWidth + i % segWidth * segLen;
                        if (temp < end_query) {
                            end_query = temp;
                        }
                    }
                }
            }
        }
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
    result->flag = PARASAIL_FLAG_SG | PARASAIL_FLAG_SCAN
        | PARASAIL_FLAG_BITS_8 | PARASAIL_FLAG_LANES_16;
#ifdef PARASAIL_TABLE
    result->flag |= PARASAIL_FLAG_TABLE;
#endif
#ifdef PARASAIL_ROWCOL
    result->flag |= PARASAIL_FLAG_ROWCOL;
#endif

    parasail_free(pvGapper);
    parasail_free(pvH);
    parasail_free(pvHt);
    parasail_free(pvE);

    return result;
}


