/**
 * @file
 *
 * @author jeff.daily@pnnl.gov
 *
 * Copyright (c) 2015 Battelle Memorial Institute.
 */
#include "config.h"

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

#define _mm256_cmplt_epi64_rpl(a,b) _mm256_cmpgt_epi64(b,a)

#define _mm256_srli_si256_rpl(a,imm) _mm256_or_si256(_mm256_slli_si256(_mm256_permute2x128_si256(a, a, _MM_SHUFFLE(3,0,0,1)), 16-imm), _mm256_srli_si256(a, imm))


#ifdef PARASAIL_TABLE
static inline void arr_store_si256(
        int *array,
        __m256i vWscore,
        int32_t i,
        int32_t s1Len,
        int32_t j,
        int32_t s2Len)
{
    if (0 <= i+0 && i+0 < s1Len && 0 <= j-0 && j-0 < s2Len) {
        array[(i+0)*s2Len + (j-0)] = (int64_t)_mm256_extract_epi64_rpl(vWscore, 3);
    }
    if (0 <= i+1 && i+1 < s1Len && 0 <= j-1 && j-1 < s2Len) {
        array[(i+1)*s2Len + (j-1)] = (int64_t)_mm256_extract_epi64_rpl(vWscore, 2);
    }
    if (0 <= i+2 && i+2 < s1Len && 0 <= j-2 && j-2 < s2Len) {
        array[(i+2)*s2Len + (j-2)] = (int64_t)_mm256_extract_epi64_rpl(vWscore, 1);
    }
    if (0 <= i+3 && i+3 < s1Len && 0 <= j-3 && j-3 < s2Len) {
        array[(i+3)*s2Len + (j-3)] = (int64_t)_mm256_extract_epi64_rpl(vWscore, 0);
    }
}
#endif

#ifdef PARASAIL_ROWCOL
static inline void arr_store_rowcol(
        int *row,
        int *col,
        __m256i vWscore,
        int32_t i,
        int32_t s1Len,
        int32_t j,
        int32_t s2Len)
{
    if (i+0 == s1Len-1 && 0 <= j-0 && j-0 < s2Len) {
        row[j-0] = (int64_t)_mm256_extract_epi64_rpl(vWscore, 3);
    }
    if (j-0 == s2Len-1 && 0 <= i+0 && i+0 < s1Len) {
        col[(i+0)] = (int64_t)_mm256_extract_epi64_rpl(vWscore, 3);
    }
    if (i+1 == s1Len-1 && 0 <= j-1 && j-1 < s2Len) {
        row[j-1] = (int64_t)_mm256_extract_epi64_rpl(vWscore, 2);
    }
    if (j-1 == s2Len-1 && 0 <= i+1 && i+1 < s1Len) {
        col[(i+1)] = (int64_t)_mm256_extract_epi64_rpl(vWscore, 2);
    }
    if (i+2 == s1Len-1 && 0 <= j-2 && j-2 < s2Len) {
        row[j-2] = (int64_t)_mm256_extract_epi64_rpl(vWscore, 1);
    }
    if (j-2 == s2Len-1 && 0 <= i+2 && i+2 < s1Len) {
        col[(i+2)] = (int64_t)_mm256_extract_epi64_rpl(vWscore, 1);
    }
    if (i+3 == s1Len-1 && 0 <= j-3 && j-3 < s2Len) {
        row[j-3] = (int64_t)_mm256_extract_epi64_rpl(vWscore, 0);
    }
    if (j-3 == s2Len-1 && 0 <= i+3 && i+3 < s1Len) {
        col[(i+3)] = (int64_t)_mm256_extract_epi64_rpl(vWscore, 0);
    }
}
#endif

#ifdef PARASAIL_TABLE
#define FNAME parasail_sw_stats_table_diag_avx2_256_64
#else
#ifdef PARASAIL_ROWCOL
#define FNAME parasail_sw_stats_rowcol_diag_avx2_256_64
#else
#define FNAME parasail_sw_stats_diag_avx2_256_64
#endif
#endif

