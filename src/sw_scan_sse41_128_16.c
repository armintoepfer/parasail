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
#include <string.h>

#include <emmintrin.h>
#include <smmintrin.h>

#include "parasail.h"
#include "parasail/memory.h"
#include "parasail/internal_sse.h"

#define NEG_INF (INT16_MIN/(int16_t)(2))

static inline int16_t _mm_hmax_epi16_rpl(__m128i a) {
    a = _mm_max_epi16(a, _mm_srli_si128(a, 8));
    a = _mm_max_epi16(a, _mm_srli_si128(a, 4));
    a = _mm_max_epi16(a, _mm_srli_si128(a, 2));
    return _mm_extract_epi16(a, 0);
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
    array[(0*seglen+t)*dlen + d] = (int16_t)_mm_extract_epi16(vH, 0);
    array[(1*seglen+t)*dlen + d] = (int16_t)_mm_extract_epi16(vH, 1);
    array[(2*seglen+t)*dlen + d] = (int16_t)_mm_extract_epi16(vH, 2);
    array[(3*seglen+t)*dlen + d] = (int16_t)_mm_extract_epi16(vH, 3);
    array[(4*seglen+t)*dlen + d] = (int16_t)_mm_extract_epi16(vH, 4);
    array[(5*seglen+t)*dlen + d] = (int16_t)_mm_extract_epi16(vH, 5);
    array[(6*seglen+t)*dlen + d] = (int16_t)_mm_extract_epi16(vH, 6);
    array[(7*seglen+t)*dlen + d] = (int16_t)_mm_extract_epi16(vH, 7);
}
#endif

#ifdef PARASAIL_ROWCOL
static inline void arr_store_col(
        int *col,
        __m128i vH,
        int32_t t,
        int32_t seglen)
{
    col[0*seglen+t] = (int16_t)_mm_extract_epi16(vH, 0);
    col[1*seglen+t] = (int16_t)_mm_extract_epi16(vH, 1);
    col[2*seglen+t] = (int16_t)_mm_extract_epi16(vH, 2);
    col[3*seglen+t] = (int16_t)_mm_extract_epi16(vH, 3);
    col[4*seglen+t] = (int16_t)_mm_extract_epi16(vH, 4);
    col[5*seglen+t] = (int16_t)_mm_extract_epi16(vH, 5);
    col[6*seglen+t] = (int16_t)_mm_extract_epi16(vH, 6);
    col[7*seglen+t] = (int16_t)_mm_extract_epi16(vH, 7);
}
#endif

#ifdef PARASAIL_TABLE
#define FNAME parasail_sw_table_scan_sse41_128_16
#define PNAME parasail_sw_table_scan_profile_sse41_128_16
#else
#ifdef PARASAIL_ROWCOL
#define FNAME parasail_sw_rowcol_scan_sse41_128_16
#define PNAME parasail_sw_rowcol_scan_profile_sse41_128_16
#else
#define FNAME parasail_sw_scan_sse41_128_16
#define PNAME parasail_sw_scan_profile_sse41_128_16
#endif
#endif

