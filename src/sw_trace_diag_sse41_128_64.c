/**
 * @file
 *
 * @author jeff.daily@pnnl.gov
 *
 * Copyright (c) 2015 Battelle Memorial Institute.
 */
#include "config.h"

#include <stdlib.h>

#include <emmintrin.h>
#include <smmintrin.h>

#include "parasail.h"
#include "parasail/memory.h"
#include "parasail/internal_sse.h"

#define NEG_INF (INT64_MIN/(int64_t)(2))

static inline __m128i _mm_cmpgt_epi64_rpl(__m128i a, __m128i b) {
    __m128i_64_t A;
    __m128i_64_t B;
    A.m = a;
    B.m = b;
    A.v[0] = (A.v[0]>B.v[0]) ? 0xFFFFFFFFFFFFFFFF : 0;
    A.v[1] = (A.v[1]>B.v[1]) ? 0xFFFFFFFFFFFFFFFF : 0;
    return A.m;
}

static inline __m128i _mm_max_epi64_rpl(__m128i a, __m128i b) {
    __m128i_64_t A;
    __m128i_64_t B;
    A.m = a;
    B.m = b;
    A.v[0] = (A.v[0]>B.v[0]) ? A.v[0] : B.v[0];
    A.v[1] = (A.v[1]>B.v[1]) ? A.v[1] : B.v[1];
    return A.m;
}

static inline __m128i _mm_cmplt_epi64_rpl(__m128i a, __m128i b) {
    __m128i_64_t A;
    __m128i_64_t B;
    A.m = a;
    B.m = b;
    A.v[0] = (A.v[0]<B.v[0]) ? 0xFFFFFFFFFFFFFFFF : 0;
    A.v[1] = (A.v[1]<B.v[1]) ? 0xFFFFFFFFFFFFFFFF : 0;
    return A.m;
}


static inline void arr_store_si128(
        int *array,
        __m128i vWscore,
        int32_t i,
        int32_t s1Len,
        int32_t j,
        int32_t s2Len)
{
    if (0 <= i+0 && i+0 < s1Len && 0 <= j-0 && j-0 < s2Len) {
        array[(i+0)*s2Len + (j-0)] = (int64_t)_mm_extract_epi64(vWscore, 1);
    }
    if (0 <= i+1 && i+1 < s1Len && 0 <= j-1 && j-1 < s2Len) {
        array[(i+1)*s2Len + (j-1)] = (int64_t)_mm_extract_epi64(vWscore, 0);
    }
}

#define FNAME parasail_sw_trace_diag_sse41_128_64

