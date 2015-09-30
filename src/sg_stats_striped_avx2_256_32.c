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

#define FASTSTATS

#define SWAP(A,B) { __m256i* tmp = A; A = B; B = tmp; }


#if HAVE_AVX2_MM256_INSERT_EPI32
#define _mm256_insert_epi32_rpl _mm256_insert_epi32
#else
static inline __m256i _mm256_insert_epi32_rpl(__m256i a, int32_t i, int imm) {
    __m256i_32_t A;
    A.m = a;
    A.v[imm] = i;
    return A.m;
}
#endif

#if HAVE_AVX2_MM256_EXTRACT_EPI32
#define _mm256_extract_epi32_rpl _mm256_extract_epi32
#else
static inline int32_t _mm256_extract_epi32_rpl(__m256i a, int imm) {
    __m256i_32_t A;
    A.m = a;
    return A.v[imm];
}
#endif

#define _mm256_cmplt_epi32_rpl(a,b) _mm256_cmpgt_epi32(b,a)

#define _mm256_slli_si256_rpl(a,imm) _mm256_alignr_epi8(a, _mm256_permute2x128_si256(a, a, _MM_SHUFFLE(0,0,3,0)), 16-imm)

static inline int32_t _mm256_hmax_epi32_rpl(__m256i a) {
    a = _mm256_max_epi32(a, _mm256_permute2x128_si256(a, a, _MM_SHUFFLE(0,0,0,0)));
    a = _mm256_max_epi32(a, _mm256_slli_si256(a, 8));
    a = _mm256_max_epi32(a, _mm256_slli_si256(a, 4));
    return _mm256_extract_epi32_rpl(a, 7);
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
    array[(0*seglen+t)*dlen + d] = (int32_t)_mm256_extract_epi32_rpl(vH, 0);
    array[(1*seglen+t)*dlen + d] = (int32_t)_mm256_extract_epi32_rpl(vH, 1);
    array[(2*seglen+t)*dlen + d] = (int32_t)_mm256_extract_epi32_rpl(vH, 2);
    array[(3*seglen+t)*dlen + d] = (int32_t)_mm256_extract_epi32_rpl(vH, 3);
    array[(4*seglen+t)*dlen + d] = (int32_t)_mm256_extract_epi32_rpl(vH, 4);
    array[(5*seglen+t)*dlen + d] = (int32_t)_mm256_extract_epi32_rpl(vH, 5);
    array[(6*seglen+t)*dlen + d] = (int32_t)_mm256_extract_epi32_rpl(vH, 6);
    array[(7*seglen+t)*dlen + d] = (int32_t)_mm256_extract_epi32_rpl(vH, 7);
}
#endif

#ifdef PARASAIL_ROWCOL
static inline void arr_store_col(
        int *col,
        __m256i vH,
        int32_t t,
        int32_t seglen)
{
    col[0*seglen+t] = (int32_t)_mm256_extract_epi32_rpl(vH, 0);
    col[1*seglen+t] = (int32_t)_mm256_extract_epi32_rpl(vH, 1);
    col[2*seglen+t] = (int32_t)_mm256_extract_epi32_rpl(vH, 2);
    col[3*seglen+t] = (int32_t)_mm256_extract_epi32_rpl(vH, 3);
    col[4*seglen+t] = (int32_t)_mm256_extract_epi32_rpl(vH, 4);
    col[5*seglen+t] = (int32_t)_mm256_extract_epi32_rpl(vH, 5);
    col[6*seglen+t] = (int32_t)_mm256_extract_epi32_rpl(vH, 6);
    col[7*seglen+t] = (int32_t)_mm256_extract_epi32_rpl(vH, 7);
}
#endif

