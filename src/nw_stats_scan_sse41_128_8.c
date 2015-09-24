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

#define NEG_INF INT8_MIN
#define MAX(a,b) ((a)>(b)?(a):(b))


#ifdef PARASAIL_TABLE
static inline void arr_store_si128(
        int *array,
        __m128i vH,
        int32_t t,
        int32_t seglen,
        int32_t d,
        int32_t dlen)
{
    array[( 0*seglen+t)*dlen + d] = (int8_t)_mm_extract_epi8(vH,  0);
    array[( 1*seglen+t)*dlen + d] = (int8_t)_mm_extract_epi8(vH,  1);
    array[( 2*seglen+t)*dlen + d] = (int8_t)_mm_extract_epi8(vH,  2);
    array[( 3*seglen+t)*dlen + d] = (int8_t)_mm_extract_epi8(vH,  3);
    array[( 4*seglen+t)*dlen + d] = (int8_t)_mm_extract_epi8(vH,  4);
    array[( 5*seglen+t)*dlen + d] = (int8_t)_mm_extract_epi8(vH,  5);
    array[( 6*seglen+t)*dlen + d] = (int8_t)_mm_extract_epi8(vH,  6);
    array[( 7*seglen+t)*dlen + d] = (int8_t)_mm_extract_epi8(vH,  7);
    array[( 8*seglen+t)*dlen + d] = (int8_t)_mm_extract_epi8(vH,  8);
    array[( 9*seglen+t)*dlen + d] = (int8_t)_mm_extract_epi8(vH,  9);
    array[(10*seglen+t)*dlen + d] = (int8_t)_mm_extract_epi8(vH, 10);
    array[(11*seglen+t)*dlen + d] = (int8_t)_mm_extract_epi8(vH, 11);
    array[(12*seglen+t)*dlen + d] = (int8_t)_mm_extract_epi8(vH, 12);
    array[(13*seglen+t)*dlen + d] = (int8_t)_mm_extract_epi8(vH, 13);
    array[(14*seglen+t)*dlen + d] = (int8_t)_mm_extract_epi8(vH, 14);
    array[(15*seglen+t)*dlen + d] = (int8_t)_mm_extract_epi8(vH, 15);
}
#endif

#ifdef PARASAIL_ROWCOL
static inline void arr_store_col(
        int *col,
        __m128i vH,
        int32_t t,
        int32_t seglen)
{
    col[ 0*seglen+t] = (int8_t)_mm_extract_epi8(vH,  0);
    col[ 1*seglen+t] = (int8_t)_mm_extract_epi8(vH,  1);
    col[ 2*seglen+t] = (int8_t)_mm_extract_epi8(vH,  2);
    col[ 3*seglen+t] = (int8_t)_mm_extract_epi8(vH,  3);
    col[ 4*seglen+t] = (int8_t)_mm_extract_epi8(vH,  4);
    col[ 5*seglen+t] = (int8_t)_mm_extract_epi8(vH,  5);
    col[ 6*seglen+t] = (int8_t)_mm_extract_epi8(vH,  6);
    col[ 7*seglen+t] = (int8_t)_mm_extract_epi8(vH,  7);
    col[ 8*seglen+t] = (int8_t)_mm_extract_epi8(vH,  8);
    col[ 9*seglen+t] = (int8_t)_mm_extract_epi8(vH,  9);
    col[10*seglen+t] = (int8_t)_mm_extract_epi8(vH, 10);
    col[11*seglen+t] = (int8_t)_mm_extract_epi8(vH, 11);
    col[12*seglen+t] = (int8_t)_mm_extract_epi8(vH, 12);
    col[13*seglen+t] = (int8_t)_mm_extract_epi8(vH, 13);
    col[14*seglen+t] = (int8_t)_mm_extract_epi8(vH, 14);
    col[15*seglen+t] = (int8_t)_mm_extract_epi8(vH, 15);
}
#endif

#ifdef PARASAIL_TABLE
#define FNAME parasail_nw_stats_table_scan_sse41_128_8
#define PNAME parasail_nw_stats_table_scan_profile_sse41_128_8
#else
#ifdef PARASAIL_ROWCOL
#define FNAME parasail_nw_stats_rowcol_scan_sse41_128_8
#define PNAME parasail_nw_stats_rowcol_scan_profile_sse41_128_8
#else
#define FNAME parasail_nw_stats_scan_sse41_128_8
#define PNAME parasail_nw_stats_scan_profile_sse41_128_8
#endif
#endif