parasail_result_t* FNAME(
        const char * const restrict _s1, const int s1Len,
        const char * const restrict _s2, const int s2Len,
        const int open, const int gap, const parasail_matrix_t *matrix)
{
    const int32_t N = 2; /* number of values in vector */
    const int32_t PAD = N-1;
    const int32_t PAD2 = PAD*2;
    const int32_t s1Len_PAD = s1Len+PAD;
    const int32_t s2Len_PAD = s2Len+PAD;
    int64_t * const restrict s1 = parasail_memalign_int64_t(16, s1Len+PAD);
    int64_t * const restrict s2B= parasail_memalign_int64_t(16, s2Len+PAD2);
    int64_t * const restrict _tbl_pr = parasail_memalign_int64_t(16, s2Len+PAD2);
    int64_t * const restrict _del_pr = parasail_memalign_int64_t(16, s2Len+PAD2);
    int64_t * const restrict s2 = s2B+PAD; /* will allow later for negative indices */
    int64_t * const restrict tbl_pr = _tbl_pr+PAD;
    int64_t * const restrict del_pr = _del_pr+PAD;
    parasail_result_t *result = parasail_result_new_trace(s1Len, s2Len);
    int32_t i = 0;
    int32_t j = 0;
    int64_t end_query = 0;
    int64_t end_ref = 0;
    int64_t score = NEG_INF;
    __m128i vNegInf = _mm_set1_epi64x(NEG_INF);
    __m128i vNegInf0 = _mm_srli_si128(vNegInf, 8); /* shift in a 0 */
    __m128i vOpen = _mm_set1_epi64x(open);
    __m128i vGap  = _mm_set1_epi64x(gap);
    __m128i vZero = _mm_set1_epi64x(0);
    __m128i vOne = _mm_set1_epi64x(1);
    __m128i vN = _mm_set1_epi64x(N);
    __m128i vNegOne = _mm_set1_epi64x(-1);
    __m128i vI = _mm_set_epi64x(0,1);
    __m128i vJreset = _mm_set_epi64x(0,-1);
    __m128i vMax = vNegInf;
    __m128i vEndI = vNegInf;
    __m128i vEndJ = vNegInf;
    __m128i vILimit = _mm_set1_epi64x(s1Len);
    __m128i vJLimit = _mm_set1_epi64x(s2Len);
    

    /* convert _s1 from char to int in range 0-23 */
    for (i=0; i<s1Len; ++i) {
        s1[i] = matrix->mapper[(unsigned char)_s1[i]];
    }
    /* pad back of s1 with dummy values */
    for (i=s1Len; i<s1Len_PAD; ++i) {
        s1[i] = 0; /* point to first matrix row because we don't care */
    }

    /* convert _s2 from char to int in range 0-23 */
    for (j=0; j<s2Len; ++j) {
        s2[j] = matrix->mapper[(unsigned char)_s2[j]];
    }
    /* pad front of s2 with dummy values */
    for (j=-PAD; j<0; ++j) {
        s2[j] = 0; /* point to first matrix row because we don't care */
    }
    /* pad back of s2 with dummy values */
    for (j=s2Len; j<s2Len_PAD; ++j) {
        s2[j] = 0; /* point to first matrix row because we don't care */
    }

    /* set initial values for stored row */
    for (j=0; j<s2Len; ++j) {
        tbl_pr[j] = 0;
        del_pr[j] = NEG_INF;
    }
    /* pad front of stored row values */
    for (j=-PAD; j<0; ++j) {
        tbl_pr[j] = NEG_INF;
        del_pr[j] = NEG_INF;
    }
    /* pad back of stored row values */
    for (j=s2Len; j<s2Len+PAD; ++j) {
        tbl_pr[j] = NEG_INF;
        del_pr[j] = NEG_INF;
    }

    /* iterate over query sequence */
    for (i=0; i<s1Len; i+=N) {
        __m128i vNscore = vNegInf0;
        __m128i vWscore = vNegInf0;
        __m128i vIns = vNegInf;
        __m128i vDel = vNegInf;
        __m128i vJ = vJreset;
        const int * const restrict matrow0 = &matrix->matrix[matrix->size*s1[i+0]];
        const int * const restrict matrow1 = &matrix->matrix[matrix->size*s1[i+1]];
        __m128i vIltLimit = _mm_cmplt_epi64_rpl(vI, vILimit);
        /* iterate over database sequence */
        for (j=0; j<s2Len+PAD; ++j) {
            __m128i vMat;
            __m128i vNWscore = vNscore;
            vNscore = _mm_srli_si128(vWscore, 8);
            vNscore = _mm_insert_epi64(vNscore, tbl_pr[j], 1);
            vDel = _mm_srli_si128(vDel, 8);
            vDel = _mm_insert_epi64(vDel, del_pr[j], 1);
            vDel = _mm_max_epi64_rpl(
                    _mm_sub_epi64(vNscore, vOpen),
                    _mm_sub_epi64(vDel, vGap));
            vIns = _mm_max_epi64_rpl(
                    _mm_sub_epi64(vWscore, vOpen),
                    _mm_sub_epi64(vIns, vGap));
            vMat = _mm_set_epi64x(
                    matrow0[s2[j-0]],
                    matrow1[s2[j-1]]
                    );
            vNWscore = _mm_add_epi64(vNWscore, vMat);
            vWscore = _mm_max_epi64_rpl(vNWscore, vIns);
            vWscore = _mm_max_epi64_rpl(vWscore, vDel);
            vWscore = _mm_max_epi64_rpl(vWscore, vZero);
            /* as minor diagonal vector passes across the j=-1 boundary,
             * assign the appropriate boundary conditions */
            {
                __m128i cond = _mm_cmpeq_epi64(vJ,vNegOne);
                vWscore = _mm_andnot_si128(cond, vWscore);
                vDel = _mm_blendv_epi8(vDel, vNegInf, cond);
                vIns = _mm_blendv_epi8(vIns, vNegInf, cond);
            }
            
            tbl_pr[j-1] = (int64_t)_mm_extract_epi64(vWscore,0);
            del_pr[j-1] = (int64_t)_mm_extract_epi64(vDel,0);
            /* as minor diagonal vector passes across table, extract
             * max values within the i,j bounds */
            {
                __m128i cond_valid_J = _mm_and_si128(
                        _mm_cmpgt_epi64_rpl(vJ, vNegOne),
                        _mm_cmplt_epi64_rpl(vJ, vJLimit));
                __m128i cond_valid_IJ = _mm_and_si128(cond_valid_J, vIltLimit);
                __m128i cond_eq = _mm_cmpeq_epi64(vWscore, vMax);
                __m128i cond_max = _mm_cmpgt_epi64_rpl(vWscore, vMax);
                __m128i cond_all = _mm_and_si128(cond_max, cond_valid_IJ);
                __m128i cond_Jlt = _mm_cmplt_epi64_rpl(vJ, vEndJ);
                vMax = _mm_blendv_epi8(vMax, vWscore, cond_all);
                vEndI = _mm_blendv_epi8(vEndI, vI, cond_all);
                vEndJ = _mm_blendv_epi8(vEndJ, vJ, cond_all);
                cond_all = _mm_and_si128(cond_Jlt, cond_eq);
                cond_all = _mm_and_si128(cond_all, cond_valid_IJ);
                vEndI = _mm_blendv_epi8(vEndI, vI, cond_all);
                vEndJ = _mm_blendv_epi8(vEndJ, vJ, cond_all);
            }
            vJ = _mm_add_epi64(vJ, vOne);
        }
        vI = _mm_add_epi64(vI, vN);
    }

    /* alignment ending position */
    {
        int64_t *t = (int64_t*)&vMax;
        int64_t *i = (int64_t*)&vEndI;
        int64_t *j = (int64_t*)&vEndJ;
        int32_t k;
        for (k=0; k<N; ++k, ++t, ++i, ++j) {
            if (*t > score) {
                score = *t;
                end_query = *i;
                end_ref = *j;
            }
            else if (*t == score) {
                if (*j < end_ref) {
                    end_query = *i;
                    end_ref = *j;
                }
                else if (*j == end_ref && *i < end_query) {
                    end_query = *i;
                    end_ref = *j;
                }
            }
        }
    }

    

    result->score = score;
    result->end_query = end_query;
    result->end_ref = end_ref;

    parasail_free(_del_pr);
    parasail_free(_tbl_pr);
    parasail_free(s2B);
    parasail_free(s1);

    return result;
}


