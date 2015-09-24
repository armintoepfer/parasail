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

%(HEADER)s

#include "parasail.h"
#include "parasail/memory.h"
#include "parasail/internal_%(ISA)s.h"

#define NEG_INF %(NEG_INF)s
#define MAX(a,b) ((a)>(b)?(a):(b))
%(FIXES)s

#ifdef PARASAIL_TABLE
static inline void arr_store_si%(BITS)s(
        int *array,
        %(VTYPE)s vH,
        %(INDEX)s t,
        %(INDEX)s seglen,
        %(INDEX)s d,
        %(INDEX)s dlen)
{
%(PRINTER)s
}
#endif

#ifdef PARASAIL_ROWCOL
static inline void arr_store_col(
        int *col,
        %(VTYPE)s vH,
        %(INDEX)s t,
        %(INDEX)s seglen)
{
%(PRINTER_ROWCOL)s
}
#endif

#ifdef PARASAIL_TABLE
#define FNAME %(NAME_TABLE)s
#define PNAME %(PNAME_TABLE)s
#else
#ifdef PARASAIL_ROWCOL
#define FNAME %(NAME_ROWCOL)s
#define PNAME %(PNAME_ROWCOL)s
#else
#define FNAME %(NAME)s
#define PNAME %(PNAME)s
#endif
#endif

parasail_result_t* FNAME(
        const char * const restrict s1, const int s1Len,
        const char * const restrict s2, const int s2Len,
        const int open, const int gap, const parasail_matrix_t *matrix)
{
    parasail_profile_t *profile = parasail_profile_create_stats_%(ISA)s_%(BITS)s_%(WIDTH)s(s1, s1Len, matrix);
    parasail_result_t *result = PNAME(profile, s2, s2Len, open, gap);
    parasail_profile_free(profile);
    return result;
}