parasail_result_t* FNAME(
        const char * const restrict s1, const int s1Len,
        const char * const restrict s2, const int s2Len,
        const int open, const int gap, const parasail_matrix_t *matrix)
{
    parasail_profile_t *profile = parasail_profile_create_sse_128_16(s1, s1Len, matrix);
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
    int32_t end_query = 0;
    int32_t end_ref = 0;
    const int s1Len = profile->s1Len;
    const parasail_matrix_t *matrix = profile->matrix;
    const int32_t segWidth = 8; /* number of values in vector unit */
    const int32_t segLen = (s1Len + segWidth - 1) / segWidth;
    __m128i* const restrict pvP = (__m128i*)profile->profile16.score;
    __m128i* const restrict pvE = parasail_memalign___m128i(16, segLen);
    __m128i* const restrict pvHt= parasail_memalign___m128i(16, segLen);
    __m128i* const restrict pvH = parasail_memalign___m128i(16, segLen);
    __m128i* const restrict pvHMax = parasail_memalign___m128i(16, segLen);
    __m128i* const restrict pvGapper = parasail_memalign___m128i(16, segLen);
    __m128i vGapO = _mm_set1_epi16(open);
    __m128i vGapE = _mm_set1_epi16(gap);
    __m128i vNegInf = _mm_set1_epi16(NEG_INF);
    __m128i vZero = _mm_setzero_si128();
    int16_t score = NEG_INF;
    __m128i vMaxH = vNegInf;
    __m128i vMaxHUnit = vNegInf;
    __m128i vNegInfFront = _mm_set_epi16(0,0,0,0,0,0,0,NEG_INF);
    __m128i vSegLenXgap = _mm_add_epi16(vNegInfFront,
            _mm_slli_si128(_mm_set1_epi16(-segLen*gap), 2));
    
#ifdef PARASAIL_TABLE
    parasail_result_t *result = parasail_result_new_table1(segLen*segWidth, s2Len);
#else
#ifdef PARASAIL_ROWCOL
    parasail_result_t *result = parasail_result_new_rowcol1(segLen*segWidth, s2Len);
    const int32_t offset = (s1Len - 1) % segLen;
    const int32_t position = (segWidth - 1) - (s1Len - 1) / segLen;
#else
    parasail_result_t *result = parasail_result_new();
#endif
#endif

    parasail_memset___m128i(pvH, vZero, segLen);
    parasail_memset___m128i(pvE, vNegInf, segLen);
    {
        __m128i vGapper = _mm_sub_epi16(vZero,vGapO);
        for (i=segLen-1; i>=0; --i) {
            _mm_store_si128(pvGapper+i, vGapper);
            vGapper = _mm_sub_epi16(vGapper, vGapE);
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
        vHp = _mm_slli_si128(vHp, 2);
        pvW = pvP + matrix->mapper[(unsigned char)s2[j]]*segLen;
        vHt = vZero;
        vF = vNegInf;
        for (i=0; i<segLen; ++i) {
            vH = _mm_load_si128(pvH+i);
            vE = _mm_load_si128(pvE+i);
            vW = _mm_load_si128(pvW+i);
            vE = _mm_max_epi16(
                    _mm_sub_epi16(vE, vGapE),
                    _mm_sub_epi16(vH, vGapO));
            vHp = _mm_add_epi16(vHp, vW);
            vF = _mm_max_epi16(vF, _mm_add_epi16(vHt, pvGapper[i]));
            vHt = _mm_max_epi16(vE, vHp);
            _mm_store_si128(pvE+i, vE);
            _mm_store_si128(pvHt+i, vHt);
            vHp = vH;
        }

        /* pseudo prefix scan on F and H */
        vHt = _mm_slli_si128(vHt, 2);
        vF = _mm_max_epi16(vF, _mm_add_epi16(vHt, pvGapper[0]));
        for (i=0; i<segWidth-2; ++i) {
            __m128i vFt = _mm_slli_si128(vF, 2);
            vFt = _mm_add_epi16(vFt, vSegLenXgap);
            vF = _mm_max_epi16(vF, vFt);
        }

        /* calculate final H */
        vF = _mm_slli_si128(vF, 2);
        vF = _mm_add_epi16(vF, vNegInfFront);
        vH = _mm_max_epi16(vHt, vF);
        for (i=0; i<segLen; ++i) {
            vHt = _mm_load_si128(pvHt+i);
            vF = _mm_max_epi16(
                    _mm_sub_epi16(vF, vGapE),
                    _mm_sub_epi16(vH, vGapO));
            vH = _mm_max_epi16(vHt, vF);
            vH = _mm_max_epi16(vH, vZero);
            _mm_store_si128(pvH+i, vH);
            
#ifdef PARASAIL_TABLE
            arr_store_si128(result->score_table, vH, i, segLen, j, s2Len);
#endif
            vMaxH = _mm_max_epi16(vH, vMaxH);
        } 

        {
            __m128i vCompare = _mm_cmpgt_epi16(vMaxH, vMaxHUnit);
            if (_mm_movemask_epi8(vCompare)) {
                score = _mm_hmax_epi16_rpl(vMaxH);
                vMaxHUnit = _mm_set1_epi16(score);
                end_ref = j;
                (void)memcpy(pvHMax, pvH, sizeof(__m128i)*segLen);
            }
        }

#ifdef PARASAIL_ROWCOL
        /* extract last value from the column */
        {
            int32_t k = 0;
            __m128i vH = _mm_load_si128(pvH + offset);
            for (k=0; k<position; ++k) {
                vH = _mm_slli_si128(vH, 2);
            }
            result->score_row[j] = (int16_t) _mm_extract_epi16 (vH, 7);
        }
#endif
    }

    /* Trace the alignment ending position on read. */
    {
        int16_t *t = (int16_t*)pvHMax;
        int32_t column_len = segLen * segWidth;
        end_query = s1Len;
        for (i = 0; i<column_len; ++i, ++t) {
            if (*t == score) {
                int32_t temp = i / segWidth + i % segWidth * segLen;
                if (temp < end_query) {
                    end_query = temp;
                }
            }
        }
    }

#ifdef PARASAIL_ROWCOL
    for (i=0; i<segLen; ++i) {
        __m128i vH = _mm_load_si128(pvH+i);
        arr_store_col(result->score_col, vH, i, segLen);
    }
#endif

    

    result->score = score;
    result->end_query = end_query;
    result->end_ref = end_ref;
    result->flag = PARASAIL_FLAG_SW | PARASAIL_FLAG_SCAN
        | PARASAIL_FLAG_BITS_16 | PARASAIL_FLAG_LANES_8;
#ifdef PARASAIL_TABLE
    result->flag |= PARASAIL_FLAG_TABLE;
#endif
#ifdef PARASAIL_ROWCOL
    result->flag |= PARASAIL_FLAG_ROWCOL;
#endif

    parasail_free(pvGapper);
    parasail_free(pvHMax);
    parasail_free(pvH);
    parasail_free(pvHt);
    parasail_free(pvE);

    return result;
}