#ifdef PARASAIL_TABLE
#define FNAME parasail_sg_stats_table_striped_avx2_256_32
#define PNAME parasail_sg_stats_table_striped_profile_avx2_256_32
#define INAME PNAME
#define STATIC
#else
#ifdef PARASAIL_ROWCOL
#define FNAME parasail_sg_stats_rowcol_striped_avx2_256_32
#define PNAME parasail_sg_stats_rowcol_striped_profile_avx2_256_32
#define INAME PNAME
#define STATIC
#else
#define FNAME parasail_sg_stats_striped_avx2_256_32
#ifdef FASTSTATS
#define PNAME parasail_sg_stats_striped_profile_avx2_256_32_internal
#define INAME parasail_sg_stats_striped_profile_avx2_256_32
#define STATIC static
#else
#define PNAME parasail_sg_stats_striped_profile_avx2_256_32
#define INAME PNAME
#define STATIC
#endif
#endif
#endif

parasail_result_t* FNAME(
        const char * const restrict s1, const int s1Len,
        const char * const restrict s2, const int s2Len,
        const int open, const int gap, const parasail_matrix_t *matrix)
{
    parasail_profile_t *profile = parasail_profile_create_stats_avx_256_32(s1, s1Len, matrix);
    parasail_result_t *result = INAME(profile, s2, s2Len, open, gap);
    parasail_profile_free(profile);
    return result;
}