parasail_result_t* FNAME(
        const char * const restrict _s1, const int s1Len,
        const char * const restrict _s2, const int s2Len,
        const int open, const int gap, const parasail_matrix_t *matrix)
{
    const int32_t N = 4; /* number of values in vector */
    const int32_t PAD = N-1;
    const int32_t PAD2 = PAD*2;
    const int32_t s1Len_PAD = s1Len+PAD;
    const int32_t s2Len_PAD = s2Len+PAD;
    int64_t * const restrict s1      = parasail_memalign_int64_t(32, s1Len+PAD);
    int64_t * const restrict s2B     = parasail_memalign_int64_t(32, s2Len+PAD2);
    int64_t * const restrict _tbl_pr = parasail_memalign_int64_t(32, s2Len+PAD2);
    int64_t * const restrict _del_pr = parasail_memalign_int64_t(32, s2Len+PAD2);
    int64_t * const restrict _mch_pr = parasail_memalign_int64_t(32, s2Len+PAD2);
    int64_t * const restrict _sim_pr = parasail_memalign_int64_t(32, s2Len+PAD2);
    int64_t * const restrict _len_pr = parasail_memalign_int64_t(32, s2Len+PAD2);
    int64_t * const restrict s2 = s2B+PAD; /* will allow later for negative indices */
    int64_t * const restrict tbl_pr = _tbl_pr+PAD;
    int64_t * const restrict del_pr = _del_pr+PAD;
    int64_t * const restrict mch_pr = _mch_pr+PAD;
    int64_t * const restrict sim_pr = _sim_pr+PAD;
    int64_t * const restrict len_pr = _len_pr+PAD;
#ifdef PARASAIL_TABLE
    parasail_result_t *result = parasail_result_new_table3(s1Len, s2Len);
#else
#ifdef PARASAIL_ROWCOL
    parasail_result_t *result = parasail_result_new_rowcol3(s1Len, s2Len);
#else
    parasail_result_t *result = parasail_result_new();
#endif
#endif
    int32_t i = 0;
    int32_t j = 0;
    int64_t end_query = 0;
    int64_t end_ref = 0;
    int64_t score = NEG_INF;
    int64_t matches = NEG_INF;
    int64_t similar = NEG_INF;
    int64_t length = NEG_INF;
    
    __m256i vNegInf = _mm256_set1_epi64x(NEG_INF);
    __m256i vNegInf0 = _mm256_srli_si256_rpl(vNegInf, 8); /* shift in a 0 */
    __m256i vOpen = _mm256_set1_epi64x(open);
    __m256i vGap  = _mm256_set1_epi64x(gap);
    __m256i vZero = _mm256_set1_epi64x(0);
    __m256i vOne = _mm256_set1_epi64x(1);
    __m256i vN = _mm256_set1_epi64x(N);
    __m256i vNegOne = _mm256_set1_epi64x(-1);
    __m256i vI = _mm256_set_epi64x(0,1,2,3);
    __m256i vJreset = _mm256_set_epi64x(0,-1,-2,-3);
    __m256i vMaxScore = vNegInf;
    __m256i vMaxMatch = vNegInf;
    __m256i vMaxSimilar = vNegInf;
    __m256i vMaxLength = vNegInf;
    __m256i vEndI = vNegInf;
    __m256i vEndJ = vNegInf;
    __m256i vILimit = _mm256_set1_epi64x(s1Len);
    __m256i vJLimit = _mm256_set1_epi64x(s2Len);

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
        mch_pr[j] = 0;
        sim_pr[j] = 0;
        len_pr[j] = 0;
    }
    /* pad front of stored row values */
    for (j=-PAD; j<0; ++j) {
        tbl_pr[j] = NEG_INF;
        del_pr[j] = NEG_INF;
        mch_pr[j] = 0;
        sim_pr[j] = 0;
        len_pr[j] = 0;
    }
    /* pad back of stored row values */
    for (j=s2Len; j<s2Len+PAD; ++j) {
        tbl_pr[j] = NEG_INF;
        del_pr[j] = NEG_INF;
        mch_pr[j] = 0;
        sim_pr[j] = 0;
        len_pr[j] = 0;
    }
    tbl_pr[-1] = 0; /* upper left corner */

    /* iterate over query sequence */
    for (i=0; i<s1Len; i+=N) {
        __m256i vNscore = vNegInf0;
        __m256i vNmatch = vZero;
        __m256i vNsimilar = vZero;
        __m256i vNlength = vZero;
        __m256i vWscore = vNegInf0;
        __m256i vWmatch = vZero;
        __m256i vWsimilar = vZero;
        __m256i vWlength = vZero;
        __m256i vIns = vNegInf;
        __m256i vDel = vNegInf;
        __m256i vJ = vJreset;
        __m256i vs1 = _mm256_set_epi64x(
                s1[i+0],
                s1[i+1],
                s1[i+2],
                s1[i+3]);
        __m256i vs2 = vNegInf;
        const int * const restrict matrow0 = &matrix->matrix[matrix->size*s1[i+0]];
        const int * const restrict matrow1 = &matrix->matrix[matrix->size*s1[i+1]];
        const int * const restrict matrow2 = &matrix->matrix[matrix->size*s1[i+2]];
        const int * const restrict matrow3 = &matrix->matrix[matrix->size*s1[i+3]];
        __m256i vIltLimit = _mm256_cmplt_epi64_rpl(vI, vILimit);
        /* iterate over database sequence */
        for (j=0; j<s2Len+PAD; ++j) {
            __m256i vMat;
            __m256i vNWscore = vNscore;
            __m256i vNWmatch = vNmatch;
            __m256i vNWsimilar = vNsimilar;
            __m256i vNWlength = vNlength;
            vNscore = _mm256_srli_si256_rpl(vWscore, 8);
            vNscore = _mm256_insert_epi64_rpl(vNscore, tbl_pr[j], 3);
            vNmatch = _mm256_srli_si256_rpl(vWmatch, 8);
            vNmatch = _mm256_insert_epi64_rpl(vNmatch, mch_pr[j], 3);
            vNsimilar = _mm256_srli_si256_rpl(vWsimilar, 8);
            vNsimilar = _mm256_insert_epi64_rpl(vNsimilar, sim_pr[j], 3);
            vNlength = _mm256_srli_si256_rpl(vWlength, 8);
            vNlength = _mm256_insert_epi64_rpl(vNlength, len_pr[j], 3);
            vDel = _mm256_srli_si256_rpl(vDel, 8);
            vDel = _mm256_insert_epi64_rpl(vDel, del_pr[j], 3);
            vDel = _mm256_max_epi64_rpl(
                    _mm256_sub_epi64(vNscore, vOpen),
                    _mm256_sub_epi64(vDel, vGap));
            vIns = _mm256_max_epi64_rpl(
                    _mm256_sub_epi64(vWscore, vOpen),
                    _mm256_sub_epi64(vIns, vGap));
            vs2 = _mm256_srli_si256_rpl(vs2, 8);
            vs2 = _mm256_insert_epi64_rpl(vs2, s2[j], 3);
            vMat = _mm256_set_epi64x(
                    matrow0[s2[j-0]],
                    matrow1[s2[j-1]],
                    matrow2[s2[j-2]],
                    matrow3[s2[j-3]]
                    );
            vNWscore = _mm256_add_epi64(vNWscore, vMat);
            vWscore = _mm256_max_epi64_rpl(vNWscore, vIns);
            vWscore = _mm256_max_epi64_rpl(vWscore, vDel);
            vWscore = _mm256_max_epi64_rpl(vWscore, vZero);
            /* conditional block */
            {
                __m256i case1not;
                __m256i case2not;
                __m256i case2;
                __m256i case3;
                __m256i vCmatch;
                __m256i vCsimilar;
                __m256i vClength;
                case1not = _mm256_or_si256(
                        _mm256_cmplt_epi64_rpl(vNWscore,vDel),
                        _mm256_cmplt_epi64_rpl(vNWscore,vIns));
                case2not = _mm256_cmplt_epi64_rpl(vDel,vIns);
                case2 = _mm256_andnot_si256(case2not,case1not);
                case3 = _mm256_and_si256(case1not,case2not);
                vCmatch = _mm256_andnot_si256(case1not,
                        _mm256_add_epi64(vNWmatch, _mm256_and_si256(
                                _mm256_cmpeq_epi64(vs1,vs2),vOne)));
                vCmatch = _mm256_or_si256(vCmatch, _mm256_and_si256(case2, vNmatch));
                vCmatch = _mm256_or_si256(vCmatch, _mm256_and_si256(case3, vWmatch));
                vCsimilar = _mm256_andnot_si256(case1not,
                        _mm256_add_epi64(vNWsimilar, _mm256_and_si256(
                                _mm256_cmpgt_epi64(vMat,vZero),vOne)));
                vCsimilar = _mm256_or_si256(vCsimilar, _mm256_and_si256(case2, vNsimilar));
                vCsimilar = _mm256_or_si256(vCsimilar, _mm256_and_si256(case3, vWsimilar));
                vClength= _mm256_andnot_si256(case1not,
                        _mm256_add_epi64(vNWlength, vOne));
                vClength= _mm256_or_si256(vClength,_mm256_and_si256(case2,
                            _mm256_add_epi64(vNlength, vOne)));
                vClength= _mm256_or_si256(vClength,_mm256_and_si256(case3,
                            _mm256_add_epi64(vWlength, vOne)));
                vWmatch = vCmatch;
                vWsimilar = vCsimilar;
                vWlength = vClength;
            }
            {
                __m256i cond_zero = _mm256_cmpeq_epi64(vWscore, vZero);
                vWmatch = _mm256_andnot_si256(cond_zero, vWmatch);
                vWsimilar = _mm256_andnot_si256(cond_zero, vWsimilar);
                vWlength = _mm256_andnot_si256(cond_zero, vWlength);
            }

            /* as minor diagonal vector passes across the j=-1 boundary,
             * assign the appropriate boundary conditions */
            {
                __m256i cond = _mm256_cmpeq_epi64(vJ,vNegOne);
                vWscore = _mm256_andnot_si256(cond, vWscore);
                vWmatch = _mm256_andnot_si256(cond, vWmatch);
                vWsimilar = _mm256_andnot_si256(cond, vWsimilar);
                vWlength = _mm256_andnot_si256(cond, vWlength);
                vDel = _mm256_blendv_epi8(vDel, vNegInf, cond);
                vIns = _mm256_blendv_epi8(vIns, vNegInf, cond);
            }

            
#ifdef PARASAIL_TABLE
            arr_store_si256(result->score_table, vWscore, i, s1Len, j, s2Len);
            arr_store_si256(result->matches_table, vWmatch, i, s1Len, j, s2Len);
            arr_store_si256(result->similar_table, vWsimilar, i, s1Len, j, s2Len);
            arr_store_si256(result->length_table, vWlength, i, s1Len, j, s2Len);
#endif
#ifdef PARASAIL_ROWCOL
            arr_store_rowcol(result->score_row, result->score_col, vWscore, i, s1Len, j, s2Len);
            arr_store_rowcol(result->matches_row, result->matches_col, vWmatch, i, s1Len, j, s2Len);
            arr_store_rowcol(result->similar_row, result->similar_col, vWsimilar, i, s1Len, j, s2Len);
            arr_store_rowcol(result->length_row, result->length_col, vWlength, i, s1Len, j, s2Len);
#endif
            tbl_pr[j-3] = (int64_t)_mm256_extract_epi64_rpl(vWscore,0);
            mch_pr[j-3] = (int64_t)_mm256_extract_epi64_rpl(vWmatch,0);
            sim_pr[j-3] = (int64_t)_mm256_extract_epi64_rpl(vWsimilar,0);
            len_pr[j-3] = (int64_t)_mm256_extract_epi64_rpl(vWlength,0);
            del_pr[j-3] = (int64_t)_mm256_extract_epi64_rpl(vDel,0);
            /* as minor diagonal vector passes across table, extract
             * max values within the i,j bounds */
            {
                __m256i cond_valid_J = _mm256_and_si256(
                        _mm256_cmpgt_epi64(vJ, vNegOne),
                        _mm256_cmplt_epi64_rpl(vJ, vJLimit));
                __m256i cond_valid_IJ = _mm256_and_si256(cond_valid_J, vIltLimit);
                __m256i cond_eq = _mm256_cmpeq_epi64(vWscore, vMaxScore);
                __m256i cond_max = _mm256_cmpgt_epi64(vWscore, vMaxScore);
                __m256i cond_all = _mm256_and_si256(cond_max, cond_valid_IJ);
                __m256i cond_Jlt = _mm256_cmplt_epi64_rpl(vJ, vEndJ);
                vMaxScore = _mm256_blendv_epi8(vMaxScore, vWscore, cond_all);
                vMaxMatch = _mm256_blendv_epi8(vMaxMatch, vWmatch, cond_all);
                vMaxSimilar = _mm256_blendv_epi8(vMaxSimilar, vWsimilar, cond_all);
                vMaxLength = _mm256_blendv_epi8(vMaxLength, vWlength, cond_all);
                vEndI = _mm256_blendv_epi8(vEndI, vI, cond_all);
                vEndJ = _mm256_blendv_epi8(vEndJ, vJ, cond_all);
                cond_all = _mm256_and_si256(cond_Jlt, cond_eq);
                cond_all = _mm256_and_si256(cond_all, cond_valid_IJ);
                vMaxMatch = _mm256_blendv_epi8(vMaxMatch, vWmatch, cond_all);
                vMaxSimilar = _mm256_blendv_epi8(vMaxSimilar, vWsimilar, cond_all);
                vMaxLength = _mm256_blendv_epi8(vMaxLength, vWlength, cond_all);
                vEndI = _mm256_blendv_epi8(vEndI, vI, cond_all);
                vEndJ = _mm256_blendv_epi8(vEndJ, vJ, cond_all);
            }
            vJ = _mm256_add_epi64(vJ, vOne);
        }
        vI = _mm256_add_epi64(vI, vN);
    }

    /* alignment ending position */
    {
        int64_t *t = (int64_t*)&vMaxScore;
        int64_t *m = (int64_t*)&vMaxMatch;
        int64_t *s = (int64_t*)&vMaxSimilar;
        int64_t *l = (int64_t*)&vMaxLength;
        int64_t *i = (int64_t*)&vEndI;
        int64_t *j = (int64_t*)&vEndJ;
        int32_t k;
        for (k=0; k<N; ++k, ++t, ++m, ++s, ++l, ++i, ++j) {
            if (*t > score) {
                score = *t;
                matches = *m;
                similar = *s;
                length = *l;
                end_query = *i;
                end_ref = *j;
            }
            else if (*t == score) {
                if (*j < end_ref) {
                    matches = *m;
                    similar = *s;
                    length = *l;
                    end_query = *i;
                    end_ref = *j;
                }
                else if (*j == end_ref && *i < end_query) {
                    matches = *m;
                    similar = *s;
                    length = *l;
                    end_query = *i;
                    end_ref = *j;
                }
            }
        }
    }

    

    result->score = score;
    result->matches = matches;
    result->similar = similar;
    result->length = length;
    result->end_query = end_query;
    result->end_ref = end_ref;
    result->flag = PARASAIL_FLAG_SW | PARASAIL_FLAG_DIAG
        | PARASAIL_FLAG_STATS
        | PARASAIL_FLAG_BITS_64 | PARASAIL_FLAG_LANES_4;
#ifdef PARASAIL_TABLE
    result->flag |= PARASAIL_FLAG_TABLE;
#endif
#ifdef PARASAIL_ROWCOL
    result->flag |= PARASAIL_FLAG_ROWCOL;
#endif

    parasail_free(_len_pr);
    parasail_free(_sim_pr);
    parasail_free(_mch_pr);
    parasail_free(_del_pr);
    parasail_free(_tbl_pr);
    parasail_free(s2B);
    parasail_free(s1);

    return result;
}