parasail_result_t* FNAME(
        const char * const restrict s1, const int s1Len,
        const char * const restrict s2, const int s2Len,
        const int open, const int gap, const parasail_matrix_t *matrix)
{
    parasail_profile_t *profile = parasail_profile_create_stats_sse_128_8(s1, s1Len, matrix);
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
    const int32_t segWidth = 16;
    const int32_t segLen = (s1Len + segWidth - 1) / segWidth;
    const int32_t offset = (s1Len - 1) % segLen;
    const int32_t position = (segWidth - 1) - (s1Len - 1) / segLen;
    __m128i* const restrict pvP  = (__m128i*)profile->profile8.score;
    __m128i* const restrict pvPm = (__m128i*)profile->profile8.matches;
    __m128i* const restrict pvPs = (__m128i*)profile->profile8.similar;
    __m128i* const restrict pvE  = parasail_memalign___m128i(16, segLen);
    __m128i* const restrict pvHt = parasail_memalign___m128i(16, segLen);
    __m128i* const restrict pvFt = parasail_memalign___m128i(16, segLen);
    __m128i* const restrict pvMt = parasail_memalign___m128i(16, segLen);
    __m128i* const restrict pvSt = parasail_memalign___m128i(16, segLen);
    __m128i* const restrict pvLt = parasail_memalign___m128i(16, segLen);
    __m128i* const restrict pvEx = parasail_memalign___m128i(16, segLen);
    __m128i* const restrict pvH  = parasail_memalign___m128i(16, segLen);
    __m128i* const restrict pvM  = parasail_memalign___m128i(16, segLen);
    __m128i* const restrict pvS  = parasail_memalign___m128i(16, segLen);
    __m128i* const restrict pvL  = parasail_memalign___m128i(16, segLen);
    int8_t* const restrict boundary = parasail_memalign_int8_t(16, s2Len+1);
    __m128i vGapO = _mm_set1_epi8(open);
    __m128i vGapE = _mm_set1_epi8(gap);
    __m128i vZero = _mm_setzero_si128();
    __m128i vOne = _mm_set1_epi8(1);
    __m128i vNegInf = _mm_set1_epi8(NEG_INF);
    int8_t score = NEG_INF;
    int8_t matches = 0;
    int8_t similar = 0;
    int8_t length = 0;
    __m128i vNegLimit = _mm_set1_epi8(INT8_MIN);
    __m128i vPosLimit = _mm_set1_epi8(INT8_MAX);
    __m128i vSaturationCheckMin = vPosLimit;
    __m128i vSaturationCheckMax = vNegLimit;
#ifdef PARASAIL_TABLE
    parasail_result_t *result = parasail_result_new_table3(segLen*segWidth, s2Len);
#else
#ifdef PARASAIL_ROWCOL
    parasail_result_t *result = parasail_result_new_rowcol3(segLen*segWidth, s2Len);
#else
    parasail_result_t *result = parasail_result_new();
#endif
#endif

    parasail_memset___m128i(pvM, vZero, segLen);
    parasail_memset___m128i(pvS, vZero, segLen);
    parasail_memset___m128i(pvL, vZero, segLen);

    /* initialize H and E */
    {
        int32_t index = 0;
        for (i=0; i<segLen; ++i) {
            __m128i_8_t h;
            __m128i_8_t e;
            for (segNum=0; segNum<segWidth; ++segNum) {
                int64_t tmp = -open-gap*(segNum*segLen+i);
                h.v[segNum] = tmp < INT8_MIN ? INT8_MIN : tmp;
                e.v[segNum] = NEG_INF;
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
        __m128i *pvW;
        __m128i vW;
        __m128i *pvC;
        __m128i *pvD;
        __m128i vC;
        __m128i vD;
        __m128i vM;
        __m128i vMp;
        __m128i vMt;
        __m128i vS;
        __m128i vSp;
        __m128i vSt;
        __m128i vL;
        __m128i vLp;
        __m128i vLt;
        __m128i vEx;

        /* calculate E */
        for (i=0; i<segLen; ++i) {
            vH = _mm_load_si128(pvH+i);
            vE = _mm_load_si128(pvE+i);
            vE = _mm_max_epi8(
                    _mm_subs_epi8(vE, vGapE),
                    _mm_subs_epi8(vH, vGapO));
            _mm_store_si128(pvE+i, vE);
        }

        /* calculate Ht */
        vH = _mm_slli_si128(_mm_load_si128(pvH+(segLen-1)), 1);
        vH = _mm_insert_epi8(vH, boundary[j], 0);
        vMp= _mm_slli_si128(_mm_load_si128(pvM+(segLen-1)), 1);
        vSp= _mm_slli_si128(_mm_load_si128(pvS+(segLen-1)), 1);
        vLp= _mm_slli_si128(_mm_load_si128(pvL+(segLen-1)), 1);
        vLp= _mm_adds_epi8(vLp, vOne);
        pvW = pvP + matrix->mapper[(unsigned char)s2[j]]*segLen;
        pvC = pvPm+ matrix->mapper[(unsigned char)s2[j]]*segLen;
        pvD = pvPs+ matrix->mapper[(unsigned char)s2[j]]*segLen;
        for (i=0; i<segLen; ++i) {
            /* load values we need */
            vE = _mm_load_si128(pvE+i);
            vW = _mm_load_si128(pvW+i);
            /* compute */
            vH = _mm_adds_epi8(vH, vW);
            vHt = _mm_max_epi8(vH, vE);
            /* statistics */
            vC = _mm_load_si128(pvC+i);
            vD = _mm_load_si128(pvD+i);
            vMp = _mm_adds_epi8(vMp, vC);
            vSp = _mm_adds_epi8(vSp, vD);
            vEx = _mm_cmpgt_epi8(vE, vH);
            vM = _mm_load_si128(pvM+i);
            vS = _mm_load_si128(pvS+i);
            vL = _mm_load_si128(pvL+i);
            vL = _mm_adds_epi8(vL, vOne);
            vMt = _mm_blendv_epi8(vMp, vM, vEx);
            vSt = _mm_blendv_epi8(vSp, vS, vEx);
            vLt = _mm_blendv_epi8(vLp, vL, vEx);
            /* store results */
            _mm_store_si128(pvHt+i, vHt);
            _mm_store_si128(pvEx+i, vEx);
            _mm_store_si128(pvMt+i, vMt);
            _mm_store_si128(pvSt+i, vSt);
            _mm_store_si128(pvLt+i, vLt);
            /* prep for next iteration */
            vH = _mm_load_si128(pvH+i);
            vMp = vM;
            vSp = vS;
            vLp = vL;
        }

        /* calculate Ft */
        vHt = _mm_slli_si128(_mm_load_si128(pvHt+(segLen-1)), 1);
        vHt = _mm_insert_epi8(vHt, boundary[j+1], 0);
        vFt = vNegInf;
        for (i=0; i<segLen; ++i) {
            vFt = _mm_subs_epi8(vFt, vGapE);
            vFt = _mm_max_epi8(vFt, vHt);
            vHt = _mm_load_si128(pvHt+i);
        }
        {
            __m128i_8_t tmp;
            tmp.m = vFt;
            tmp.v[ 1] = MAX(tmp.v[ 0]-segLen*gap, tmp.v[ 1]);
            tmp.v[ 2] = MAX(tmp.v[ 1]-segLen*gap, tmp.v[ 2]);
            tmp.v[ 3] = MAX(tmp.v[ 2]-segLen*gap, tmp.v[ 3]);
            tmp.v[ 4] = MAX(tmp.v[ 3]-segLen*gap, tmp.v[ 4]);
            tmp.v[ 5] = MAX(tmp.v[ 4]-segLen*gap, tmp.v[ 5]);
            tmp.v[ 6] = MAX(tmp.v[ 5]-segLen*gap, tmp.v[ 6]);
            tmp.v[ 7] = MAX(tmp.v[ 6]-segLen*gap, tmp.v[ 7]);
            tmp.v[ 8] = MAX(tmp.v[ 7]-segLen*gap, tmp.v[ 8]);
            tmp.v[ 9] = MAX(tmp.v[ 8]-segLen*gap, tmp.v[ 9]);
            tmp.v[10] = MAX(tmp.v[ 9]-segLen*gap, tmp.v[10]);
            tmp.v[11] = MAX(tmp.v[10]-segLen*gap, tmp.v[11]);
            tmp.v[12] = MAX(tmp.v[11]-segLen*gap, tmp.v[12]);
            tmp.v[13] = MAX(tmp.v[12]-segLen*gap, tmp.v[13]);
            tmp.v[14] = MAX(tmp.v[13]-segLen*gap, tmp.v[14]);
            tmp.v[15] = MAX(tmp.v[14]-segLen*gap, tmp.v[15]);
            vFt = tmp.m;
        }
        vHt = _mm_slli_si128(_mm_load_si128(pvHt+(segLen-1)), 1);
        vHt = _mm_insert_epi8(vHt, boundary[j+1], 0);
        vFt = _mm_slli_si128(vFt, 1);
        vFt = _mm_insert_epi8(vFt, NEG_INF, 0);
        for (i=0; i<segLen; ++i) {
            vFt = _mm_subs_epi8(vFt, vGapE);
            vFt = _mm_max_epi8(vFt, vHt);
            vHt = _mm_load_si128(pvHt+i);
            _mm_store_si128(pvFt+i, vFt);
        }

        /* calculate H,M,L */
        vMp = vZero;
        vSp = vZero;
        vLp = vOne;
        vC = _mm_cmpeq_epi8(vZero, vZero); /* check if prefix sum is needed */
        vC = _mm_srli_si128(vC, 1); /* zero out last value */
        for (i=0; i<segLen; ++i) {
            /* load values we need */
            vHt = _mm_load_si128(pvHt+i);
            vFt = _mm_load_si128(pvFt+i);
            /* compute */
            vFt = _mm_subs_epi8(vFt, vGapO);
            vH = _mm_max_epi8(vHt, vFt);
            /* statistics */
            vEx = _mm_load_si128(pvEx+i);
            vMt = _mm_load_si128(pvMt+i);
            vSt = _mm_load_si128(pvSt+i);
            vLt = _mm_load_si128(pvLt+i);
            vEx = _mm_or_si128(
                    _mm_and_si128(vEx, _mm_cmpeq_epi8(vHt, vFt)),
                    _mm_cmplt_epi8(vHt, vFt));
            vM = _mm_blendv_epi8(vMt, vMp, vEx);
            vS = _mm_blendv_epi8(vSt, vSp, vEx);
            vL = _mm_blendv_epi8(vLt, vLp, vEx);
            vMp = vM;
            vSp = vS;
            vLp = _mm_adds_epi8(vL, vOne);
            vC = _mm_and_si128(vC, vEx);
            /* store results */
            _mm_store_si128(pvH+i, vH);
            _mm_store_si128(pvEx+i, vEx);
            /* check for saturation */
            {
                vSaturationCheckMax = _mm_max_epi8(vSaturationCheckMax, vH);
                vSaturationCheckMin = _mm_min_epi8(vSaturationCheckMin, vH);
            }
#ifdef PARASAIL_TABLE
            arr_store_si128(result->score_table, vH, i, segLen, j, s2Len);
#endif
        }
        {
            vLp = _mm_subs_epi8(vLp, vOne);
            {
                __m128i_8_t uMp, uSp, uLp, uC;
                uC.m = vC;
                uMp.m = vMp;
                uMp.v[ 1] = uC.v[ 1] ? uMp.v[ 0] : uMp.v[ 1];
                uMp.v[ 2] = uC.v[ 2] ? uMp.v[ 1] : uMp.v[ 2];
                uMp.v[ 3] = uC.v[ 3] ? uMp.v[ 2] : uMp.v[ 3];
                uMp.v[ 4] = uC.v[ 4] ? uMp.v[ 3] : uMp.v[ 4];
                uMp.v[ 5] = uC.v[ 5] ? uMp.v[ 4] : uMp.v[ 5];
                uMp.v[ 6] = uC.v[ 6] ? uMp.v[ 5] : uMp.v[ 6];
                uMp.v[ 7] = uC.v[ 7] ? uMp.v[ 6] : uMp.v[ 7];
                uMp.v[ 8] = uC.v[ 8] ? uMp.v[ 7] : uMp.v[ 8];
                uMp.v[ 9] = uC.v[ 9] ? uMp.v[ 8] : uMp.v[ 9];
                uMp.v[10] = uC.v[10] ? uMp.v[ 9] : uMp.v[10];
                uMp.v[11] = uC.v[11] ? uMp.v[10] : uMp.v[11];
                uMp.v[12] = uC.v[12] ? uMp.v[11] : uMp.v[12];
                uMp.v[13] = uC.v[13] ? uMp.v[12] : uMp.v[13];
                uMp.v[14] = uC.v[14] ? uMp.v[13] : uMp.v[14];
                uMp.v[15] = uC.v[15] ? uMp.v[14] : uMp.v[15];
                vMp = uMp.m;
                uSp.m = vSp;
                uSp.v[ 1] = uC.v[ 1] ? uSp.v[ 0] : uSp.v[ 1];
                uSp.v[ 2] = uC.v[ 2] ? uSp.v[ 1] : uSp.v[ 2];
                uSp.v[ 3] = uC.v[ 3] ? uSp.v[ 2] : uSp.v[ 3];
                uSp.v[ 4] = uC.v[ 4] ? uSp.v[ 3] : uSp.v[ 4];
                uSp.v[ 5] = uC.v[ 5] ? uSp.v[ 4] : uSp.v[ 5];
                uSp.v[ 6] = uC.v[ 6] ? uSp.v[ 5] : uSp.v[ 6];
                uSp.v[ 7] = uC.v[ 7] ? uSp.v[ 6] : uSp.v[ 7];
                uSp.v[ 8] = uC.v[ 8] ? uSp.v[ 7] : uSp.v[ 8];
                uSp.v[ 9] = uC.v[ 9] ? uSp.v[ 8] : uSp.v[ 9];
                uSp.v[10] = uC.v[10] ? uSp.v[ 9] : uSp.v[10];
                uSp.v[11] = uC.v[11] ? uSp.v[10] : uSp.v[11];
                uSp.v[12] = uC.v[12] ? uSp.v[11] : uSp.v[12];
                uSp.v[13] = uC.v[13] ? uSp.v[12] : uSp.v[13];
                uSp.v[14] = uC.v[14] ? uSp.v[13] : uSp.v[14];
                uSp.v[15] = uC.v[15] ? uSp.v[14] : uSp.v[15];
                vSp = uSp.m;
                uLp.m = vLp;
                uLp.v[ 1] = uC.v[ 1] ? uLp.v[ 1] + uLp.v[ 0] : uLp.v[ 1];
                uLp.v[ 2] = uC.v[ 2] ? uLp.v[ 2] + uLp.v[ 1] : uLp.v[ 2];
                uLp.v[ 3] = uC.v[ 3] ? uLp.v[ 3] + uLp.v[ 2] : uLp.v[ 3];
                uLp.v[ 4] = uC.v[ 4] ? uLp.v[ 4] + uLp.v[ 3] : uLp.v[ 4];
                uLp.v[ 5] = uC.v[ 5] ? uLp.v[ 5] + uLp.v[ 4] : uLp.v[ 5];
                uLp.v[ 6] = uC.v[ 6] ? uLp.v[ 6] + uLp.v[ 5] : uLp.v[ 6];
                uLp.v[ 7] = uC.v[ 7] ? uLp.v[ 7] + uLp.v[ 6] : uLp.v[ 7];
                uLp.v[ 8] = uC.v[ 8] ? uLp.v[ 8] + uLp.v[ 7] : uLp.v[ 8];
                uLp.v[ 9] = uC.v[ 9] ? uLp.v[ 9] + uLp.v[ 8] : uLp.v[ 9];
                uLp.v[10] = uC.v[10] ? uLp.v[10] + uLp.v[ 9] : uLp.v[10];
                uLp.v[11] = uC.v[11] ? uLp.v[11] + uLp.v[10] : uLp.v[11];
                uLp.v[12] = uC.v[12] ? uLp.v[12] + uLp.v[11] : uLp.v[12];
                uLp.v[13] = uC.v[13] ? uLp.v[13] + uLp.v[12] : uLp.v[13];
                uLp.v[14] = uC.v[14] ? uLp.v[14] + uLp.v[13] : uLp.v[14];
                uLp.v[15] = uC.v[15] ? uLp.v[15] + uLp.v[14] : uLp.v[15];
                vLp = uLp.m;
            }
            vLp = _mm_adds_epi8(vLp, vOne);
        }
        /* final pass for M,L */
        vMp = _mm_slli_si128(vMp, 1);
        vSp = _mm_slli_si128(vSp, 1);
        vLp = _mm_slli_si128(vLp, 1);
        for (i=0; i<segLen; ++i) {
            /* statistics */
            vEx = _mm_load_si128(pvEx+i);
            vMt = _mm_load_si128(pvMt+i);
            vSt = _mm_load_si128(pvSt+i);
            vLt = _mm_load_si128(pvLt+i);
            vM = _mm_blendv_epi8(vMt, vMp, vEx);
            vS = _mm_blendv_epi8(vSt, vSp, vEx);
            vL = _mm_blendv_epi8(vLt, vLp, vEx);
            vMp = vM;
            vSp = vS;
            vLp = _mm_adds_epi8(vL, vOne);
            /* store results */
            _mm_store_si128(pvM+i, vM);
            _mm_store_si128(pvS+i, vS);
            _mm_store_si128(pvL+i, vL);
            /* check for saturation */
            {
                vSaturationCheckMax = _mm_max_epi8(vSaturationCheckMax, vM);
                vSaturationCheckMax = _mm_max_epi8(vSaturationCheckMax, vS);
                vSaturationCheckMax = _mm_max_epi8(vSaturationCheckMax, vL);
            }
#ifdef PARASAIL_TABLE
            arr_store_si128(result->matches_table, vM, i, segLen, j, s2Len);
            arr_store_si128(result->similar_table, vS, i, segLen, j, s2Len);
            arr_store_si128(result->length_table, vL, i, segLen, j, s2Len);
#endif
        }

#ifdef PARASAIL_ROWCOL
        /* extract last value from the column */
        {
            vH = _mm_load_si128(pvH + offset);
            vM = _mm_load_si128(pvM + offset);
            vS = _mm_load_si128(pvS + offset);
            vL = _mm_load_si128(pvL + offset);
            for (k=0; k<position; ++k) {
                vH = _mm_slli_si128(vH, 1);
                vM = _mm_slli_si128(vM, 1);
                vS = _mm_slli_si128(vS, 1);
                vL = _mm_slli_si128(vL, 1);
            }
            result->score_row[j] = (int8_t) _mm_extract_epi8 (vH, 15);
            result->matches_row[j] = (int8_t) _mm_extract_epi8 (vM, 15);
            result->similar_row[j] = (int8_t) _mm_extract_epi8 (vS, 15);
            result->length_row[j] = (int8_t) _mm_extract_epi8 (vL, 15);
        }
#endif
    }

#ifdef PARASAIL_ROWCOL
    for (i=0; i<segLen; ++i) {
        __m128i vH = _mm_load_si128(pvH+i);
        __m128i vM = _mm_load_si128(pvM+i);
        __m128i vS = _mm_load_si128(pvS+i);
        __m128i vL = _mm_load_si128(pvL+i);
        arr_store_col(result->score_col, vH, i, segLen);
        arr_store_col(result->matches_col, vM, i, segLen);
        arr_store_col(result->similar_col, vS, i, segLen);
        arr_store_col(result->length_col, vL, i, segLen);
    }
#endif

    /* extract last value from the last column */
    {
        __m128i vH = _mm_load_si128(pvH + offset);
        __m128i vM = _mm_load_si128(pvM + offset);
        __m128i vS = _mm_load_si128(pvS + offset);
        __m128i vL = _mm_load_si128(pvL + offset);
        for (k=0; k<position; ++k) {
            vH = _mm_slli_si128(vH, 1);
            vM = _mm_slli_si128(vM, 1);
            vS = _mm_slli_si128(vS, 1);
            vL = _mm_slli_si128(vL, 1);
        }
        score = (int8_t) _mm_extract_epi8 (vH, 15);
        matches = (int8_t) _mm_extract_epi8 (vM, 15);
        similar = (int8_t) _mm_extract_epi8 (vS, 15);
        length = (int8_t) _mm_extract_epi8 (vL, 15);
    }

    if (_mm_movemask_epi8(_mm_or_si128(
            _mm_cmpeq_epi8(vSaturationCheckMin, vNegLimit),
            _mm_cmpeq_epi8(vSaturationCheckMax, vPosLimit)))) {
        result->saturated = 1;
        score = INT8_MAX;
        matches = 0;
        similar = 0;
        length = 0;
        end_query = 0;
        end_ref = 0;
    }

    result->score = score;
    result->matches = matches;
    result->similar = similar;
    result->length = length;
    result->end_query = end_query;
    result->end_ref = end_ref;

    parasail_free(boundary);
    parasail_free(pvL);
    parasail_free(pvS);
    parasail_free(pvM);
    parasail_free(pvH);
    parasail_free(pvEx);
    parasail_free(pvLt);
    parasail_free(pvSt);
    parasail_free(pvMt);
    parasail_free(pvFt);
    parasail_free(pvHt);
    parasail_free(pvE);

    return result;
}


