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
#include <smmintrin.h>

#include "parasail.h"
#include "parasail/memory.h"
#include "parasail/internal_sse.h"

#define NEG_INF (INT32_MIN/(int32_t)(2))


static inline void arr_store_si128(
        int *array,
        __m128i vH,
        int32_t t,
        int32_t seglen,
        int32_t d,
        int32_t dlen)
{
    array[(0*seglen+t)*dlen + d] = (int32_t)_mm_extract_epi32(vH, 0);
    array[(1*seglen+t)*dlen + d] = (int32_t)_mm_extract_epi32(vH, 1);
    array[(2*seglen+t)*dlen + d] = (int32_t)_mm_extract_epi32(vH, 2);
    array[(3*seglen+t)*dlen + d] = (int32_t)_mm_extract_epi32(vH, 3);
}

#define FNAME parasail_nw_trace_striped_sse41_128_32
#define PNAME parasail_nw_trace_striped_profile_sse41_128_32

parasail_result_t* FNAME(
        const char * const restrict s1, const int s1Len,
        const char * const restrict s2, const int s2Len,
        const int open, const int gap, const parasail_matrix_t *matrix)
{
    parasail_profile_t *profile = parasail_profile_create_sse_128_32(s1, s1Len, matrix);
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
    const int32_t segWidth = 4; /* number of values in vector unit */
    const int32_t segLen = (s1Len + segWidth - 1) / segWidth;
    const int32_t offset = (s1Len - 1) % segLen;
    const int32_t position = (segWidth - 1) - (s1Len - 1) / segLen;
    __m128i* const restrict vProfile = (__m128i*)profile->profile32.score;
    __m128i* restrict pvHStore = parasail_memalign___m128i(16, segLen);
    __m128i* restrict pvHLoad =  parasail_memalign___m128i(16, segLen);
    __m128i* const restrict pvE = parasail_memalign___m128i(16, segLen);
    int32_t* const restrict boundary = parasail_memalign_int32_t(16, s2Len+1);
    __m128i vGapO = _mm_set1_epi32(open);
    __m128i vGapE = _mm_set1_epi32(gap);
    __m128i vNegInf = _mm_set1_epi32(NEG_INF);
    int32_t score = NEG_INF;
    
    parasail_result_t *result = parasail_result_new_trace(segLen*segWidth, s2Len);

    /* initialize H and E */
    {
        int32_t index = 0;
        for (i=0; i<segLen; ++i) {
            __m128i_32_t h;
            __m128i_32_t e;
            for (segNum=0; segNum<segWidth; ++segNum) {
                int64_t tmp = -open-gap*(segNum*segLen+i);
                h.v[segNum] = tmp < INT32_MIN ? INT32_MIN : tmp;
                tmp = tmp - open;
                e.v[segNum] = tmp < INT32_MIN ? INT32_MIN : tmp;
            }
            _mm_store_si128(&pvHStore[index], h.m);
            _mm_store_si128(&pvE[index], e.m);
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
        __m128i vE;
        /* Initialize F value to -inf.  Any errors to vH values will be
         * corrected in the Lazy_F loop.  */
        __m128i vF = vNegInf;

        /* load final segment of pvHStore and shift left by 2 bytes */
        __m128i vH = _mm_slli_si128(pvHStore[segLen - 1], 4);

        /* insert upper boundary condition */
        vH = _mm_insert_epi32(vH, boundary[j], 0);

        /* Correct part of the vProfile */
        const __m128i* vP = vProfile + matrix->mapper[(unsigned char)s2[j]] * segLen;

        /* Swap the 2 H buffers. */
        __m128i* pv = pvHLoad;
        pvHLoad = pvHStore;
        pvHStore = pv;

        /* inner loop to process the query sequence */
        for (i=0; i<segLen; ++i) {
            vH = _mm_add_epi32(vH, _mm_load_si128(vP + i));
            vE = _mm_load_si128(pvE + i);

            /* Get max from vH, vE and vF. */
            vH = _mm_max_epi32(vH, vE);
            vH = _mm_max_epi32(vH, vF);
            /* Save vH values. */
            _mm_store_si128(pvHStore + i, vH);
            

            /* Update vE value. */
            vH = _mm_sub_epi32(vH, vGapO);
            vE = _mm_sub_epi32(vE, vGapE);
            vE = _mm_max_epi32(vE, vH);
            _mm_store_si128(pvE + i, vE);

            /* Update vF value. */
            vF = _mm_sub_epi32(vF, vGapE);
            vF = _mm_max_epi32(vF, vH);

            /* Load the next vH. */
            vH = _mm_load_si128(pvHLoad + i);
        }

        /* Lazy_F loop: has been revised to disallow adjecent insertion and
         * then deletion, so don't update E(i, i), learn from SWPS3 */
        for (k=0; k<segWidth; ++k) {
            int64_t tmp = boundary[j+1]-open;
            int32_t tmp2 = tmp < INT32_MIN ? INT32_MIN : tmp;
            vF = _mm_slli_si128(vF, 4);
            vF = _mm_insert_epi32(vF, tmp2, 0);
            for (i=0; i<segLen; ++i) {
                vH = _mm_load_si128(pvHStore + i);
                vH = _mm_max_epi32(vH,vF);
                _mm_store_si128(pvHStore + i, vH);
                
                vH = _mm_sub_epi32(vH, vGapO);
                vF = _mm_sub_epi32(vF, vGapE);
                if (! _mm_movemask_epi8(_mm_cmpgt_epi32(vF, vH))) goto end;
                /*vF = _mm_max_epi32(vF, vH);*/
            }
        }
end:
        {
        }
    }

    /* extract last value from the last column */
    {
        __m128i vH = _mm_load_si128(pvHStore + offset);
        for (k=0; k<position; ++k) {
            vH = _mm_slli_si128 (vH, 4);
        }
        score = (int32_t) _mm_extract_epi32 (vH, 3);
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


