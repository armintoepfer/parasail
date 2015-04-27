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

#include <stdlib.h>

#include <immintrin.h>

#include "parasail.h"
#include "parasail/memory.h"
#include "parasail/internal_avx.h"
#include "parasail/matrices/blosum_map.h"

#define NEG_INF INT8_MIN

#define _mm256_cmplt_epi8_rpl(a,b) _mm256_cmpgt_epi8(b,a)

#define _mm256_srli_si256_rpl(a,imm) _mm256_or_si256(_mm256_slli_si256(_mm256_permute2x128_si256(a, a, _MM_SHUFFLE(3,0,0,1)), 16-imm), _mm256_srli_si256(a, imm))

static inline __m256i _mm256_packs_epi16_rpl(__m256i a, __m256i b) {
    return _mm256_permute4x64_epi64(
            _mm256_packs_epi16(a, b),
            _MM_SHUFFLE(3,1,2,0));
}

#define _mm256_cmplt_epi16_rpl(a,b) _mm256_cmpgt_epi16(b,a)

#define _mm256_slli_si256_rpl(a,imm) _mm256_alignr_epi8(a, _mm256_permute2x128_si256(a, a, _MM_SHUFFLE(0,0,3,0)), 16-imm)


#ifdef PARASAIL_TABLE
static inline void arr_store_si128(
        int *array,
        __m256i vWscore,
        int32_t i,
        int32_t s1Len,
        int32_t j,
        int32_t s2Len)
{
    if (0 <= i+0 && i+0 < s1Len && 0 <= j-0 && j-0 < s2Len) {
        array[(i+0)*s2Len + (j-0)] = (int8_t)_mm256_extract_epi8(vWscore, 31);
    }
    if (0 <= i+1 && i+1 < s1Len && 0 <= j-1 && j-1 < s2Len) {
        array[(i+1)*s2Len + (j-1)] = (int8_t)_mm256_extract_epi8(vWscore, 30);
    }
    if (0 <= i+2 && i+2 < s1Len && 0 <= j-2 && j-2 < s2Len) {
        array[(i+2)*s2Len + (j-2)] = (int8_t)_mm256_extract_epi8(vWscore, 29);
    }
    if (0 <= i+3 && i+3 < s1Len && 0 <= j-3 && j-3 < s2Len) {
        array[(i+3)*s2Len + (j-3)] = (int8_t)_mm256_extract_epi8(vWscore, 28);
    }
    if (0 <= i+4 && i+4 < s1Len && 0 <= j-4 && j-4 < s2Len) {
        array[(i+4)*s2Len + (j-4)] = (int8_t)_mm256_extract_epi8(vWscore, 27);
    }
    if (0 <= i+5 && i+5 < s1Len && 0 <= j-5 && j-5 < s2Len) {
        array[(i+5)*s2Len + (j-5)] = (int8_t)_mm256_extract_epi8(vWscore, 26);
    }
    if (0 <= i+6 && i+6 < s1Len && 0 <= j-6 && j-6 < s2Len) {
        array[(i+6)*s2Len + (j-6)] = (int8_t)_mm256_extract_epi8(vWscore, 25);
    }
    if (0 <= i+7 && i+7 < s1Len && 0 <= j-7 && j-7 < s2Len) {
        array[(i+7)*s2Len + (j-7)] = (int8_t)_mm256_extract_epi8(vWscore, 24);
    }
    if (0 <= i+8 && i+8 < s1Len && 0 <= j-8 && j-8 < s2Len) {
        array[(i+8)*s2Len + (j-8)] = (int8_t)_mm256_extract_epi8(vWscore, 23);
    }
    if (0 <= i+9 && i+9 < s1Len && 0 <= j-9 && j-9 < s2Len) {
        array[(i+9)*s2Len + (j-9)] = (int8_t)_mm256_extract_epi8(vWscore, 22);
    }
    if (0 <= i+10 && i+10 < s1Len && 0 <= j-10 && j-10 < s2Len) {
        array[(i+10)*s2Len + (j-10)] = (int8_t)_mm256_extract_epi8(vWscore, 21);
    }
    if (0 <= i+11 && i+11 < s1Len && 0 <= j-11 && j-11 < s2Len) {
        array[(i+11)*s2Len + (j-11)] = (int8_t)_mm256_extract_epi8(vWscore, 20);
    }
    if (0 <= i+12 && i+12 < s1Len && 0 <= j-12 && j-12 < s2Len) {
        array[(i+12)*s2Len + (j-12)] = (int8_t)_mm256_extract_epi8(vWscore, 19);
    }
    if (0 <= i+13 && i+13 < s1Len && 0 <= j-13 && j-13 < s2Len) {
        array[(i+13)*s2Len + (j-13)] = (int8_t)_mm256_extract_epi8(vWscore, 18);
    }
    if (0 <= i+14 && i+14 < s1Len && 0 <= j-14 && j-14 < s2Len) {
        array[(i+14)*s2Len + (j-14)] = (int8_t)_mm256_extract_epi8(vWscore, 17);
    }
    if (0 <= i+15 && i+15 < s1Len && 0 <= j-15 && j-15 < s2Len) {
        array[(i+15)*s2Len + (j-15)] = (int8_t)_mm256_extract_epi8(vWscore, 16);
    }
    if (0 <= i+16 && i+16 < s1Len && 0 <= j-16 && j-16 < s2Len) {
        array[(i+16)*s2Len + (j-16)] = (int8_t)_mm256_extract_epi8(vWscore, 15);
    }
    if (0 <= i+17 && i+17 < s1Len && 0 <= j-17 && j-17 < s2Len) {
        array[(i+17)*s2Len + (j-17)] = (int8_t)_mm256_extract_epi8(vWscore, 14);
    }
    if (0 <= i+18 && i+18 < s1Len && 0 <= j-18 && j-18 < s2Len) {
        array[(i+18)*s2Len + (j-18)] = (int8_t)_mm256_extract_epi8(vWscore, 13);
    }
    if (0 <= i+19 && i+19 < s1Len && 0 <= j-19 && j-19 < s2Len) {
        array[(i+19)*s2Len + (j-19)] = (int8_t)_mm256_extract_epi8(vWscore, 12);
    }
    if (0 <= i+20 && i+20 < s1Len && 0 <= j-20 && j-20 < s2Len) {
        array[(i+20)*s2Len + (j-20)] = (int8_t)_mm256_extract_epi8(vWscore, 11);
    }
    if (0 <= i+21 && i+21 < s1Len && 0 <= j-21 && j-21 < s2Len) {
        array[(i+21)*s2Len + (j-21)] = (int8_t)_mm256_extract_epi8(vWscore, 10);
    }
    if (0 <= i+22 && i+22 < s1Len && 0 <= j-22 && j-22 < s2Len) {
        array[(i+22)*s2Len + (j-22)] = (int8_t)_mm256_extract_epi8(vWscore, 9);
    }
    if (0 <= i+23 && i+23 < s1Len && 0 <= j-23 && j-23 < s2Len) {
        array[(i+23)*s2Len + (j-23)] = (int8_t)_mm256_extract_epi8(vWscore, 8);
    }
    if (0 <= i+24 && i+24 < s1Len && 0 <= j-24 && j-24 < s2Len) {
        array[(i+24)*s2Len + (j-24)] = (int8_t)_mm256_extract_epi8(vWscore, 7);
    }
    if (0 <= i+25 && i+25 < s1Len && 0 <= j-25 && j-25 < s2Len) {
        array[(i+25)*s2Len + (j-25)] = (int8_t)_mm256_extract_epi8(vWscore, 6);
    }
    if (0 <= i+26 && i+26 < s1Len && 0 <= j-26 && j-26 < s2Len) {
        array[(i+26)*s2Len + (j-26)] = (int8_t)_mm256_extract_epi8(vWscore, 5);
    }
    if (0 <= i+27 && i+27 < s1Len && 0 <= j-27 && j-27 < s2Len) {
        array[(i+27)*s2Len + (j-27)] = (int8_t)_mm256_extract_epi8(vWscore, 4);
    }
    if (0 <= i+28 && i+28 < s1Len && 0 <= j-28 && j-28 < s2Len) {
        array[(i+28)*s2Len + (j-28)] = (int8_t)_mm256_extract_epi8(vWscore, 3);
    }
    if (0 <= i+29 && i+29 < s1Len && 0 <= j-29 && j-29 < s2Len) {
        array[(i+29)*s2Len + (j-29)] = (int8_t)_mm256_extract_epi8(vWscore, 2);
    }
    if (0 <= i+30 && i+30 < s1Len && 0 <= j-30 && j-30 < s2Len) {
        array[(i+30)*s2Len + (j-30)] = (int8_t)_mm256_extract_epi8(vWscore, 1);
    }
    if (0 <= i+31 && i+31 < s1Len && 0 <= j-31 && j-31 < s2Len) {
        array[(i+31)*s2Len + (j-31)] = (int8_t)_mm256_extract_epi8(vWscore, 0);
    }
}
#endif


