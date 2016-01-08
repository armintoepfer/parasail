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

#include <immintrin.h>

#include "parasail.h"
#include "parasail/memory.h"
#include "parasail/internal_avx.h"

#define SEGWIDTH 4
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

#if HAVE_AVX2_MM256_EXTRACT_EPI64
#define _mm256_extract_epi64_rpl _mm256_extract_epi64
#else
static inline int64_t _mm256_extract_epi64_rpl(__m256i a, int imm) {
    __m256i_64_t A;
    A.m = a;
    return A.v[imm];
}
#endif

#define _mm256_slli_si256_rpl(a,imm) _mm256_alignr_epi8(a, _mm256_permute2x128_si256(a, a, _MM_SHUFFLE(0,0,3,0)), 16-imm)

static inline int64_t _mm256_hmax_epi64_rpl(__m256i a) {
    a = _mm256_max_epi64_rpl(a, _mm256_permute2x128_si256(a, a, _MM_SHUFFLE(0,0,0,0)));
    a = _mm256_max_epi64_rpl(a, _mm256_slli_si256(a, 8));
    return _mm256_extract_epi64_rpl(a, 3);
}


#ifdef PARASAIL_TABLE
static inline void arr_store_si256(
        int *array,
        __m256i vH,
        int32_t t,
        int32_t seglen,
        int32_t d,
        int32_t dlen)
{
    array[(0*seglen+t)*dlen + d] = (int64_t)_mm256_extract_epi64_rpl(vH, 0);
    array[(1*seglen+t)*dlen + d] = (int64_t)_mm256_extract_epi64_rpl(vH, 1);
    array[(2*seglen+t)*dlen + d] = (int64_t)_mm256_extract_epi64_rpl(vH, 2);
    array[(3*seglen+t)*dlen + d] = (int64_t)_mm256_extract_epi64_rpl(vH, 3);
}
#endif

#ifdef PARASAIL_ROWCOL
static inline void arr_store_col(
        int *col,
        __m256i vH,
        int32_t t,
        int32_t seglen)
{
    col[0*seglen+t] = (int64_t)_mm256_extract_epi64_rpl(vH, 0);
    col[1*seglen+t] = (int64_t)_mm256_extract_epi64_rpl(vH, 1);
    col[2*seglen+t] = (int64_t)_mm256_extract_epi64_rpl(vH, 2);
    col[3*seglen+t] = (int64_t)_mm256_extract_epi64_rpl(vH, 3);
}
#endif

#ifdef PARASAIL_TABLE
#define FNAME parasail_sw_table_scan_avx2_256_64
#define PNAME parasail_sw_table_scan_profile_avx2_256_64
#else
#ifdef PARASAIL_ROWCOL
#define FNAME parasail_sw_rowcol_scan_avx2_256_64
#define PNAME parasail_sw_rowcol_scan_profile_avx2_256_64
#else
#define FNAME parasail_sw_scan_avx2_256_64
#define PNAME parasail_sw_scan_profile_avx2_256_64
#endif
#endif