STATIC parasail_result_t* PNAME(
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
    const int32_t segWidth = 8; /* number of values in vector unit */
    const int32_t segLen = (s1Len + segWidth - 1) / segWidth;
    const int32_t offset = (s1Len - 1) % segLen;
    const int32_t position = (segWidth - 1) - (s1Len - 1) / segLen;
    __m256i* const restrict vProfile  = (__m256i*)profile->profile32.score;
    __m256i* const restrict vProfileM = (__m256i*)profile->profile32.matches;
    __m256i* const restrict vProfileS = (__m256i*)profile->profile32.similar;
    __m256i* restrict pvHStore        = parasail_memalign___m256i(32, segLen);
    __m256i* restrict pvHLoad         = parasail_memalign___m256i(32, segLen);
    __m256i* restrict pvHMStore       = parasail_memalign___m256i(32, segLen);
    __m256i* restrict pvHMLoad        = parasail_memalign___m256i(32, segLen);
    __m256i* restrict pvHSStore       = parasail_memalign___m256i(32, segLen);
    __m256i* restrict pvHSLoad        = parasail_memalign___m256i(32, segLen);
    __m256i* restrict pvHLStore       = parasail_memalign___m256i(32, segLen);
    __m256i* restrict pvHLLoad        = parasail_memalign___m256i(32, segLen);
    __m256i* const restrict pvE       = parasail_memalign___m256i(32, segLen);
    __m256i* const restrict pvEM      = parasail_memalign___m256i(32, segLen);
    __m256i* const restrict pvES      = parasail_memalign___m256i(32, segLen);
    __m256i* const restrict pvEL      = parasail_memalign___m256i(32, segLen);
    const __m256i vGapO = _mm256_set1_epi32(open);
    const __m256i vGapE = _mm256_set1_epi32(gap);
    const int32_t NEG_LIMIT = (-open < matrix->min ?
        INT32_MIN + open : INT32_MIN - matrix->min) + 1;
    const int32_t POS_LIMIT = INT32_MAX - matrix->max - 1;
    const __m256i vZero = _mm256_setzero_si256();
    const __m256i vOne = _mm256_set1_epi32(1);
    const __m256i vAll = _mm256_cmpeq_epi32(vZero,vZero);
    int32_t score = NEG_LIMIT;
    int32_t matches = 0;
    int32_t similar = 0;
    int32_t length = 0;
    __m256i vNegLimit = _mm256_set1_epi32(NEG_LIMIT);
    __m256i vPosLimit = _mm256_set1_epi32(POS_LIMIT);
    __m256i vSaturationCheckMin = vPosLimit;
    __m256i vSaturationCheckMax = vNegLimit;
    __m256i vMaxH = vNegLimit;
    __m256i vMaxHM = vNegLimit;
    __m256i vMaxHS = vNegLimit;
    __m256i vMaxHL = vNegLimit;
    __m256i vPosMask = _mm256_cmpeq_epi32(_mm256_set1_epi32(position),
            _mm256_set_epi32(0,1,2,3,4,5,6,7));
#ifdef PARASAIL_TABLE
    parasail_result_t *result = parasail_result_new_table3(segLen*segWidth, s2Len);
#else
#ifdef PARASAIL_ROWCOL
    parasail_result_t *result = parasail_result_new_rowcol3(segLen*segWidth, s2Len);
#else
    parasail_result_t *result = parasail_result_new();
#endif
#endif

    parasail_memset___m256i(pvHStore, vZero, segLen);
    parasail_memset___m256i(pvHMStore, vZero, segLen);
    parasail_memset___m256i(pvHSStore, vZero, segLen);
    parasail_memset___m256i(pvHLStore, vZero, segLen);
    parasail_memset___m256i(pvE, _mm256_set1_epi32(-open), segLen);
    parasail_memset___m256i(pvEM, vZero, segLen);
    parasail_memset___m256i(pvES, vZero, segLen);
    parasail_memset___m256i(pvEL, vOne, segLen);

    /* outer loop over database sequence */
    for (j=0; j<s2Len; ++j) {
        __m256i vEF_opn;
        __m256i vE;
        __m256i vE_ext;
        __m256i vEM;
        __m256i vES;
        __m256i vEL;
        __m256i vF;
        __m256i vF_ext;
        __m256i vFM;
        __m256i vFS;
        __m256i vFL;
        __m256i vH;
        __m256i vH_dag;
        __m256i vHM;
        __m256i vHS;
        __m256i vHL;
        const __m256i* vP = NULL;
        const __m256i* vPM = NULL;
        const __m256i* vPS = NULL;
        __m256i* pv = NULL;

        /* Initialize F value to neg inf.  Any errors to vH values will
         * be corrected in the Lazy_F loop. */
        vF = vNegLimit;
        vFM = vZero;
        vFS = vZero;
        vFL = vOne;

        /* load final segment of pvHStore and shift left by 4 bytes */
        vH = _mm256_load_si256(&pvHStore[segLen - 1]);
        vHM = _mm256_load_si256(&pvHMStore[segLen - 1]);
        vHS = _mm256_load_si256(&pvHSStore[segLen - 1]);
        vHL = _mm256_load_si256(&pvHLStore[segLen - 1]);
        vH = _mm256_slli_si256_rpl(vH, 4);
        vHM = _mm256_slli_si256_rpl(vHM, 4);
        vHS = _mm256_slli_si256_rpl(vHS, 4);
        vHL = _mm256_slli_si256_rpl(vHL, 4);

        /* Correct part of the vProfile */
        vP = vProfile + matrix->mapper[(unsigned char)s2[j]] * segLen;
        vPM = vProfileM + matrix->mapper[(unsigned char)s2[j]] * segLen;
        vPS = vProfileS + matrix->mapper[(unsigned char)s2[j]] * segLen;

        /* Swap the 2 H buffers. */
        SWAP(pvHLoad,  pvHStore)
        SWAP(pvHMLoad, pvHMStore)
        SWAP(pvHSLoad, pvHSStore)
        SWAP(pvHLLoad, pvHLStore)

        /* inner loop to process the query sequence */
        for (i=0; i<segLen; ++i) {
            __m256i case1;
            __m256i case2;
            __m256i notcase1andcase2;
            __m256i notcase1andnotcase2;

            vE = _mm256_load_si256(pvE+ i);
            vEM = _mm256_load_si256(pvEM+ i);
            vES = _mm256_load_si256(pvES+ i);
            vEL = _mm256_load_si256(pvEL+ i);

            /* Get max from vH, vE and vF. */
            vH_dag = _mm256_add_epi32(vH, _mm256_load_si256(vP + i));
            vH = _mm256_max_epi32(vH_dag, vE);
            vH = _mm256_max_epi32(vH, vF);
            /* Save vH values. */
            _mm256_store_si256(pvHStore + i, vH);

            case1 = _mm256_cmpeq_epi32(vH, vH_dag);
            case2 = _mm256_cmpeq_epi32(vH, vF);
            notcase1andcase2 = _mm256_andnot_si256(case1, case2);
            notcase1andnotcase2 = _mm256_andnot_si256(case1, _mm256_xor_si256(case2, vAll));

            /* calculate vM */
            vHM = _mm256_blendv_epi8(
                    _mm256_or_si256(
                        _mm256_and_si256(notcase1andcase2, vFM),
                        _mm256_and_si256(notcase1andnotcase2, vEM)),
                    _mm256_add_epi32(vHM, _mm256_load_si256(vPM + i)),
                    case1);
            _mm256_store_si256(pvHMStore + i, vHM);

            /* calculate vS */
            vHS = _mm256_blendv_epi8(
                    _mm256_or_si256(
                        _mm256_and_si256(notcase1andcase2, vFS),
                        _mm256_and_si256(notcase1andnotcase2, vES)),
                    _mm256_add_epi32(vHS, _mm256_load_si256(vPS + i)),
                    case1);
            _mm256_store_si256(pvHSStore + i, vHS);

            /* calculate vL */
            vHL = _mm256_blendv_epi8(
                    _mm256_or_si256(
                        _mm256_and_si256(notcase1andcase2, vFL),
                        _mm256_and_si256(notcase1andnotcase2, vEL)),
                    _mm256_add_epi32(vHL, vOne),
                    case1);
            _mm256_store_si256(pvHLStore + i, vHL);

            vSaturationCheckMin = _mm256_min_epi32(vSaturationCheckMin, vH);
            vSaturationCheckMax = _mm256_max_epi32(vSaturationCheckMax, vH);
            vSaturationCheckMax = _mm256_max_epi32(vSaturationCheckMax, vHM);
            vSaturationCheckMax = _mm256_max_epi32(vSaturationCheckMax, vHS);
            vSaturationCheckMax = _mm256_max_epi32(vSaturationCheckMax, vHL);
#ifdef PARASAIL_TABLE
            arr_store_si256(result->matches_table, vHM, i, segLen, j, s2Len);
            arr_store_si256(result->similar_table, vHS, i, segLen, j, s2Len);
            arr_store_si256(result->length_table, vHL, i, segLen, j, s2Len);
            arr_store_si256(result->score_table, vH, i, segLen, j, s2Len);
#endif
            vEF_opn = _mm256_sub_epi32(vH, vGapO);

            /* Update vE value. */
            vE_ext = _mm256_sub_epi32(vE, vGapE);
            vE = _mm256_max_epi32(vEF_opn, vE_ext);
            case1 = _mm256_cmpgt_epi32(vEF_opn, vE_ext);
            vEM = _mm256_blendv_epi8(vEM, vHM, case1);
            vES = _mm256_blendv_epi8(vES, vHS, case1);
            vEL = _mm256_blendv_epi8(
                    _mm256_add_epi32(vEL, vOne),
                    _mm256_add_epi32(vHL, vOne),
                    case1);
            _mm256_store_si256(pvE + i, vE);
            _mm256_store_si256(pvEM + i, vEM);
            _mm256_store_si256(pvES + i, vES);
            _mm256_store_si256(pvEL + i, vEL);

            /* Update vF value. */
            vF_ext = _mm256_sub_epi32(vF, vGapE);
            vF = _mm256_max_epi32(vEF_opn, vF_ext);
            case1 = _mm256_cmpgt_epi32(vEF_opn, vF_ext);
            vFM = _mm256_blendv_epi8(vFM, vHM, case1);
            vFS = _mm256_blendv_epi8(vFS, vHS, case1);
            vFL = _mm256_blendv_epi8(
                    _mm256_add_epi32(vFL, vOne),
                    _mm256_add_epi32(vHL, vOne),
                    case1);

            /* Load the next vH. */
            vH = _mm256_load_si256(pvHLoad + i);
            vHM = _mm256_load_si256(pvHMLoad + i);
            vHS = _mm256_load_si256(pvHSLoad + i);
            vHL = _mm256_load_si256(pvHLLoad + i);
        }

        /* Lazy_F loop: has been revised to disallow adjecent insertion and
         * then deletion, so don't update E(i, i), learn from SWPS3 */
        for (k=0; k<segWidth; ++k) {
            __m256i vHp = _mm256_load_si256(&pvHLoad[segLen - 1]);
            vHp = _mm256_slli_si256_rpl(vHp, 4);
            vF = _mm256_slli_si256_rpl(vF, 4);
            vF = _mm256_insert_epi32_rpl(vF, -open, 0);
            vFM = _mm256_slli_si256_rpl(vFM, 4);
            vFS = _mm256_slli_si256_rpl(vFS, 4);
            vFL = _mm256_slli_si256_rpl(vFL, 4);
            vFL = _mm256_insert_epi32_rpl(vFL, 1, 0);
            for (i=0; i<segLen; ++i) {
                __m256i case1;
                __m256i case2;
                __m256i cond;

                vHp = _mm256_add_epi32(vHp, _mm256_load_si256(vP + i));
                vH = _mm256_load_si256(pvHStore + i);
                vH = _mm256_max_epi32(vH,vF);
                _mm256_store_si256(pvHStore + i, vH);
                case1 = _mm256_cmpeq_epi32(vH, vHp);
                case2 = _mm256_cmpeq_epi32(vH, vF);
                cond = _mm256_andnot_si256(case1, case2);

                /* calculate vM */
                vHM = _mm256_load_si256(pvHMStore + i);
                vHM = _mm256_blendv_epi8(vHM, vFM, cond);
                _mm256_store_si256(pvHMStore + i, vHM);

                /* calculate vS */
                vHS = _mm256_load_si256(pvHSStore + i);
                vHS = _mm256_blendv_epi8(vHS, vFS, cond);
                _mm256_store_si256(pvHSStore + i, vHS);

                /* calculate vL */
                vHL = _mm256_load_si256(pvHLStore + i);
                vHL = _mm256_blendv_epi8(vHL, vFL, cond);
                _mm256_store_si256(pvHLStore + i, vHL);

                vSaturationCheckMin = _mm256_min_epi32(vSaturationCheckMin, vH);
                vSaturationCheckMax = _mm256_max_epi32(vSaturationCheckMax, vH);
                vSaturationCheckMax = _mm256_max_epi32(vSaturationCheckMax, vHM);
                vSaturationCheckMax = _mm256_max_epi32(vSaturationCheckMax, vHS);
                vSaturationCheckMax = _mm256_max_epi32(vSaturationCheckMax, vHL);
#ifdef PARASAIL_TABLE
                arr_store_si256(result->matches_table, vHM, i, segLen, j, s2Len);
                arr_store_si256(result->similar_table, vHS, i, segLen, j, s2Len);
                arr_store_si256(result->length_table, vHL, i, segLen, j, s2Len);
                arr_store_si256(result->score_table, vH, i, segLen, j, s2Len);
#endif
                /* Update vF value. */
                vEF_opn = _mm256_sub_epi32(vH, vGapO);
                vF_ext = _mm256_sub_epi32(vF, vGapE);
                if (! _mm256_movemask_epi8(
                            _mm256_or_si256(
                                _mm256_cmpgt_epi32(vF_ext, vEF_opn),
                                _mm256_cmpeq_epi32(vF_ext, vEF_opn))))
                    goto end;
                /*vF = _mm256_max_epi32(vEF_opn, vF_ext);*/
                vF = vF_ext;
                cond = _mm256_cmpgt_epi32(vEF_opn, vF_ext);
                vFM = _mm256_blendv_epi8(vFM, vHM, cond);
                vFS = _mm256_blendv_epi8(vFS, vHS, cond);
                vFL = _mm256_blendv_epi8(
                        _mm256_add_epi32(vFL, vOne),
                        _mm256_add_epi32(vHL, vOne),
                        cond);
                vHp = _mm256_load_si256(pvHLoad + i);
            }
        }
end:
        {
        }

        /* extract vector containing last value from the column */
        __m256i cond_max;
        vH = _mm256_load_si256(pvHStore + offset);
        vHM = _mm256_load_si256(pvHMStore + offset);
        vHS = _mm256_load_si256(pvHSStore + offset);
        vHL = _mm256_load_si256(pvHLStore + offset);
        cond_max = _mm256_cmpgt_epi32(vH, vMaxH);
        vMaxH = _mm256_blendv_epi8(vMaxH, vH, cond_max);
        vMaxHM = _mm256_blendv_epi8(vMaxHM, vHM, cond_max);
        vMaxHS = _mm256_blendv_epi8(vMaxHS, vHS, cond_max);
        vMaxHL = _mm256_blendv_epi8(vMaxHL, vHL, cond_max);
        if (_mm256_movemask_epi8(_mm256_and_si256(vPosMask, cond_max))) {
            end_ref = j;
            end_query = s1Len - 1;
        }
#ifdef PARASAIL_ROWCOL
        for (k=0; k<position; ++k) {
            vH = _mm256_slli_si256_rpl(vH, 4);
            vHM = _mm256_slli_si256_rpl(vHM, 4);
            vHS = _mm256_slli_si256_rpl(vHS, 4);
            vHL = _mm256_slli_si256_rpl(vHL, 4);
        }
        result->score_row[j] = (int32_t) _mm256_extract_epi32_rpl (vH, 7);
        result->matches_row[j] = (int32_t) _mm256_extract_epi32_rpl (vHM, 7);
        result->similar_row[j] = (int32_t) _mm256_extract_epi32_rpl (vHS, 7);
        result->length_row[j] = (int32_t) _mm256_extract_epi32_rpl (vHL, 7);
#endif
    }

    {
        /* extract last value from the column maximums */
        for (k=0; k<position; ++k) {
            vMaxH  = _mm256_slli_si256_rpl (vMaxH, 4);
            vMaxHM = _mm256_slli_si256_rpl (vMaxHM, 4);
            vMaxHS = _mm256_slli_si256_rpl (vMaxHS, 4);
            vMaxHL = _mm256_slli_si256_rpl (vMaxHL, 4);
        }
        score = (int32_t) _mm256_extract_epi32_rpl (vMaxH, 7);
        matches = (int32_t)_mm256_extract_epi32_rpl(vMaxHM, 7);
        similar = (int32_t)_mm256_extract_epi32_rpl(vMaxHS, 7);
        length = (int32_t)_mm256_extract_epi32_rpl(vMaxHL, 7);
    }

    /* max of last column */
    if (INT32_MAX == profile->stop || 0 == profile->stop)
    {
        int32_t score_last;
        vMaxH = vNegLimit;

        if (0 == profile->stop) {
            /* ignore last row contributions */
            score = NEG_LIMIT;
            matches = 0;
            similar = 0;
            length = 0;
            end_query = s1Len;
            end_ref = s2Len - 1;
        }

        for (i=0; i<segLen; ++i) {
            /* load the last stored values */
            __m256i vH = _mm256_load_si256(pvHStore + i);
#ifdef PARASAIL_ROWCOL
            __m256i vHM = _mm256_load_si256(pvHMStore + i);
            __m256i vHS = _mm256_load_si256(pvHSStore + i);
            __m256i vHL = _mm256_load_si256(pvHLStore + i);
            arr_store_col(result->score_col, vH, i, segLen);
            arr_store_col(result->matches_col, vHM, i, segLen);
            arr_store_col(result->similar_col, vHS, i, segLen);
            arr_store_col(result->length_col, vHL, i, segLen);
#endif
            vMaxH = _mm256_max_epi32(vH, vMaxH);
        }

        /* max in vec */
        score_last = _mm256_hmax_epi32_rpl(vMaxH);
        if (score_last > score) {
            end_query = s1Len;
            end_ref = s2Len - 1;
            /* Trace the alignment ending position on read. */
            {
                int32_t *t = (int32_t*)pvHStore;
                int32_t *m = (int32_t*)pvHMStore;
                int32_t *s = (int32_t*)pvHSStore;
                int32_t *l = (int32_t*)pvHLStore;
                int32_t column_len = segLen * segWidth;
                for (i = 0; i<column_len; ++i, ++t, ++m, ++s, ++l) {
                    int32_t temp = i / segWidth + i % segWidth * segLen;
                    if (temp < s1Len) {
                        if (*t > score || (*t == score && temp < end_query)) {
                            score = *t;
                            end_query = temp;
                            matches = *m;
                            similar = *s;
                            length = *l;
                        }
                    }
                }
            }
        }
    }

    if (_mm256_movemask_epi8(_mm256_or_si256(
            _mm256_cmplt_epi32_rpl(vSaturationCheckMin, vNegLimit),
            _mm256_cmpgt_epi32(vSaturationCheckMax, vPosLimit)))) {
        result->saturated = 1;
        score = 0;
        matches = 0;
        similar = 0;
        length = 0;
        end_query = 0;
        end_ref = 0;
    }

    result->score = score;
    result->end_query = end_query;
    result->end_ref = end_ref;
    result->matches = matches;
    result->similar = similar;
    result->length = length;

    parasail_free(pvEL);
    parasail_free(pvES);
    parasail_free(pvEM);
    parasail_free(pvE);
    parasail_free(pvHLLoad);
    parasail_free(pvHLStore);
    parasail_free(pvHSLoad);
    parasail_free(pvHSStore);
    parasail_free(pvHMLoad);
    parasail_free(pvHMStore);
    parasail_free(pvHLoad);
    parasail_free(pvHStore);

    return result;
}