parasail_result_t* PNAME(
        const parasail_profile_t * const restrict profile,
        const char * const restrict s2, const int s2Len,
        const int open, const int gap)
{
    %(INDEX)s i = 0;
    %(INDEX)s j = 0;
    %(INDEX)s k = 0;
    %(INDEX)s segNum = 0;
    const int s1Len = profile->s1Len;
    %(INDEX)s end_query = s1Len-1;
    %(INDEX)s end_ref = s2Len-1;
    const parasail_matrix_t *matrix = profile->matrix;
    const %(INDEX)s segWidth = %(LANES)s;
    const %(INDEX)s segLen = (s1Len + segWidth - 1) / segWidth;
    const %(INDEX)s offset = (s1Len - 1) %% segLen;
    const %(INDEX)s position = (segWidth - 1) - (s1Len - 1) / segLen;
    %(VTYPE)s* const restrict pvP  = (%(VTYPE)s*)profile->profile%(WIDTH)s.score;
    %(VTYPE)s* const restrict pvPm = (%(VTYPE)s*)profile->profile%(WIDTH)s.matches;
    %(VTYPE)s* const restrict pvPs = (%(VTYPE)s*)profile->profile%(WIDTH)s.similar;
    %(VTYPE)s* const restrict pvE  = parasail_memalign_%(VTYPE)s(%(ALIGNMENT)s, segLen);
    %(VTYPE)s* const restrict pvHt = parasail_memalign_%(VTYPE)s(%(ALIGNMENT)s, segLen);
    %(VTYPE)s* const restrict pvFt = parasail_memalign_%(VTYPE)s(%(ALIGNMENT)s, segLen);
    %(VTYPE)s* const restrict pvMt = parasail_memalign_%(VTYPE)s(%(ALIGNMENT)s, segLen);
    %(VTYPE)s* const restrict pvSt = parasail_memalign_%(VTYPE)s(%(ALIGNMENT)s, segLen);
    %(VTYPE)s* const restrict pvLt = parasail_memalign_%(VTYPE)s(%(ALIGNMENT)s, segLen);
    %(VTYPE)s* const restrict pvEx = parasail_memalign_%(VTYPE)s(%(ALIGNMENT)s, segLen);
    %(VTYPE)s* const restrict pvH  = parasail_memalign_%(VTYPE)s(%(ALIGNMENT)s, segLen);
    %(VTYPE)s* const restrict pvM  = parasail_memalign_%(VTYPE)s(%(ALIGNMENT)s, segLen);
    %(VTYPE)s* const restrict pvS  = parasail_memalign_%(VTYPE)s(%(ALIGNMENT)s, segLen);
    %(VTYPE)s* const restrict pvL  = parasail_memalign_%(VTYPE)s(%(ALIGNMENT)s, segLen);
    %(INT)s* const restrict boundary = parasail_memalign_%(INT)s(%(ALIGNMENT)s, s2Len+1);
    %(VTYPE)s vGapO = %(VSET1)s(open);
    %(VTYPE)s vGapE = %(VSET1)s(gap);
    %(VTYPE)s vZero = %(VSET0)s();
    %(VTYPE)s vOne = %(VSET1)s(1);
    %(VTYPE)s vNegInf = %(VSET1)s(NEG_INF);
    %(INT)s score = NEG_INF;
    %(INT)s matches = 0;
    %(INT)s similar = 0;
    %(INT)s length = 0;
    %(STATS_SATURATION_CHECK_INIT)s
#ifdef PARASAIL_TABLE
    parasail_result_t *result = parasail_result_new_table3(segLen*segWidth, s2Len);
#else
#ifdef PARASAIL_ROWCOL
    parasail_result_t *result = parasail_result_new_rowcol3(segLen*segWidth, s2Len);
#else
    parasail_result_t *result = parasail_result_new();
#endif
#endif

    parasail_memset_%(VTYPE)s(pvM, vZero, segLen);
    parasail_memset_%(VTYPE)s(pvS, vZero, segLen);
    parasail_memset_%(VTYPE)s(pvL, vZero, segLen);

    /* initialize H and E */
    {
        %(INDEX)s index = 0;
        for (i=0; i<segLen; ++i) {
            %(VTYPE)s_%(WIDTH)s_t h;
            %(VTYPE)s_%(WIDTH)s_t e;
            for (segNum=0; segNum<segWidth; ++segNum) {
                int64_t tmp = -open-gap*(segNum*segLen+i);
                h.v[segNum] = tmp < INT%(WIDTH)s_MIN ? INT%(WIDTH)s_MIN : tmp;
                e.v[segNum] = NEG_INF;
            }
            %(VSTORE)s(&pvH[index], h.m);
            %(VSTORE)s(&pvE[index], e.m);
            ++index;
        }
    }

    /* initialize uppder boundary */
    {
        boundary[0] = 0;
        for (i=1; i<=s2Len; ++i) {
            int64_t tmp = -open-gap*(i-1);
            boundary[i] = tmp < INT%(WIDTH)s_MIN ? INT%(WIDTH)s_MIN : tmp;
        }
    }

    /* outer loop over database sequence */
    for (j=0; j<s2Len; ++j) {
        %(VTYPE)s vE;
        %(VTYPE)s vHt;
        %(VTYPE)s vFt;
        %(VTYPE)s vH;
        %(VTYPE)s *pvW;
        %(VTYPE)s vW;
        %(VTYPE)s *pvC;
        %(VTYPE)s *pvD;
        %(VTYPE)s vC;
        %(VTYPE)s vD;
        %(VTYPE)s vM;
        %(VTYPE)s vMp;
        %(VTYPE)s vMt;
        %(VTYPE)s vS;
        %(VTYPE)s vSp;
        %(VTYPE)s vSt;
        %(VTYPE)s vL;
        %(VTYPE)s vLp;
        %(VTYPE)s vLt;
        %(VTYPE)s vEx;

        /* calculate E */
        for (i=0; i<segLen; ++i) {
            vH = %(VLOAD)s(pvH+i);
            vE = %(VLOAD)s(pvE+i);
            vE = %(VMAX)s(
                    %(VSUB)s(vE, vGapE),
                    %(VSUB)s(vH, vGapO));
            %(VSTORE)s(pvE+i, vE);
        }

        /* calculate Ht */
        vH = %(VSHIFT)s(%(VLOAD)s(pvH+(segLen-1)), %(BYTES)s);
        vH = %(VINSERT)s(vH, boundary[j], 0);
        vMp= %(VSHIFT)s(%(VLOAD)s(pvM+(segLen-1)), %(BYTES)s);
        vSp= %(VSHIFT)s(%(VLOAD)s(pvS+(segLen-1)), %(BYTES)s);
        vLp= %(VSHIFT)s(%(VLOAD)s(pvL+(segLen-1)), %(BYTES)s);
        vLp= %(VADD)s(vLp, vOne);
        pvW = pvP + matrix->mapper[(unsigned char)s2[j]]*segLen;
        pvC = pvPm+ matrix->mapper[(unsigned char)s2[j]]*segLen;
        pvD = pvPs+ matrix->mapper[(unsigned char)s2[j]]*segLen;
        for (i=0; i<segLen; ++i) {
            /* load values we need */
            vE = %(VLOAD)s(pvE+i);
            vW = %(VLOAD)s(pvW+i);
            /* compute */
            vH = %(VADD)s(vH, vW);
            vHt = %(VMAX)s(vH, vE);
            /* statistics */
            vC = %(VLOAD)s(pvC+i);
            vD = %(VLOAD)s(pvD+i);
            vMp = %(VADD)s(vMp, vC);
            vSp = %(VADD)s(vSp, vD);
            vEx = %(VCMPGT)s(vE, vH);
            vM = %(VLOAD)s(pvM+i);
            vS = %(VLOAD)s(pvS+i);
            vL = %(VLOAD)s(pvL+i);
            vL = %(VADD)s(vL, vOne);
            vMt = %(VBLEND)s(vMp, vM, vEx);
            vSt = %(VBLEND)s(vSp, vS, vEx);
            vLt = %(VBLEND)s(vLp, vL, vEx);
            /* store results */
            %(VSTORE)s(pvHt+i, vHt);
            %(VSTORE)s(pvEx+i, vEx);
            %(VSTORE)s(pvMt+i, vMt);
            %(VSTORE)s(pvSt+i, vSt);
            %(VSTORE)s(pvLt+i, vLt);
            /* prep for next iteration */
            vH = %(VLOAD)s(pvH+i);
            vMp = vM;
            vSp = vS;
            vLp = vL;
        }

        /* calculate Ft */
        vHt = %(VSHIFT)s(%(VLOAD)s(pvHt+(segLen-1)), %(BYTES)s);
        vHt = %(VINSERT)s(vHt, boundary[j+1], 0);
        vFt = vNegInf;
        for (i=0; i<segLen; ++i) {
            vFt = %(VSUB)s(vFt, vGapE);
            vFt = %(VMAX)s(vFt, vHt);
            vHt = %(VLOAD)s(pvHt+i);
        }
        {
            %(VTYPE)s_%(WIDTH)s_t tmp;
            tmp.m = vFt;
            %(STATS_SCAN_VFT)s
            vFt = tmp.m;
        }
        vHt = %(VSHIFT)s(%(VLOAD)s(pvHt+(segLen-1)), %(BYTES)s);
        vHt = %(VINSERT)s(vHt, boundary[j+1], 0);
        vFt = %(VSHIFT)s(vFt, %(BYTES)s);
        vFt = %(VINSERT)s(vFt, NEG_INF, 0);
        for (i=0; i<segLen; ++i) {
            vFt = %(VSUB)s(vFt, vGapE);
            vFt = %(VMAX)s(vFt, vHt);
            vHt = %(VLOAD)s(pvHt+i);
            %(VSTORE)s(pvFt+i, vFt);
        }

        /* calculate H,M,L */
        vMp = vZero;
        vSp = vZero;
        vLp = vOne;
        vC = %(VCMPEQ)s(vZero, vZero); /* check if prefix sum is needed */
        vC = %(VRSHIFT)s(vC, %(BYTES)s); /* zero out last value */
        for (i=0; i<segLen; ++i) {
            /* load values we need */
            vHt = %(VLOAD)s(pvHt+i);
            vFt = %(VLOAD)s(pvFt+i);
            /* compute */
            vFt = %(VSUB)s(vFt, vGapO);
            vH = %(VMAX)s(vHt, vFt);
            /* statistics */
            vEx = %(VLOAD)s(pvEx+i);
            vMt = %(VLOAD)s(pvMt+i);
            vSt = %(VLOAD)s(pvSt+i);
            vLt = %(VLOAD)s(pvLt+i);
            vEx = %(VOR)s(
                    %(VAND)s(vEx, %(VCMPEQ)s(vHt, vFt)),
                    %(VCMPLT)s(vHt, vFt));
            vM = %(VBLEND)s(vMt, vMp, vEx);
            vS = %(VBLEND)s(vSt, vSp, vEx);
            vL = %(VBLEND)s(vLt, vLp, vEx);
            vMp = vM;
            vSp = vS;
            vLp = %(VADD)s(vL, vOne);
            vC = %(VAND)s(vC, vEx);
            /* store results */
            %(VSTORE)s(pvH+i, vH);
            %(VSTORE)s(pvEx+i, vEx);
            %(STATS_SATURATION_CHECK_MID1)s
#ifdef PARASAIL_TABLE
            arr_store_si%(BITS)s(result->score_table, vH, i, segLen, j, s2Len);
#endif
        }
        {
            vLp = %(VSUB)s(vLp, vOne);
            {
                %(VTYPE)s_%(WIDTH)s_t uMp, uSp, uLp, uC;
                uC.m = vC;
                uMp.m = vMp;
                %(STATS_SCAN_UMP)s
                vMp = uMp.m;
                uSp.m = vSp;
                %(STATS_SCAN_USP)s
                vSp = uSp.m;
                uLp.m = vLp;
                %(STATS_SCAN_ULP)s
                vLp = uLp.m;
            }
            vLp = %(VADD)s(vLp, vOne);
        }
        /* final pass for M,L */
        vMp = %(VSHIFT)s(vMp, %(BYTES)s);
        vSp = %(VSHIFT)s(vSp, %(BYTES)s);
        vLp = %(VSHIFT)s(vLp, %(BYTES)s);
        for (i=0; i<segLen; ++i) {
            /* statistics */
            vEx = %(VLOAD)s(pvEx+i);
            vMt = %(VLOAD)s(pvMt+i);
            vSt = %(VLOAD)s(pvSt+i);
            vLt = %(VLOAD)s(pvLt+i);
            vM = %(VBLEND)s(vMt, vMp, vEx);
            vS = %(VBLEND)s(vSt, vSp, vEx);
            vL = %(VBLEND)s(vLt, vLp, vEx);
            vMp = vM;
            vSp = vS;
            vLp = %(VADD)s(vL, vOne);
            /* store results */
            %(VSTORE)s(pvM+i, vM);
            %(VSTORE)s(pvS+i, vS);
            %(VSTORE)s(pvL+i, vL);
            %(STATS_SATURATION_CHECK_MID2)s
#ifdef PARASAIL_TABLE
            arr_store_si%(BITS)s(result->matches_table, vM, i, segLen, j, s2Len);
            arr_store_si%(BITS)s(result->similar_table, vS, i, segLen, j, s2Len);
            arr_store_si%(BITS)s(result->length_table, vL, i, segLen, j, s2Len);
#endif
        }

#ifdef PARASAIL_ROWCOL
        /* extract last value from the column */
        {
            vH = %(VLOAD)s(pvH + offset);
            vM = %(VLOAD)s(pvM + offset);
            vS = %(VLOAD)s(pvS + offset);
            vL = %(VLOAD)s(pvL + offset);
            for (k=0; k<position; ++k) {
                vH = %(VSHIFT)s(vH, %(BYTES)s);
                vM = %(VSHIFT)s(vM, %(BYTES)s);
                vS = %(VSHIFT)s(vS, %(BYTES)s);
                vL = %(VSHIFT)s(vL, %(BYTES)s);
            }
            result->score_row[j] = (%(INT)s) %(VEXTRACT)s (vH, %(LAST_POS)s);
            result->matches_row[j] = (%(INT)s) %(VEXTRACT)s (vM, %(LAST_POS)s);
            result->similar_row[j] = (%(INT)s) %(VEXTRACT)s (vS, %(LAST_POS)s);
            result->length_row[j] = (%(INT)s) %(VEXTRACT)s (vL, %(LAST_POS)s);
        }
#endif
    }

#ifdef PARASAIL_ROWCOL
    for (i=0; i<segLen; ++i) {
        %(VTYPE)s vH = %(VLOAD)s(pvH+i);
        %(VTYPE)s vM = %(VLOAD)s(pvM+i);
        %(VTYPE)s vS = %(VLOAD)s(pvS+i);
        %(VTYPE)s vL = %(VLOAD)s(pvL+i);
        arr_store_col(result->score_col, vH, i, segLen);
        arr_store_col(result->matches_col, vM, i, segLen);
        arr_store_col(result->similar_col, vS, i, segLen);
        arr_store_col(result->length_col, vL, i, segLen);
    }
#endif

    /* extract last value from the last column */
    {
        %(VTYPE)s vH = %(VLOAD)s(pvH + offset);
        %(VTYPE)s vM = %(VLOAD)s(pvM + offset);
        %(VTYPE)s vS = %(VLOAD)s(pvS + offset);
        %(VTYPE)s vL = %(VLOAD)s(pvL + offset);
        for (k=0; k<position; ++k) {
            vH = %(VSHIFT)s(vH, %(BYTES)s);
            vM = %(VSHIFT)s(vM, %(BYTES)s);
            vS = %(VSHIFT)s(vS, %(BYTES)s);
            vL = %(VSHIFT)s(vL, %(BYTES)s);
        }
        score = (%(INT)s) %(VEXTRACT)s (vH, %(LAST_POS)s);
        matches = (%(INT)s) %(VEXTRACT)s (vM, %(LAST_POS)s);
        similar = (%(INT)s) %(VEXTRACT)s (vS, %(LAST_POS)s);
        length = (%(INT)s) %(VEXTRACT)s (vL, %(LAST_POS)s);
    }

    %(STATS_SATURATION_CHECK_FINAL)s

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