#ifdef PARASAIL_TABLE
#define FNAME sg_table_diag_avx2_256_8
#else
#define FNAME sg_diag_avx2_256_8
#endif

parasail_result_t* FNAME(
        const char * const restrict _s1, const int s1Len,
        const char * const restrict _s2, const int s2Len,
        const int open, const int gap, const parasail_matrix_t *matrix)
{
    const int32_t N = 32; /* number of values in vector */
    const int32_t PAD = N-1;
    const int32_t PAD2 = PAD*2;
    int8_t * const restrict s1 = parasail_memalign_int8_t(32, s1Len+PAD);
    int8_t * const restrict s2B= parasail_memalign_int8_t(32, s2Len+PAD2);
    int8_t * const restrict _tbl_pr = parasail_memalign_int8_t(32, s2Len+PAD2);
    int8_t * const restrict _del_pr = parasail_memalign_int8_t(32, s2Len+PAD2);
    int8_t * const restrict s2 = s2B+PAD; /* will allow later for negative indices */
    int8_t * const restrict tbl_pr = _tbl_pr+PAD;
    int8_t * const restrict del_pr = _del_pr+PAD;
#ifdef PARASAIL_TABLE
    parasail_result_t *result = parasail_result_new_table1(s1Len, s2Len);
#else
    parasail_result_t *result = parasail_result_new();
#endif
    int32_t i = 0;
    int32_t j = 0;
    int8_t score = NEG_INF;
    __m256i vNegInf = _mm256_set1_epi8(NEG_INF);
    __m256i vNegInf0 = _mm256_srli_si256_rpl(vNegInf, 1); /* shift in a 0 */
    __m256i vOpen = _mm256_set1_epi8(open);
    __m256i vGap  = _mm256_set1_epi8(gap);
    __m256i vOne16 = _mm256_set1_epi16(1);
    __m256i vN16 = _mm256_set1_epi16(N);
    __m256i vNegOne16 = _mm256_set1_epi16(-1);
    __m256i vILo16 = _mm256_set_epi16(16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31);
    __m256i vIHi16 = _mm256_set_epi16(0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15);
    __m256i vJresetLo16 = _mm256_set_epi16(-16,-17,-18,-19,-20,-21,-22,-23,-24,-25,-26,-27,-28,-29,-30,-31);
    __m256i vJresetHi16 = _mm256_set_epi16(0,-1,-2,-3,-4,-5,-6,-7,-8,-9,-10,-11,-12,-13,-14,-15);
    __m256i vMaxScore = vNegInf;
    __m256i vILimit16 = _mm256_set1_epi16(s1Len);
    __m256i vILimit116 = _mm256_sub_epi16(vILimit16, vOne16);
    __m256i vJLimit16 = _mm256_set1_epi16(s2Len);
    __m256i vJLimit116 = _mm256_sub_epi16(vJLimit16, vOne16);
    __m256i vNegLimit = _mm256_set1_epi8(INT8_MIN);
    __m256i vPosLimit = _mm256_set1_epi8(INT8_MAX);
    __m256i vSaturationCheckMin = vPosLimit;
    __m256i vSaturationCheckMax = vNegLimit;

    /* convert _s1 from char to int in range 0-23 */
    for (i=0; i<s1Len; ++i) {
        s1[i] = matrix->mapper[(unsigned char)_s1[i]];
    }
    /* pad back of s1 with dummy values */
    for (i=s1Len; i<s1Len+PAD; ++i) {
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
    for (j=s2Len; j<s2Len+PAD; ++j) {
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
        __m256i vNscore = vNegInf0;
        __m256i vWscore = vNegInf0;
        __m256i vIns = vNegInf;
        __m256i vDel = vNegInf;
        __m256i vJLo16 = vJresetLo16;
        __m256i vJHi16 = vJresetHi16;
        const int8_t * const restrict matrow0 = &matrix->matrix[matrix->size*s1[i+0]];
        const int8_t * const restrict matrow1 = &matrix->matrix[matrix->size*s1[i+1]];
        const int8_t * const restrict matrow2 = &matrix->matrix[matrix->size*s1[i+2]];
        const int8_t * const restrict matrow3 = &matrix->matrix[matrix->size*s1[i+3]];
        const int8_t * const restrict matrow4 = &matrix->matrix[matrix->size*s1[i+4]];
        const int8_t * const restrict matrow5 = &matrix->matrix[matrix->size*s1[i+5]];
        const int8_t * const restrict matrow6 = &matrix->matrix[matrix->size*s1[i+6]];
        const int8_t * const restrict matrow7 = &matrix->matrix[matrix->size*s1[i+7]];
        const int8_t * const restrict matrow8 = &matrix->matrix[matrix->size*s1[i+8]];
        const int8_t * const restrict matrow9 = &matrix->matrix[matrix->size*s1[i+9]];
        const int8_t * const restrict matrow10 = &matrix->matrix[matrix->size*s1[i+10]];
        const int8_t * const restrict matrow11 = &matrix->matrix[matrix->size*s1[i+11]];
        const int8_t * const restrict matrow12 = &matrix->matrix[matrix->size*s1[i+12]];
        const int8_t * const restrict matrow13 = &matrix->matrix[matrix->size*s1[i+13]];
        const int8_t * const restrict matrow14 = &matrix->matrix[matrix->size*s1[i+14]];
        const int8_t * const restrict matrow15 = &matrix->matrix[matrix->size*s1[i+15]];
        const int8_t * const restrict matrow16 = &matrix->matrix[matrix->size*s1[i+16]];
        const int8_t * const restrict matrow17 = &matrix->matrix[matrix->size*s1[i+17]];
        const int8_t * const restrict matrow18 = &matrix->matrix[matrix->size*s1[i+18]];
        const int8_t * const restrict matrow19 = &matrix->matrix[matrix->size*s1[i+19]];
        const int8_t * const restrict matrow20 = &matrix->matrix[matrix->size*s1[i+20]];
        const int8_t * const restrict matrow21 = &matrix->matrix[matrix->size*s1[i+21]];
        const int8_t * const restrict matrow22 = &matrix->matrix[matrix->size*s1[i+22]];
        const int8_t * const restrict matrow23 = &matrix->matrix[matrix->size*s1[i+23]];
        const int8_t * const restrict matrow24 = &matrix->matrix[matrix->size*s1[i+24]];
        const int8_t * const restrict matrow25 = &matrix->matrix[matrix->size*s1[i+25]];
        const int8_t * const restrict matrow26 = &matrix->matrix[matrix->size*s1[i+26]];
        const int8_t * const restrict matrow27 = &matrix->matrix[matrix->size*s1[i+27]];
        const int8_t * const restrict matrow28 = &matrix->matrix[matrix->size*s1[i+28]];
        const int8_t * const restrict matrow29 = &matrix->matrix[matrix->size*s1[i+29]];
        const int8_t * const restrict matrow30 = &matrix->matrix[matrix->size*s1[i+30]];
        const int8_t * const restrict matrow31 = &matrix->matrix[matrix->size*s1[i+31]];
        __m256i vIltLimit = _mm256_packs_epi16_rpl(
                _mm256_cmplt_epi16_rpl(vILo16, vILimit16),
                _mm256_cmplt_epi16_rpl(vIHi16, vILimit16));
        __m256i vIeqLimit1 = _mm256_packs_epi16_rpl(
                _mm256_cmpeq_epi16(vILo16, vILimit116),
                _mm256_cmpeq_epi16(vIHi16, vILimit116));
        /* iterate over database sequence */
        for (j=0; j<s2Len+PAD; ++j) {
            __m256i vMat;
            __m256i vNWscore = vNscore;
            vNscore = _mm256_srli_si256_rpl(vWscore, 1);
            vNscore = _mm256_insert_epi8(vNscore, tbl_pr[j], 31);
            vDel = _mm256_srli_si256_rpl(vDel, 1);
            vDel = _mm256_insert_epi8(vDel, del_pr[j], 31);
            vDel = _mm256_max_epi8(
                    _mm256_subs_epi8(vNscore, vOpen),
                    _mm256_subs_epi8(vDel, vGap));
            vIns = _mm256_max_epi8(
                    _mm256_subs_epi8(vWscore, vOpen),
                    _mm256_subs_epi8(vIns, vGap));
            vMat = _mm256_set_epi8(
                    matrow0[s2[j-0]],
                    matrow1[s2[j-1]],
                    matrow2[s2[j-2]],
                    matrow3[s2[j-3]],
                    matrow4[s2[j-4]],
                    matrow5[s2[j-5]],
                    matrow6[s2[j-6]],
                    matrow7[s2[j-7]],
                    matrow8[s2[j-8]],
                    matrow9[s2[j-9]],
                    matrow10[s2[j-10]],
                    matrow11[s2[j-11]],
                    matrow12[s2[j-12]],
                    matrow13[s2[j-13]],
                    matrow14[s2[j-14]],
                    matrow15[s2[j-15]],
                    matrow16[s2[j-16]],
                    matrow17[s2[j-17]],
                    matrow18[s2[j-18]],
                    matrow19[s2[j-19]],
                    matrow20[s2[j-20]],
                    matrow21[s2[j-21]],
                    matrow22[s2[j-22]],
                    matrow23[s2[j-23]],
                    matrow24[s2[j-24]],
                    matrow25[s2[j-25]],
                    matrow26[s2[j-26]],
                    matrow27[s2[j-27]],
                    matrow28[s2[j-28]],
                    matrow29[s2[j-29]],
                    matrow30[s2[j-30]],
                    matrow31[s2[j-31]]
                    );
            vNWscore = _mm256_adds_epi8(vNWscore, vMat);
            vWscore = _mm256_max_epi8(vNWscore, vIns);
            vWscore = _mm256_max_epi8(vWscore, vDel);
            /* as minor diagonal vector passes across the j=-1 boundary,
             * assign the appropriate boundary conditions */
            {
                __m256i cond = _mm256_packs_epi16_rpl(
                        _mm256_cmpeq_epi16(vJLo16,vNegOne16),
                        _mm256_cmpeq_epi16(vJHi16,vNegOne16));
                vWscore = _mm256_andnot_si256(cond, vWscore);
                vDel = _mm256_blendv_epi8(vDel, vNegInf, cond);
                vIns = _mm256_blendv_epi8(vIns, vNegInf, cond);
            }
            /* check for saturation */
            {
                vSaturationCheckMax = _mm256_max_epi8(vSaturationCheckMax, vWscore);
                vSaturationCheckMin = _mm256_min_epi8(vSaturationCheckMin, vWscore);
            }
#ifdef PARASAIL_TABLE
            arr_store_si128(result->score_table, vWscore, i, s1Len, j, s2Len);
#endif
            tbl_pr[j-31] = (int8_t)_mm256_extract_epi8(vWscore,0);
            del_pr[j-31] = (int8_t)_mm256_extract_epi8(vDel,0);
            /* as minor diagonal vector passes across the i or j limit
             * boundary, extract the last value of the column or row */
            {
                __m256i vJeqLimit1 = _mm256_packs_epi16_rpl(
                        _mm256_cmpeq_epi16(vJLo16, vJLimit116),
                        _mm256_cmpeq_epi16(vJHi16, vJLimit116));
                __m256i vJgtNegOne = _mm256_packs_epi16_rpl(
                        _mm256_cmpgt_epi16(vJLo16, vNegOne16),
                        _mm256_cmpgt_epi16(vJHi16, vNegOne16));
                __m256i vJltLimit = _mm256_packs_epi16_rpl(
                        _mm256_cmplt_epi16_rpl(vJLo16, vJLimit16),
                        _mm256_cmplt_epi16_rpl(vJHi16, vJLimit16));
                __m256i cond_j = _mm256_and_si256(vIltLimit, vJeqLimit1);
                __m256i cond_i = _mm256_and_si256(vIeqLimit1,
                        _mm256_and_si256(vJgtNegOne, vJltLimit));
                __m256i cond_max = _mm256_cmpgt_epi8(vWscore, vMaxScore);
                __m256i cond_all = _mm256_and_si256(cond_max,
                        _mm256_or_si256(cond_i, cond_j));
                vMaxScore = _mm256_blendv_epi8(vMaxScore, vWscore, cond_all);
            }
            vJLo16 = _mm256_add_epi16(vJLo16, vOne16);
            vJHi16 = _mm256_add_epi16(vJHi16, vOne16);
        }
        vILo16 = _mm256_add_epi16(vILo16, vN16);
        vIHi16 = _mm256_add_epi16(vIHi16, vN16);
    }

    /* max in vMaxScore */
    for (i=0; i<N; ++i) {
        int8_t value;
        value = (int8_t) _mm256_extract_epi8(vMaxScore, 31);
        if (value > score) {
            score = value;
        }
        vMaxScore = _mm256_slli_si256_rpl(vMaxScore, 1);
    }

    if (_mm256_movemask_epi8(_mm256_or_si256(
            _mm256_cmpeq_epi8(vSaturationCheckMin, vNegLimit),
            _mm256_cmpeq_epi8(vSaturationCheckMax, vPosLimit)))) {
        result->saturated = 1;
        score = INT8_MAX;
    }

    result->score = score;

    parasail_free(_del_pr);
    parasail_free(_tbl_pr);
    parasail_free(s2B);
    parasail_free(s1);

    return result;
}