#ifdef FASTSTATS
#ifdef PARASAIL_TABLE
#else
#ifdef PARASAIL_ROWCOL
#else
#include <assert.h>
parasail_result_t* INAME(
        const parasail_profile_t * const restrict profile,
        const char * const restrict s2, const int s2Len,
        const int open, const int gap)
{
    const char *s1 = profile->s1;
    const parasail_matrix_t *matrix = profile->matrix;

    /* find the end loc first with the faster implementation */
    parasail_result_t *result = parasail_sg_striped_profile_avx2_256_32(profile, s2, s2Len, open, gap);
    if (!result->saturated) {
        int s1Len_new = 0;
        int s2Len_new = 0;
        parasail_result_t *result_final = NULL;

        /* using the end loc, call the original stats function */
        s1Len_new = result->end_query+1;
        s2Len_new = result->end_ref+1;

        if (s1Len_new == profile->s1Len) {
            /* special 'stop' value tells stats function not to
             * consider last column results */
            int stop_save = profile->stop;
            ((parasail_profile_t*)profile)->stop = 1;
            result_final = PNAME(
                    profile, s2, s2Len_new, open, gap);
            ((parasail_profile_t*)profile)->stop = stop_save;
        }
        else {
            parasail_profile_t *profile_final = NULL;
            profile_final = parasail_profile_create_stats_avx_256_32(
                    s1, s1Len_new, matrix);
            /* special 'stop' value tells stats function not to
             * consider last row results */
            profile_final->stop = 0;
            result_final = PNAME(
                    profile_final, s2, s2Len_new, open, gap);

            parasail_profile_free(profile_final);
        }

        parasail_result_free(result);

        /* correct the end locations before returning */
        result_final->end_query = s1Len_new-1;
        result_final->end_ref = s2Len_new-1;
        return result_final;
    }
    else {
        return result;
    }
}
#endif
#endif
#endif


