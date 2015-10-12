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

#define SWAP(A,B) { __m256i* tmp = A; A = B; B = tmp; }
#define SWAP3(A,B,C) { __m256i* tmp = A; A = B; B = C; C = tmp; }

#define NEG_INF (INT16_MIN/(int16_t)(2))

#if HAVE_AVX2_MM256_EXTRACT_EPI16
#define _mm256_extract_epi16_rpl _mm256_extract_epi16
#else
static inline int16_t _mm256_extract_epi16_rpl(__m256i a, int imm) {
    __m256i_16_t A;
    A.m = a;
    return A.v[imm];
}
#endif

#if HAVE_AVX2_MM256_INSERT_EPI16
#define _mm256_insert_epi16_rpl _mm256_insert_epi16
#else
static inline __m256i _mm256_insert_epi16_rpl(__m256i a, int16_t i, int imm) {
    __m256i_16_t A;
    A.m = a;
    A.v[imm] = i;
    return A.m;
}
#endif

#define _mm256_slli_si256_rpl(a,imm) _mm256_alignr_epi8(a, _mm256_permute2x128_si256(a, a, _MM_SHUFFLE(0,0,3,0)), 16-imm)

static inline int16_t _mm256_hmax_epi16_rpl(__m256i a) {
    a = _mm256_max_epi16(a, _mm256_permute2x128_si256(a, a, _MM_SHUFFLE(0,0,0,0)));
    a = _mm256_max_epi16(a, _mm256_slli_si256(a, 8));
    a = _mm256_max_epi16(a, _mm256_slli_si256(a, 4));
    a = _mm256_max_epi16(a, _mm256_slli_si256(a, 2));
    return _mm256_extract_epi16_rpl(a, 15);
}


static inline void arr_store(
        int *array,
        __m256i vH,
        int32_t t,
        int32_t seglen,
        int32_t d,
        int32_t dlen)
{
    array[( 0*seglen+t)*dlen + d] = (int16_t)_mm256_extract_epi16_rpl(vH,  0);
    array[( 1*seglen+t)*dlen + d] = (int16_t)_mm256_extract_epi16_rpl(vH,  1);
    array[( 2*seglen+t)*dlen + d] = (int16_t)_mm256_extract_epi16_rpl(vH,  2);
    array[( 3*seglen+t)*dlen + d] = (int16_t)_mm256_extract_epi16_rpl(vH,  3);
    array[( 4*seglen+t)*dlen + d] = (int16_t)_mm256_extract_epi16_rpl(vH,  4);
    array[( 5*seglen+t)*dlen + d] = (int16_t)_mm256_extract_epi16_rpl(vH,  5);
    array[( 6*seglen+t)*dlen + d] = (int16_t)_mm256_extract_epi16_rpl(vH,  6);
    array[( 7*seglen+t)*dlen + d] = (int16_t)_mm256_extract_epi16_rpl(vH,  7);
    array[( 8*seglen+t)*dlen + d] = (int16_t)_mm256_extract_epi16_rpl(vH,  8);
    array[( 9*seglen+t)*dlen + d] = (int16_t)_mm256_extract_epi16_rpl(vH,  9);
    array[(10*seglen+t)*dlen + d] = (int16_t)_mm256_extract_epi16_rpl(vH, 10);
    array[(11*seglen+t)*dlen + d] = (int16_t)_mm256_extract_epi16_rpl(vH, 11);
    array[(12*seglen+t)*dlen + d] = (int16_t)_mm256_extract_epi16_rpl(vH, 12);
    array[(13*seglen+t)*dlen + d] = (int16_t)_mm256_extract_epi16_rpl(vH, 13);
    array[(14*seglen+t)*dlen + d] = (int16_t)_mm256_extract_epi16_rpl(vH, 14);
    array[(15*seglen+t)*dlen + d] = (int16_t)_mm256_extract_epi16_rpl(vH, 15);
}

#define FNAME parasail_sw_trace_striped_avx2_256_16
#define PNAME parasail_sw_trace_striped_profile_avx2_256_16

