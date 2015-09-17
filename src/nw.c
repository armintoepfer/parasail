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
#define ENAME parasail_nw_table
#else
#ifdef PARASAIL_ROWCOL
#define ENAME parasail_nw_rowcol
#else
#ifdef PARASAIL_TRACE
#define ENAME parasail_nw_trace
#else
#define ENAME parasail_nw
#endif
#endif
#endif

parasail_result_t* ENAME(
        const char * const restrict _s1, const int s1Len,
        const char * const restrict _s2, const int s2Len,
        const int open, const int gap, const parasail_matrix_t *matrix)
{
#ifdef PARASAIL_TABLE
    parasail_result_t *result = parasail_result_new_table1(s1Len, s2Len);
#else
#ifdef PARASAIL_ROWCOL
    parasail_result_t *result = parasail_result_new_rowcol1(s1Len, s2Len);
#else
#ifdef PARASAIL_TRACE
    parasail_result_t *result = parasail_result_new_trace(s1Len, s2Len);
#else
    parasail_result_t *result = parasail_result_new();
#endif
#endif
#endif
    int * const restrict s1 = parasail_memalign_int(16, s1Len);
    int * const restrict s2 = parasail_memalign_int(16, s2Len);
    int * const restrict tbl_pr = parasail_memalign_int(16, s2Len+1);
    int * const restrict del_pr = parasail_memalign_int(16, s2Len+1);
    int i = 0;
    int j = 0;

    for (i=0; i<s1Len; ++i) {
        s1[i] = matrix->mapper[(unsigned char)_s1[i]];
    }
    for (j=0; j<s2Len; ++j) {
        s2[j] = matrix->mapper[(unsigned char)_s2[j]];
    }

    /* upper left corner */
    tbl_pr[0] = 0;
    del_pr[0] = NEG_INF_32;
    
    /* first row */
    for (j=1; j<=s2Len; ++j) {
        tbl_pr[j] = -open -(j-1)*gap;
        del_pr[j] = NEG_INF_32;
    }

    /* iter over first sequence */
    for (i=1; i<=s1Len; ++i) {
        const int * const restrict matrow = &matrix->matrix[matrix->size*s1[i-1]];
        /* init first column */
        int Nscore = tbl_pr[0];
        int Wscore = -open - (i-1)*gap;
        int ins_cr = NEG_INF_32;
        tbl_pr[0] = Wscore;
        for (j=1; j<=s2Len; ++j) {
            int NWscore = Nscore;
            Nscore = tbl_pr[j];
            del_pr[j] = MAX(Nscore - open, del_pr[j] - gap);
            ins_cr    = MAX(Wscore - open, ins_cr    - gap);
            tbl_pr[j] = NWscore + matrow[s2[j-1]];
            Wscore = tbl_pr[j] = MAX(tbl_pr[j],MAX(ins_cr,del_pr[j]));
#ifdef PARASAIL_TABLE
            result->score_table[(i-1)*s2Len + (j-1)] = Wscore;
#endif
        }
#ifdef PARASAIL_ROWCOL
        result->score_col[i-1] = Wscore;
#endif
    }
#ifdef PARASAIL_ROWCOL
    for (j=1; j<=s2Len; ++j) {
        result->score_row[j-1] = tbl_pr[j];
    }
#endif

    result->score = tbl_pr[s2Len];

    parasail_free(del_pr);
    parasail_free(tbl_pr);
    parasail_free(s2);
    parasail_free(s1);
    
    return result;
}

