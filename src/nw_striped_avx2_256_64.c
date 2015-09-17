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

#define NEG_INF (INT64_MIN/(int64_t)(2))

#if HAVE_AVX2_MM256_INSERT_EPI64
#define _mm256_insert_epi64_rpl _mm256_insert_epi64
#else
static inline __m256i _mm256_insert_epi64_rpl(__m256i a, int64_t i, int imm) {
    __m256i_64_t A;
    A.m = a;
    A.v[imm] = i;
    return A.m;
}
#endif

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
#define FNAME parasail_nw_table_striped_avx2_256_64
#define PNAME parasail_nw_table_striped_profile_avx2_256_64
#else
#ifdef PARASAIL_ROWCOL
#define FNAME parasail_nw_rowcol_striped_avx2_256_64
#define PNAME parasail_nw_rowcol_striped_profile_avx2_256_64
#else
#ifdef PARASAIL_TRACE
#define FNAME parasail_nw_trace_striped_avx2_256_64
#define PNAME parasail_nw_trace_striped_profile_avx2_256_64
#else
#define FNAME parasail_nw_striped_avx2_256_64
#define PNAME parasail_nw_striped_profile_avx2_256_64
#endif
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
    int32_t k = 0;
    int32_t end_query = 0;
    int32_t end_ref = 0;
    int32_t segNum = 0;
    const int s1Len = profile->s1Len;
    const parasail_matrix_t *matrix = profile->matrix;
    const int32_t segWidth = 4; /* number of values in vector unit */
    const int32_t segLen = (s1Len + segWidth - 1) / segWidth;
    const int32_t offset = (s1Len - 1) % segLen;
    const int32_t position = (segWidth - 1) - (s1Len - 1) / segLen;
    __m256i* const restrict vProfile = (__m256i*)profile->profile64.score;
    __m256i* restrict pvHStore = parasail_memalign___m256i(32, segLen);
    __m256i* restrict pvHLoad =  parasail_memalign___m256i(32, segLen);
    __m256i* const restrict pvE = parasail_memalign___m256i(32, segLen);
    int64_t* const restrict boundary = parasail_memalign_int64_t(32, s2Len+1);
    __m256i vGapO = _mm256_set1_epi64x(open);
    __m256i vGapE = _mm256_set1_epi64x(gap);
    __m256i vNegInf = _mm256_set1_epi64x(NEG_INF);
    int64_t score = NEG_INF;
    
#ifdef PARASAIL_TABLE
    parasail_result_t *result = parasail_result_new_table1(segLen*segWidth, s2Len);
#else
#ifdef PARASAIL_ROWCOL
    parasail_result_t *result = parasail_result_new_rowcol1(segLen*segWidth, s2Len);
#else
#ifdef PARASAIL_TRACE
    parasail_result_t *result = parasail_result_new_trace(segLen*segWidth, s2Len);
#else
    parasail_result_t *result = parasail_result_new();
#endif
#endif
#endif

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
            _mm256_store_si256(&pvHStore[index], h.m);
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
        /* Initialize F value to -inf.  Any errors to vH values will be
         * corrected in the Lazy_F loop.  */
        __m256i vF = vNegInf;

        /* load final segment of pvHStore and shift left by 2 bytes */
        __m256i vH = _mm256_slli_si256_rpl(pvHStore[segLen - 1], 8);

        /* insert upper boundary condition */
        vH = _mm256_insert_epi64_rpl(vH, boundary[j], 0);

        /* Correct part of the vProfile */
        const __m256i* vP = vProfile + matrix->mapper[(unsigned char)s2[j]] * segLen;

        /* Swap the 2 H buffers. */
        __m256i* pv = pvHLoad;
        pvHLoad = pvHStore;
        pvHStore = pv;

        /* inner loop to process the query sequence */
        for (i=0; i<segLen; ++i) {
            vH = _mm256_add_epi64(vH, _mm256_load_si256(vP + i));
            vE = _mm256_load_si256(pvE + i);

            /* Get max from vH, vE and vF. */
            vH = _mm256_max_epi64_rpl(vH, vE);
            vH = _mm256_max_epi64_rpl(vH, vF);
            /* Save vH values. */
            _mm256_store_si256(pvHStore + i, vH);
            
#ifdef PARASAIL_TABLE
            arr_store_si256(result->score_table, vH, i, segLen, j, s2Len);
#endif

            /* Update vE value. */
            vH = _mm256_sub_epi64(vH, vGapO);
            vE = _mm256_sub_epi64(vE, vGapE);
            vE = _mm256_max_epi64_rpl(vE, vH);
            _mm256_store_si256(pvE + i, vE);

            /* Update vF value. */
            vF = _mm256_sub_epi64(vF, vGapE);
            vF = _mm256_max_epi64_rpl(vF, vH);

            /* Load the next vH. */
            vH = _mm256_load_si256(pvHLoad + i);
        }

        /* Lazy_F loop: has been revised to disallow adjecent insertion and
         * then deletion, so don't update E(i, i), learn from SWPS3 */
        for (k=0; k<segWidth; ++k) {
            int64_t tmp = boundary[j+1]-open;
            int64_t tmp2 = tmp < INT64_MIN ? INT64_MIN : tmp;
            vF = _mm256_slli_si256_rpl(vF, 8);
            vF = _mm256_insert_epi64_rpl(vF, tmp2, 0);
            for (i=0; i<segLen; ++i) {
                vH = _mm256_load_si256(pvHStore + i);
                vH = _mm256_max_epi64_rpl(vH,vF);
                _mm256_store_si256(pvHStore + i, vH);
                
#ifdef PARASAIL_TABLE
                arr_store_si256(result->score_table, vH, i, segLen, j, s2Len);
#endif
                vH = _mm256_sub_epi64(vH, vGapO);
                vF = _mm256_sub_epi64(vF, vGapE);
                if (! _mm256_movemask_epi8(_mm256_cmpgt_epi64(vF, vH))) goto end;
                /*vF = _mm256_max_epi64_rpl(vF, vH);*/
            }
        }
end:
        {
        }

#ifdef PARASAIL_ROWCOL
        /* extract last value from the column */
        {
            vH = _mm256_load_si256(pvHStore + offset);
            for (k=0; k<position; ++k) {
                vH = _mm256_slli_si256_rpl(vH, 8);
            }
            result->score_row[j] = (int64_t) _mm256_extract_epi64_rpl (vH, 3);
        }
#endif
    }

#ifdef PARASAIL_ROWCOL
    for (i=0; i<segLen; ++i) {
        __m256i vH = _mm256_load_si256(pvHStore+i);
        arr_store_col(result->score_col, vH, i, segLen);
    }
#endif

    /* extract last value from the last column */
    {
        __m256i vH = _mm256_load_si256(pvHStore + offset);
        for (k=0; k<position; ++k) {
            vH = _mm256_slli_si256_rpl (vH, 8);
        }
        score = (int64_t) _mm256_extract_epi64_rpl (vH, 3);
    }

    

    result->score = score;
    result->end_query = end_query;
    result->end_ref = end_ref;

    parasail_free(boundary);
    parasail_free(pvE);
    parasail_free(pvHLoad);
    parasail_free(pvHStore);

    return result;
}