parasail_result_t* FNAME(
        const char * const restrict s1, const int s1Len,
        const char * const restrict s2, const int s2Len,
        const int open, const int gap, const parasail_matrix_t *matrix)
{
    parasail_profile_t *profile = parasail_profile_create_avx_256_16(s1, s1Len, matrix);
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
    __m256i* const restrict vProfile = (__m256i*)profile->profile16.score;
    __m256i* restrict pvHStore = parasail_memalign___m256i(32, segLen);
    __m256i* restrict pvHLoad =  parasail_memalign___m256i(32, segLen);
    __m256i* const restrict pvE = parasail_memalign___m256i(32, segLen);
    __m256i* restrict pvEaStore = parasail_memalign___m256i(32, segLen);
    __m256i* restrict pvEaLoad = parasail_memalign___m256i(32, segLen);
    __m256i* const restrict pvHT = parasail_memalign___m256i(32, segLen);
    __m256i* restrict pvHMax = parasail_memalign___m256i(32, segLen);
    __m256i vGapO = _mm256_set1_epi16(open);
    __m256i vGapE = _mm256_set1_epi16(gap);
    __m256i vZero = _mm256_setzero_si256();
    int16_t score = NEG_INF;
    __m256i vMaxH = vZero;
    __m256i vMaxHUnit = vZero;
    int16_t maxp = INT16_MAX - (int16_t)(matrix->max+1);
    /*int16_t stop = profile->stop == INT32_MAX ?  INT16_MAX : (int16_t)profile->stop;*/
    parasail_result_t *result = parasail_result_new_trace(segLen*segWidth, s2Len);
    __m256i vTZero = _mm256_set1_epi16(PARASAIL_ZERO);
    __m256i vTIns  = _mm256_set1_epi16(PARASAIL_INS);
    __m256i vTDel  = _mm256_set1_epi16(PARASAIL_DEL);
    __m256i vTDiag = _mm256_set1_epi16(PARASAIL_DIAG);

    /* initialize H and E */
    parasail_memset___m256i(pvHStore, vZero, segLen);
    parasail_memset___m256i(pvE, _mm256_set1_epi16(-open), segLen);
    parasail_memset___m256i(pvEaStore, _mm256_set1_epi16(-open), segLen);

    for (i=0; i<segLen; ++i) {
        arr_store(result->trace_ins_table,
                vTDiag, i, segLen, 0, s2Len);
    }

    /* outer loop over database sequence */
    for (j=0; j<s2Len; ++j) {
        __m256i vEF_opn;
        __m256i vE;
        __m256i vE_ext;
        __m256i vF;
        __m256i vF_ext;
        __m256i vFa;
        __m256i vFa_ext;
        __m256i vH;
        __m256i vH_dag;
        const __m256i* vP = NULL;

        /* Initialize F value to 0.  Any errors to vH values will be
         * corrected in the Lazy_F loop. */
        vF = _mm256_sub_epi16(vZero,vGapO);

        /* load final segment of pvHStore and shift left by 2 bytes */
        vH = _mm256_load_si256(&pvHStore[segLen - 1]);
        vH = _mm256_slli_si256_rpl(vH, 2);

        /* Correct part of the vProfile */
        vP = vProfile + matrix->mapper[(unsigned char)s2[j]] * segLen;

        if (end_ref == j-2) {
            /* Swap in the max buffer. */
            SWAP3(pvHMax,  pvHLoad,  pvHStore)
            SWAP(pvEaLoad,  pvEaStore)
        }
        else {
            /* Swap the 2 H buffers. */
            SWAP(pvHLoad,  pvHStore)
            SWAP(pvEaLoad,  pvEaStore)
        }

        /* inner loop to process the query sequence */
        for (i=0; i<segLen; ++i) {
            vE = _mm256_load_si256(pvE + i);

            /* Get max from vH, vE and vF. */
            vH_dag = _mm256_add_epi16(vH, _mm256_load_si256(vP + i));
            vH_dag = _mm256_max_epi16(vH_dag, vZero);
            vH = _mm256_max_epi16(vH_dag, vE);
            vH = _mm256_max_epi16(vH, vF);
            /* Save vH values. */
            _mm256_store_si256(pvHStore + i, vH);

            {
                __m256i cond_zero = _mm256_cmpeq_epi16(vH, vZero);
                __m256i case1 = _mm256_cmpeq_epi16(vH, vH_dag);
                __m256i case2 = _mm256_cmpeq_epi16(vH, vF);
                __m256i vT = _mm256_blendv_epi8(
                        _mm256_blendv_epi8(vTIns, vTDel, case2),
                        _mm256_blendv_epi8(vTDiag, vTZero, cond_zero),
                        case1);
                _mm256_store_si256(pvHT + i, vT);
                arr_store(result->trace_table, vT, i, segLen, j, s2Len);
            }
            vMaxH = _mm256_max_epi16(vH, vMaxH);
            vEF_opn = _mm256_sub_epi16(vH, vGapO);

            /* Update vE value. */
            vE_ext = _mm256_sub_epi16(vE, vGapE);
            vE = _mm256_max_epi16(vEF_opn, vE_ext);
            _mm256_store_si256(pvE + i, vE);
            {
                __m256i vEa = _mm256_load_si256(pvEaLoad + i);
                __m256i vEa_ext = _mm256_sub_epi16(vEa, vGapE);
                vEa = _mm256_max_epi16(vEF_opn, vEa_ext);
                _mm256_store_si256(pvEaStore + i, vEa);
                if (j+1<s2Len) {
                    __m256i cond = _mm256_cmpgt_epi16(vEF_opn, vEa_ext);
                    __m256i vT = _mm256_blendv_epi8(vTIns, vTDiag, cond);
                    arr_store(result->trace_ins_table, vT, i, segLen, j+1, s2Len);
                }
            }

            /* Update vF value. */
            vF_ext = _mm256_sub_epi16(vF, vGapE);
            vF = _mm256_max_epi16(vEF_opn, vF_ext);
            {
                __m256i cond = _mm256_cmpgt_epi16(vEF_opn, vF_ext);
                __m256i vT = _mm256_blendv_epi8(vTDel, vTDiag, cond);
                if (i+1<segLen) {
                    arr_store(result->trace_del_table, vT, i+1, segLen, j, s2Len);
                }
            }

            /* Load the next vH. */
            vH = _mm256_load_si256(pvHLoad + i);
        }

        /* Lazy_F loop: has been revised to disallow adjecent insertion and
         * then deletion, so don't update E(i, i), learn from SWPS3 */
        vFa_ext = vF_ext;
        vFa = vF;
        for (k=0; k<segWidth; ++k) {
            __m256i vHp = _mm256_load_si256(&pvHLoad[segLen - 1]);
            vHp = _mm256_slli_si256_rpl(vHp, 2);
            vEF_opn = _mm256_slli_si256_rpl(vEF_opn, 2);
            vEF_opn = _mm256_insert_epi16_rpl(vEF_opn, -open, 0);
            vF_ext = _mm256_slli_si256_rpl(vF_ext, 2);
            vF_ext = _mm256_insert_epi16_rpl(vF_ext, NEG_INF, 0);
            vF = _mm256_slli_si256_rpl(vF, 2);
            vF = _mm256_insert_epi16_rpl(vF, -open, 0);
            vFa_ext = _mm256_slli_si256_rpl(vFa_ext, 2);
            vFa_ext = _mm256_insert_epi16_rpl(vFa_ext, NEG_INF, 0);
            vFa = _mm256_slli_si256_rpl(vFa, 2);
            vFa = _mm256_insert_epi16_rpl(vFa, -open, 0);
            for (i=0; i<segLen; ++i) {
                vH = _mm256_load_si256(pvHStore + i);
                vH = _mm256_max_epi16(vH,vF);
                _mm256_store_si256(pvHStore + i, vH);
                {
                    __m256i vT;
                    __m256i case1;
                    __m256i case2;
                    __m256i cond;
                    vHp = _mm256_add_epi16(vHp, _mm256_load_si256(vP + i));
                    vHp = _mm256_max_epi16(vHp, vZero);
                    case1 = _mm256_cmpeq_epi16(vH, vHp);
                    case2 = _mm256_cmpeq_epi16(vH, vF);
                    cond = _mm256_andnot_si256(case1,case2);
                    vT = _mm256_load_si256(pvHT + i);
                    vT = _mm256_blendv_epi8(vT, vTDel, cond);
                    _mm256_store_si256(pvHT + i, vT);
                    arr_store(result->trace_table, vT, i, segLen, j, s2Len);
                }
                vMaxH = _mm256_max_epi16(vH, vMaxH);
                /* Update vF value. */
                {
                    __m256i cond = _mm256_cmpgt_epi16(vEF_opn, vFa_ext);
                    __m256i vT = _mm256_blendv_epi8(vTDel, vTDiag, cond);
                    arr_store(result->trace_del_table, vT, i, segLen, j, s2Len);
                }
                vEF_opn = _mm256_sub_epi16(vH, vGapO);
                vF_ext = _mm256_sub_epi16(vF, vGapE);
                {
                    __m256i vT;
                    __m256i cond;
                    __m256i vEa = _mm256_load_si256(pvEaLoad + i);
                    __m256i vEa_ext = _mm256_sub_epi16(vEa, vGapE);
                    vEa = _mm256_max_epi16(vEF_opn, vEa_ext);
                    _mm256_store_si256(pvEaStore + i, vEa);
                    cond = _mm256_cmpgt_epi16(vEF_opn, vEa_ext);
                    vT = _mm256_blendv_epi8(vTIns, vTDiag, cond);
                    if (j+1<s2Len) {
                        arr_store(result->trace_ins_table, vT, i, segLen, j+1, s2Len);
                    }
                }
                if (! _mm256_movemask_epi8(
                            _mm256_or_si256(
                                _mm256_cmpgt_epi16(vF_ext, vEF_opn),
                                _mm256_cmpeq_epi16(vF_ext, vEF_opn))))
                    goto end;
                /*vF = _mm256_max_epi16(vEF_opn, vF_ext);*/
                vF = vF_ext;
                vFa_ext = _mm256_sub_epi16(vFa, vGapE);
                vFa = _mm256_max_epi16(vEF_opn, vFa_ext);
                vHp = _mm256_load_si256(pvHLoad + i);
            }
        }
end:
        {
        }

        {
            __m256i vCompare = _mm256_cmpgt_epi16(vMaxH, vMaxHUnit);
            if (_mm256_movemask_epi8(vCompare)) {
                score = _mm256_hmax_epi16_rpl(vMaxH);
                /* if score has potential to overflow, abort early */
                if (score > maxp) {
                    result->saturated = 1;
                    break;
                }
                vMaxHUnit = _mm256_set1_epi16(score);
                end_ref = j;
            }
        }

        /*if (score == stop) break;*/
    }

    if (score == INT16_MAX) {
        result->saturated = 1;
    }

    if (result->saturated) {
        score = 0;
        end_query = 0;
        end_ref = 0;
    }
    else {
        if (end_ref == j-1) {
            /* end_ref was the last store column */
            SWAP(pvHMax,  pvHStore)
        }
        else if (end_ref == j-2) {
            /* end_ref was the last load column */
            SWAP(pvHMax,  pvHLoad)
        }
        /* Trace the alignment ending position on read. */
        {
            int16_t *t = (int16_t*)pvHMax;
            int32_t column_len = segLen * segWidth;
            end_query = s1Len - 1;
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

    result->score = score;
    result->end_query = end_query;
    result->end_ref = end_ref;

    parasail_free(pvHMax);
    parasail_free(pvHT);
    parasail_free(pvEaLoad);
    parasail_free(pvEaStore);
    parasail_free(pvE);
    parasail_free(pvHLoad);
    parasail_free(pvHStore);

    return result;
}