parasail_result_t* FNAME(
        const char * const restrict s1, const int s1Len,
        const char * const restrict s2, const int s2Len,
        const int open, const int gap, const parasail_matrix_t *matrix)
{
    parasail_profile_t *profile = parasail_profile_create_avx_256_64(s1, s1Len, matrix);
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
    const int32_t segWidth = 4; /* number of values in vector unit */
    const int32_t segLen = (s1Len + segWidth - 1) / segWidth;
    __m256i* const restrict pvP = (__m256i*)profile->profile64.score;
    __m256i* const restrict pvE = parasail_memalign___m256i(32, segLen);
    __m256i* const restrict pvHt= parasail_memalign___m256i(32, segLen);
    __m256i* const restrict pvH = parasail_memalign___m256i(32, segLen);
    __m256i* const restrict pvHMax = parasail_memalign___m256i(32, segLen);
    __m256i* const restrict pvGapper = parasail_memalign___m256i(32, segLen);
    __m256i vGapO = _mm256_set1_epi64x(open);
    __m256i vGapE = _mm256_set1_epi64x(gap);
    __m256i vNegInf = _mm256_set1_epi64x(NEG_INF);
    __m256i vZero = _mm256_setzero_si256();
    int64_t score = NEG_INF;
    __m256i vMaxH = vNegInf;
    __m256i vMaxHUnit = vNegInf;
#if SEGWIDTH > 2
    __m256i vSegLenXgap = _mm256_set1_epi64x(segLen*gap);
#endif
    __m256i vNegInfFront = _mm256_set_epi64x(0,0,0,NEG_INF);
    
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

    parasail_memset___m256i(pvH, vZero, segLen);
    parasail_memset___m256i(pvE, vNegInf, segLen);
    {
        __m256i vGapper = _mm256_sub_epi64(vZero,vGapO);
        for (i=segLen-1; i>=0; --i) {
            _mm256_store_si256(pvGapper+i, vGapper);
            vGapper = _mm256_sub_epi64(vGapper, vGapE);
        }
    }

    /* outer loop over database sequence */
    for (j=0; j<s2Len; ++j) {
        __m256i vE;
        __m256i vHt;
        __m256i vF;
        __m256i vH;
        __m256i vHp;
        __m256i *pvW;
        __m256i vW;

        /* calculate E */
        /* calculate Ht */
        /* calculate F and H first pass */
        vHp = _mm256_load_si256(pvH+(segLen-1));
        vHp = _mm256_slli_si256_rpl(vHp, 8);
        pvW = pvP + matrix->mapper[(unsigned char)s2[j]]*segLen;
        vHt = vZero;
        vF = vNegInf;
        for (i=0; i<segLen; ++i) {
            vH = _mm256_load_si256(pvH+i);
            vE = _mm256_load_si256(pvE+i);
            vW = _mm256_load_si256(pvW+i);
            vE = _mm256_max_epi64_rpl(
                    _mm256_sub_epi64(vE, vGapE),
                    _mm256_sub_epi64(vH, vGapO));
            vHp = _mm256_add_epi64(vHp, vW);
            vF = _mm256_max_epi64_rpl(vF, _mm256_add_epi64(vHt, pvGapper[i]));
            vHt = _mm256_max_epi64_rpl(vE, vHp);
            _mm256_store_si256(pvE+i, vE);
            _mm256_store_si256(pvHt+i, vHt);
            vHp = vH;
        }

        /* pseudo prefix scan on F and H */
        vHt = _mm256_slli_si256_rpl(vHt, 8);
        vF = _mm256_max_epi64_rpl(vF, _mm256_add_epi64(vHt, pvGapper[0]));
        vF = _mm256_slli_si256_rpl(vF, 8);
#if SEGWIDTH > 2
        vF = _mm256_add_epi64(vF, vNegInfFront);
        for (i=0; i<segWidth-2; ++i) {
            __m256i vFt = _mm256_sub_epi64(vF, vSegLenXgap);
            vFt = _mm256_slli_si256_rpl(vFt, 8);
            vF = _mm256_max_epi64_rpl(vF, vFt);
        }
#endif

        /* calculate final H */
        vF = _mm256_add_epi64(vF, vNegInfFront);
        vH = _mm256_max_epi64_rpl(vHt, vF);
        for (i=0; i<segLen; ++i) {
            vHt = _mm256_load_si256(pvHt+i);
            vF = _mm256_max_epi64_rpl(
                    _mm256_sub_epi64(vF, vGapE),
                    _mm256_sub_epi64(vH, vGapO));
            vH = _mm256_max_epi64_rpl(vHt, vF);
            vH = _mm256_max_epi64_rpl(vH, vZero);
            _mm256_store_si256(pvH+i, vH);
            
#ifdef PARASAIL_TABLE
            arr_store_si256(result->score_table, vH, i, segLen, j, s2Len);
#endif
            vMaxH = _mm256_max_epi64_rpl(vH, vMaxH);
        } 

        {
            __m256i vCompare = _mm256_cmpgt_epi64(vMaxH, vMaxHUnit);
            if (_mm256_movemask_epi8(vCompare)) {
                score = _mm256_hmax_epi64_rpl(vMaxH);
                vMaxHUnit = _mm256_set1_epi64x(score);
                end_ref = j;
                (void)memcpy(pvHMax, pvH, sizeof(__m256i)*segLen);
            }
        }

#ifdef PARASAIL_ROWCOL
        /* extract last value from the column */
        {
            int32_t k = 0;
            __m256i vH = _mm256_load_si256(pvH + offset);
            for (k=0; k<position; ++k) {
                vH = _mm256_slli_si256_rpl(vH, 8);
            }
            result->score_row[j] = (int64_t) _mm256_extract_epi64_rpl (vH, 3);
        }
#endif
    }

    /* Trace the alignment ending position on read. */
    {
        int64_t *t = (int64_t*)pvHMax;
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
        __m256i vH = _mm256_load_si256(pvH+i);
        arr_store_col(result->score_col, vH, i, segLen);
    }
#endif

    

    result->score = score;
    result->end_query = end_query;
    result->end_ref = end_ref;
    result->flag = PARASAIL_FLAG_SW | PARASAIL_FLAG_SCAN
        | PARASAIL_FLAG_BITS_64 | PARASAIL_FLAG_LANES_4;
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

