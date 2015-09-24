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

#include "parasail.h"
#include "parasail/memory.h"

#define NEG_INF_32 (INT32_MIN/2)
#define MAX(a,b) ((a)>(b)?(a):(b))

#ifdef PARASAIL_TABLE
#define ENAME parasail_sw_stats_table
#else
#ifdef PARASAIL_ROWCOL
#define ENAME parasail_sw_stats_rowcol
#else
#define ENAME parasail_sw_stats
#endif
#endif

parasail_result_t* ENAME(
        const char * const restrict _s1, const int s1Len,
        const char * const restrict _s2, const int s2Len,
        const int open, const int gap, const parasail_matrix_t *matrix)
{
#ifdef PARASAIL_TABLE
    parasail_result_t *result = parasail_result_new_table3(s1Len, s2Len);
#else
#ifdef PARASAIL_ROWCOL
    parasail_result_t *result = parasail_result_new_rowcol3(s1Len, s2Len);
#else
    parasail_result_t *result = parasail_result_new();
#endif
#endif
    int * const restrict s1 = parasail_memalign_int(16, s1Len);
    int * const restrict s2 = parasail_memalign_int(16, s2Len);
    int * const restrict H = parasail_memalign_int(16, s2Len+1);
    int * const restrict HM = parasail_memalign_int(16, s2Len+1);
    int * const restrict HS = parasail_memalign_int(16, s2Len+1);
    int * const restrict HL = parasail_memalign_int(16, s2Len+1);
    int * const restrict F = parasail_memalign_int(16, s2Len+1);
    int * const restrict FM = parasail_memalign_int(16, s2Len+1);
    int * const restrict FS = parasail_memalign_int(16, s2Len+1);
    int * const restrict FL = parasail_memalign_int(16, s2Len+1);
    int i = 0;
    int j = 0;
    int score = NEG_INF_32;
    int matches = NEG_INF_32;
    int similar = NEG_INF_32;
    int length = NEG_INF_32;
    int end_query = s1Len;
    int end_ref = s2Len;

    for (i=0; i<s1Len; ++i) {
        s1[i] = matrix->mapper[(unsigned char)_s1[i]];
    }
    for (j=0; j<s2Len; ++j) {
        s2[j] = matrix->mapper[(unsigned char)_s2[j]];
    }

    /* upper left corner */
    H[0] = 0;
    HM[0] = 0;
    HS[0] = 0;
    HL[0] = 0;
    F[0] = NEG_INF_32;
    FM[0] = 0;
    FS[0] = 0;
    FL[0] = 0;
    
    /* first row */
    for (j=1; j<=s2Len; ++j) {
        H[j] = 0;
        HM[j] = 0;
        HS[j] = 0;
        HL[j] = 0;
        F[j] = NEG_INF_32;
        FM[j] = 0;
        FS[j] = 0;
        FL[j] = 0;
    }

    /* iter over first sequence */
    for (i=1; i<=s1Len; ++i) {
        const int * const restrict matrow = &matrix->matrix[matrix->size*s1[i-1]];
        /* init first column */
        int NH = H[0];
        int NHM = HM[0];
        int NHS = HS[0];
        int NHL = HL[0];
        int WH = 0;
        int WHM = 0;
        int WHS = 0;
        int WHL = 0;
        int E = NEG_INF_32;
        int EM = 0;
        int ES = 0;
        int EL = 0;
        H[0] = WH;
        HM[0] = WHM;
        HS[0] = WHS;
        HL[0] = WHL;
        for (j=1; j<=s2Len; ++j) {
            int H_dag;
            int H_new;
            int E_opn;
            int E_ext;
            int F_opn;
            int F_ext;
            int NWH = NH;
            int NWM = NHM;
            int NWS = NHS;
            int NWL = NHL;
            NH = H[j];
            NHM = HM[j];
            NHS = HS[j];
            NHL = HL[j];
            F_opn = NH - open;
            F_ext  = F[j] - gap;
            F[j]  = MAX(F_opn, F_ext);
            E_opn = WH - open;
            E_ext = E - gap;
            E = MAX(E_opn, E_ext);
            H_dag = NWH + matrow[s2[j-1]];
            H_new = MAX(H_dag, 0);
            H_new = MAX(H_new, E);
            H_new = MAX(H_new, F[j]);
            if (F[j] == F_opn) {
                FM[j] = NHM;
                FS[j] = NHS;
                FL[j] = NHL+1;
            }
            else {
                FL[j] = FL[j]+1;
            }
            if (E == E_opn) {
                EM = WHM;
                ES = WHS;
                EL = WHL+1;
            }
            else {
                EL = EL+1;
            }
            WH = H_new;
            if (H_new == H_dag) {
                WHM  = NWM + (s1[i-1] == s2[j-1]);
                WHS  = NWS + (matrow[s2[j-1]] > 0);
                WHL  = NWL + 1;
            }
            else if (H_new == F[j]) {
                WHM  = FM[j];
                WHS  = FS[j];
                WHL  = FL[j];
            }
            else {
                WHM  = EM;
                WHS  = ES;
                WHL  = EL;
            }
            if (WH <= 0) {
                WH = 0;
                WHM = 0;
                WHS = 0;
                WHL = 0;
            }
            H[j] = WH;
            HM[j] = WHM;
            HS[j] = WHS;
            HL[j] = WHL;
#ifdef PARASAIL_TABLE
            result->score_table[(i-1)*s2Len + (j-1)] = WH;
            result->matches_table[(i-1)*s2Len + (j-1)] = WHM;
            result->similar_table[(i-1)*s2Len + (j-1)] = WHS;
            result->length_table[(i-1)*s2Len + (j-1)] = WHL;
#endif
            if (WH > score) {
                score = WH;
                matches = WHM;
                similar = WHS;
                length = WHL;
                end_query = i-1;
                end_ref = j-1;
            }
            else if (score == WH && j-1 < end_ref) {
                matches = WHM;
                similar = WHS;
                length = WHL;
                end_query = i-1;
                end_ref = j-1;
            }
        }
#ifdef PARASAIL_ROWCOL
        result->score_col[i-1] = WH;
        result->matches_col[i-1] = WHM;
        result->similar_col[i-1] = WHS;
        result->length_col[i-1] = WHL;
#endif
    }
#ifdef PARASAIL_ROWCOL
    for (j=1; j<=s2Len; ++j) {
        result->score_row[j-1] = H[j];
        result->matches_row[j-1] = HM[j];
        result->similar_row[j-1] = HS[j];
        result->length_row[j-1] = HL[j];
    }
#endif

    result->score = score;
    result->matches = matches;
    result->similar = similar;
    result->length = length;
    result->end_query = end_query;
    result->end_ref = end_ref;

    parasail_free(FL);
    parasail_free(FS);
    parasail_free(FM);
    parasail_free(F);
    parasail_free(HL);
    parasail_free(HS);
    parasail_free(HM);
    parasail_free(H);
    parasail_free(s2);
    parasail_free(s1);

    return result;
}

