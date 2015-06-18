# cython: c_string_type=str, c_string_encoding=ascii

from libc.string cimport const_char

cdef extern from "parasail.h":
    ctypedef struct parasail_result_t:
        int saturated
        int score
        int matches
        int similar
        int length
        #int * score_table
        #int * matches_table
        #int * similar_table
        #int * length_table

    cdef struct parasail_matrix:
        const char * name
        #int * matrix
        #int * mapper
        int size
        int need_free
    ctypedef parasail_matrix parasail_matrix_t

    void parasail_result_free(parasail_result_t * result)

    parasail_matrix_t* parasail_matrix_create(
            const char * alphabet, int match, int mismatch)

    void parasail_matrix_free(parasail_matrix_t *matrix)


    parasail_result_t* parasail_nw(
        const_char * s1, const int s1Len,
        const_char * s2, const int s2Len,
        const int open, const int gap,
        const parasail_matrix_t* matrix);

    parasail_result_t* parasail_nw_table(
        const_char * s1, const int s1Len,
        const_char * s2, const int s2Len,
        const int open, const int gap,
        const parasail_matrix_t* matrix);

    parasail_result_t* parasail_nw_rowcol(
        const_char * s1, const int s1Len,
        const_char * s2, const int s2Len,
        const int open, const int gap,
        const parasail_matrix_t* matrix);

    parasail_result_t* parasail_nw_stats(
        const_char * s1, const int s1Len,
        const_char * s2, const int s2Len,
        const int open, const int gap,
        const parasail_matrix_t* matrix);

    parasail_result_t* parasail_nw_stats_table(
        const_char * s1, const int s1Len,
        const_char * s2, const int s2Len,
        const int open, const int gap,
        const parasail_matrix_t* matrix);

    parasail_result_t* parasail_nw_stats_rowcol(
        const_char * s1, const int s1Len,
        const_char * s2, const int s2Len,
        const int open, const int gap,
        const parasail_matrix_t* matrix);

    parasail_result_t* parasail_sg(
        const_char * s1, const int s1Len,
        const_char * s2, const int s2Len,
        const int open, const int gap,
        const parasail_matrix_t* matrix);

    parasail_result_t* parasail_sg_table(
        const_char * s1, const int s1Len,
        const_char * s2, const int s2Len,
        const int open, const int gap,
        const parasail_matrix_t* matrix);

    parasail_result_t* parasail_sg_rowcol(
        const_char * s1, const int s1Len,
        const_char * s2, const int s2Len,
        const int open, const int gap,
        const parasail_matrix_t* matrix);

    parasail_result_t* parasail_sg_stats(
        const_char * s1, const int s1Len,
        const_char * s2, const int s2Len,
        const int open, const int gap,
        const parasail_matrix_t* matrix);

    parasail_result_t* parasail_sg_stats_table(
        const_char * s1, const int s1Len,
        const_char * s2, const int s2Len,
        const int open, const int gap,
        const parasail_matrix_t* matrix);

    parasail_result_t* parasail_sg_stats_rowcol(
        const_char * s1, const int s1Len,
        const_char * s2, const int s2Len,
        const int open, const int gap,
        const parasail_matrix_t* matrix);

    parasail_result_t* parasail_sw(
        const_char * s1, const int s1Len,
        const_char * s2, const int s2Len,
        const int open, const int gap,
        const parasail_matrix_t* matrix);

    parasail_result_t* parasail_sw_table(
        const_char * s1, const int s1Len,
        const_char * s2, const int s2Len,
        const int open, const int gap,
        const parasail_matrix_t* matrix);

    parasail_result_t* parasail_sw_rowcol(
        const_char * s1, const int s1Len,
        const_char * s2, const int s2Len,
        const int open, const int gap,
        const parasail_matrix_t* matrix);

    parasail_result_t* parasail_sw_stats(
        const_char * s1, const int s1Len,
        const_char * s2, const int s2Len,
        const int open, const int gap,
        const parasail_matrix_t* matrix);

    parasail_result_t* parasail_sw_stats_table(
        const_char * s1, const int s1Len,
        const_char * s2, const int s2Len,
        const int open, const int gap,
        const parasail_matrix_t* matrix);

    parasail_result_t* parasail_sw_stats_rowcol(
        const_char * s1, const int s1Len,
        const_char * s2, const int s2Len,
        const int open, const int gap,
        const parasail_matrix_t* matrix);

    parasail_result_t* parasail_nw_scan(
        const_char * s1, const int s1Len,
        const_char * s2, const int s2Len,
        const int open, const int gap,
        const parasail_matrix_t* matrix);

    parasail_result_t* parasail_nw_table_scan(
        const_char * s1, const int s1Len,
        const_char * s2, const int s2Len,
        const int open, const int gap,
        const parasail_matrix_t* matrix);

    parasail_result_t* parasail_nw_rowcol_scan(
        const_char * s1, const int s1Len,
        const_char * s2, const int s2Len,
        const int open, const int gap,
        const parasail_matrix_t* matrix);

    parasail_result_t* parasail_nw_stats_scan(
        const_char * s1, const int s1Len,
        const_char * s2, const int s2Len,
        const int open, const int gap,
        const parasail_matrix_t* matrix);

    parasail_result_t* parasail_nw_stats_table_scan(
        const_char * s1, const int s1Len,
        const_char * s2, const int s2Len,
        const int open, const int gap,
        const parasail_matrix_t* matrix);

    parasail_result_t* parasail_nw_stats_rowcol_scan(
        const_char * s1, const int s1Len,
        const_char * s2, const int s2Len,
        const int open, const int gap,
        const parasail_matrix_t* matrix);

    parasail_result_t* parasail_sg_scan(
        const_char * s1, const int s1Len,
        const_char * s2, const int s2Len,
        const int open, const int gap,
        const parasail_matrix_t* matrix);

    parasail_result_t* parasail_sg_table_scan(
        const_char * s1, const int s1Len,
        const_char * s2, const int s2Len,
        const int open, const int gap,
        const parasail_matrix_t* matrix);

    parasail_result_t* parasail_sg_rowcol_scan(
        const_char * s1, const int s1Len,
        const_char * s2, const int s2Len,
        const int open, const int gap,
        const parasail_matrix_t* matrix);

    parasail_result_t* parasail_sg_stats_scan(
        const_char * s1, const int s1Len,
        const_char * s2, const int s2Len,
        const int open, const int gap,
        const parasail_matrix_t* matrix);

    parasail_result_t* parasail_sg_stats_table_scan(
        const_char * s1, const int s1Len,
        const_char * s2, const int s2Len,
        const int open, const int gap,
        const parasail_matrix_t* matrix);

    parasail_result_t* parasail_sg_stats_rowcol_scan(
        const_char * s1, const int s1Len,
        const_char * s2, const int s2Len,
        const int open, const int gap,
        const parasail_matrix_t* matrix);

    parasail_result_t* parasail_sw_scan(
        const_char * s1, const int s1Len,
        const_char * s2, const int s2Len,
        const int open, const int gap,
        const parasail_matrix_t* matrix);

    parasail_result_t* parasail_sw_table_scan(
        const_char * s1, const int s1Len,
        const_char * s2, const int s2Len,
        const int open, const int gap,
        const parasail_matrix_t* matrix);

    parasail_result_t* parasail_sw_rowcol_scan(
        const_char * s1, const int s1Len,
        const_char * s2, const int s2Len,
        const int open, const int gap,
        const parasail_matrix_t* matrix);

    parasail_result_t* parasail_sw_stats_scan(
        const_char * s1, const int s1Len,
        const_char * s2, const int s2Len,
        const int open, const int gap,
        const parasail_matrix_t* matrix);

    parasail_result_t* parasail_sw_stats_table_scan(
        const_char * s1, const int s1Len,
        const_char * s2, const int s2Len,
        const int open, const int gap,
        const parasail_matrix_t* matrix);

    parasail_result_t* parasail_sw_stats_rowcol_scan(
        const_char * s1, const int s1Len,
        const_char * s2, const int s2Len,
        const int open, const int gap,
        const parasail_matrix_t* matrix);

    parasail_result_t* parasail_nw_scan_64(
        const_char * s1, const int s1Len,
        const_char * s2, const int s2Len,
        const int open, const int gap,
        const parasail_matrix_t* matrix);

    parasail_result_t* parasail_nw_scan_32(
        const_char * s1, const int s1Len,
        const_char * s2, const int s2Len,
        const int open, const int gap,
        const parasail_matrix_t* matrix);

    parasail_result_t* parasail_nw_scan_16(
        const_char * s1, const int s1Len,
        const_char * s2, const int s2Len,
        const int open, const int gap,
        const parasail_matrix_t* matrix);

    parasail_result_t* parasail_nw_scan_8(
        const_char * s1, const int s1Len,
        const_char * s2, const int s2Len,
        const int open, const int gap,
        const parasail_matrix_t* matrix);

    parasail_result_t* parasail_nw_striped_64(
        const_char * s1, const int s1Len,
        const_char * s2, const int s2Len,
        const int open, const int gap,
        const parasail_matrix_t* matrix);

    parasail_result_t* parasail_nw_striped_32(
        const_char * s1, const int s1Len,
        const_char * s2, const int s2Len,
        const int open, const int gap,
        const parasail_matrix_t* matrix);

    parasail_result_t* parasail_nw_striped_16(
        const_char * s1, const int s1Len,
        const_char * s2, const int s2Len,
        const int open, const int gap,
        const parasail_matrix_t* matrix);

    parasail_result_t* parasail_nw_striped_8(
        const_char * s1, const int s1Len,
        const_char * s2, const int s2Len,
        const int open, const int gap,
        const parasail_matrix_t* matrix);

    parasail_result_t* parasail_nw_diag_64(
        const_char * s1, const int s1Len,
        const_char * s2, const int s2Len,
        const int open, const int gap,
        const parasail_matrix_t* matrix);

    parasail_result_t* parasail_nw_diag_32(
        const_char * s1, const int s1Len,
        const_char * s2, const int s2Len,
        const int open, const int gap,
        const parasail_matrix_t* matrix);

    parasail_result_t* parasail_nw_diag_16(
        const_char * s1, const int s1Len,
        const_char * s2, const int s2Len,
        const int open, const int gap,
        const parasail_matrix_t* matrix);

    parasail_result_t* parasail_nw_diag_8(
        const_char * s1, const int s1Len,
        const_char * s2, const int s2Len,
        const int open, const int gap,
        const parasail_matrix_t* matrix);

    parasail_result_t* parasail_nw_table_scan_64(
        const_char * s1, const int s1Len,
        const_char * s2, const int s2Len,
        const int open, const int gap,
        const parasail_matrix_t* matrix);

    parasail_result_t* parasail_nw_table_scan_32(
        const_char * s1, const int s1Len,
        const_char * s2, const int s2Len,
        const int open, const int gap,
        const parasail_matrix_t* matrix);

    parasail_result_t* parasail_nw_table_scan_16(
        const_char * s1, const int s1Len,
        const_char * s2, const int s2Len,
        const int open, const int gap,
        const parasail_matrix_t* matrix);

    parasail_result_t* parasail_nw_table_scan_8(
        const_char * s1, const int s1Len,
        const_char * s2, const int s2Len,
        const int open, const int gap,
        const parasail_matrix_t* matrix);

    parasail_result_t* parasail_nw_table_striped_64(
        const_char * s1, const int s1Len,
        const_char * s2, const int s2Len,
        const int open, const int gap,
        const parasail_matrix_t* matrix);

    parasail_result_t* parasail_nw_table_striped_32(
        const_char * s1, const int s1Len,
        const_char * s2, const int s2Len,
        const int open, const int gap,
        const parasail_matrix_t* matrix);

    parasail_result_t* parasail_nw_table_striped_16(
        const_char * s1, const int s1Len,
        const_char * s2, const int s2Len,
        const int open, const int gap,
        const parasail_matrix_t* matrix);

    parasail_result_t* parasail_nw_table_striped_8(
        const_char * s1, const int s1Len,
        const_char * s2, const int s2Len,
        const int open, const int gap,
        const parasail_matrix_t* matrix);

    parasail_result_t* parasail_nw_table_diag_64(
        const_char * s1, const int s1Len,
        const_char * s2, const int s2Len,
        const int open, const int gap,
        const parasail_matrix_t* matrix);

    parasail_result_t* parasail_nw_table_diag_32(
        const_char * s1, const int s1Len,
        const_char * s2, const int s2Len,
        const int open, const int gap,
        const parasail_matrix_t* matrix);

    parasail_result_t* parasail_nw_table_diag_16(
        const_char * s1, const int s1Len,
        const_char * s2, const int s2Len,
        const int open, const int gap,
        const parasail_matrix_t* matrix);

    parasail_result_t* parasail_nw_table_diag_8(
        const_char * s1, const int s1Len,
        const_char * s2, const int s2Len,
        const int open, const int gap,
        const parasail_matrix_t* matrix);

    parasail_result_t* parasail_nw_rowcol_scan_64(
        const_char * s1, const int s1Len,
        const_char * s2, const int s2Len,
        const int open, const int gap,
        const parasail_matrix_t* matrix);

    parasail_result_t* parasail_nw_rowcol_scan_32(
        const_char * s1, const int s1Len,
        const_char * s2, const int s2Len,
        const int open, const int gap,
        const parasail_matrix_t* matrix);

    parasail_result_t* parasail_nw_rowcol_scan_16(
        const_char * s1, const int s1Len,
        const_char * s2, const int s2Len,
        const int open, const int gap,
        const parasail_matrix_t* matrix);

    parasail_result_t* parasail_nw_rowcol_scan_8(
        const_char * s1, const int s1Len,
        const_char * s2, const int s2Len,
        const int open, const int gap,
        const parasail_matrix_t* matrix);

    parasail_result_t* parasail_nw_rowcol_striped_64(
        const_char * s1, const int s1Len,
        const_char * s2, const int s2Len,
        const int open, const int gap,
        const parasail_matrix_t* matrix);

    parasail_result_t* parasail_nw_rowcol_striped_32(
        const_char * s1, const int s1Len,
        const_char * s2, const int s2Len,
        const int open, const int gap,
        const parasail_matrix_t* matrix);

    parasail_result_t* parasail_nw_rowcol_striped_16(
        const_char * s1, const int s1Len,
        const_char * s2, const int s2Len,
        const int open, const int gap,
        const parasail_matrix_t* matrix);

    parasail_result_t* parasail_nw_rowcol_striped_8(
        const_char * s1, const int s1Len,
        const_char * s2, const int s2Len,
        const int open, const int gap,
        const parasail_matrix_t* matrix);

    parasail_result_t* parasail_nw_rowcol_diag_64(
        const_char * s1, const int s1Len,
        const_char * s2, const int s2Len,
        const int open, const int gap,
        const parasail_matrix_t* matrix);

    parasail_result_t* parasail_nw_rowcol_diag_32(
        const_char * s1, const int s1Len,
        const_char * s2, const int s2Len,
        const int open, const int gap,
        const parasail_matrix_t* matrix);

    parasail_result_t* parasail_nw_rowcol_diag_16(
        const_char * s1, const int s1Len,
        const_char * s2, const int s2Len,
        const int open, const int gap,
        const parasail_matrix_t* matrix);

    parasail_result_t* parasail_nw_rowcol_diag_8(
        const_char * s1, const int s1Len,
        const_char * s2, const int s2Len,
        const int open, const int gap,
        const parasail_matrix_t* matrix);

    parasail_result_t* parasail_nw_stats_scan_64(
        const_char * s1, const int s1Len,
        const_char * s2, const int s2Len,
        const int open, const int gap,
        const parasail_matrix_t* matrix);

    parasail_result_t* parasail_nw_stats_scan_32(
        const_char * s1, const int s1Len,
        const_char * s2, const int s2Len,
        const int open, const int gap,
        const parasail_matrix_t* matrix);

    parasail_result_t* parasail_nw_stats_scan_16(
        const_char * s1, const int s1Len,
        const_char * s2, const int s2Len,
        const int open, const int gap,
        const parasail_matrix_t* matrix);

    parasail_result_t* parasail_nw_stats_scan_8(
        const_char * s1, const int s1Len,
        const_char * s2, const int s2Len,
        const int open, const int gap,
        const parasail_matrix_t* matrix);

    parasail_result_t* parasail_nw_stats_striped_64(
        const_char * s1, const int s1Len,
        const_char * s2, const int s2Len,
        const int open, const int gap,
        const parasail_matrix_t* matrix);

    parasail_result_t* parasail_nw_stats_striped_32(
        const_char * s1, const int s1Len,
        const_char * s2, const int s2Len,
        const int open, const int gap,
        const parasail_matrix_t* matrix);

    parasail_result_t* parasail_nw_stats_striped_16(
        const_char * s1, const int s1Len,
        const_char * s2, const int s2Len,
        const int open, const int gap,
        const parasail_matrix_t* matrix);

    parasail_result_t* parasail_nw_stats_striped_8(
        const_char * s1, const int s1Len,
        const_char * s2, const int s2Len,
        const int open, const int gap,
        const parasail_matrix_t* matrix);

    parasail_result_t* parasail_nw_stats_diag_64(
        const_char * s1, const int s1Len,
        const_char * s2, const int s2Len,
        const int open, const int gap,
        const parasail_matrix_t* matrix);

    parasail_result_t* parasail_nw_stats_diag_32(
        const_char * s1, const int s1Len,
        const_char * s2, const int s2Len,
        const int open, const int gap,
        const parasail_matrix_t* matrix);

    parasail_result_t* parasail_nw_stats_diag_16(
        const_char * s1, const int s1Len,
        const_char * s2, const int s2Len,
        const int open, const int gap,
        const parasail_matrix_t* matrix);

    parasail_result_t* parasail_nw_stats_diag_8(
        const_char * s1, const int s1Len,
        const_char * s2, const int s2Len,
        const int open, const int gap,
        const parasail_matrix_t* matrix);

    parasail_result_t* parasail_nw_stats_table_scan_64(
        const_char * s1, const int s1Len,
        const_char * s2, const int s2Len,
        const int open, const int gap,
        const parasail_matrix_t* matrix);

    parasail_result_t* parasail_nw_stats_table_scan_32(
        const_char * s1, const int s1Len,
        const_char * s2, const int s2Len,
        const int open, const int gap,
        const parasail_matrix_t* matrix);

    parasail_result_t* parasail_nw_stats_table_scan_16(
        const_char * s1, const int s1Len,
        const_char * s2, const int s2Len,
        const int open, const int gap,
        const parasail_matrix_t* matrix);

    parasail_result_t* parasail_nw_stats_table_scan_8(
        const_char * s1, const int s1Len,
        const_char * s2, const int s2Len,
        const int open, const int gap,
        const parasail_matrix_t* matrix);

    parasail_result_t* parasail_nw_stats_table_striped_64(
        const_char * s1, const int s1Len,
        const_char * s2, const int s2Len,
        const int open, const int gap,
        const parasail_matrix_t* matrix);

    parasail_result_t* parasail_nw_stats_table_striped_32(
        const_char * s1, const int s1Len,
        const_char * s2, const int s2Len,
        const int open, const int gap,
        const parasail_matrix_t* matrix);

    parasail_result_t* parasail_nw_stats_table_striped_16(
        const_char * s1, const int s1Len,
        const_char * s2, const int s2Len,
        const int open, const int gap,
        const parasail_matrix_t* matrix);

    parasail_result_t* parasail_nw_stats_table_striped_8(
        const_char * s1, const int s1Len,
        const_char * s2, const int s2Len,
        const int open, const int gap,
        const parasail_matrix_t* matrix);

    parasail_result_t* parasail_nw_stats_table_diag_64(
        const_char * s1, const int s1Len,
        const_char * s2, const int s2Len,
        const int open, const int gap,
        const parasail_matrix_t* matrix);

    parasail_result_t* parasail_nw_stats_table_diag_32(
        const_char * s1, const int s1Len,
        const_char * s2, const int s2Len,
        const int open, const int gap,
        const parasail_matrix_t* matrix);

    parasail_result_t* parasail_nw_stats_table_diag_16(
        const_char * s1, const int s1Len,
        const_char * s2, const int s2Len,
        const int open, const int gap,
        const parasail_matrix_t* matrix);

    parasail_result_t* parasail_nw_stats_table_diag_8(
        const_char * s1, const int s1Len,
        const_char * s2, const int s2Len,
        const int open, const int gap,
        const parasail_matrix_t* matrix);

    parasail_result_t* parasail_nw_stats_rowcol_scan_64(
        const_char * s1, const int s1Len,
        const_char * s2, const int s2Len,
        const int open, const int gap,
        const parasail_matrix_t* matrix);

    parasail_result_t* parasail_nw_stats_rowcol_scan_32(
        const_char * s1, const int s1Len,
        const_char * s2, const int s2Len,
        const int open, const int gap,
        const parasail_matrix_t* matrix);

    parasail_result_t* parasail_nw_stats_rowcol_scan_16(
        const_char * s1, const int s1Len,
        const_char * s2, const int s2Len,
        const int open, const int gap,
        const parasail_matrix_t* matrix);

    parasail_result_t* parasail_nw_stats_rowcol_scan_8(
        const_char * s1, const int s1Len,
        const_char * s2, const int s2Len,
        const int open, const int gap,
        const parasail_matrix_t* matrix);

    parasail_result_t* parasail_nw_stats_rowcol_striped_64(
        const_char * s1, const int s1Len,
        const_char * s2, const int s2Len,
        const int open, const int gap,
        const parasail_matrix_t* matrix);

    parasail_result_t* parasail_nw_stats_rowcol_striped_32(
        const_char * s1, const int s1Len,
        const_char * s2, const int s2Len,
        const int open, const int gap,
        const parasail_matrix_t* matrix);

    parasail_result_t* parasail_nw_stats_rowcol_striped_16(
        const_char * s1, const int s1Len,
        const_char * s2, const int s2Len,
        const int open, const int gap,
        const parasail_matrix_t* matrix);

    parasail_result_t* parasail_nw_stats_rowcol_striped_8(
        const_char * s1, const int s1Len,
        const_char * s2, const int s2Len,
        const int open, const int gap,
        const parasail_matrix_t* matrix);

    parasail_result_t* parasail_nw_stats_rowcol_diag_64(
        const_char * s1, const int s1Len,
        const_char * s2, const int s2Len,
        const int open, const int gap,
        const parasail_matrix_t* matrix);

    parasail_result_t* parasail_nw_stats_rowcol_diag_32(
        const_char * s1, const int s1Len,
        const_char * s2, const int s2Len,
        const int open, const int gap,
        const parasail_matrix_t* matrix);

    parasail_result_t* parasail_nw_stats_rowcol_diag_16(
        const_char * s1, const int s1Len,
        const_char * s2, const int s2Len,
        const int open, const int gap,
        const parasail_matrix_t* matrix);

    parasail_result_t* parasail_nw_stats_rowcol_diag_8(
        const_char * s1, const int s1Len,
        const_char * s2, const int s2Len,
        const int open, const int gap,
        const parasail_matrix_t* matrix);

    parasail_result_t* parasail_sg_scan_64(
        const_char * s1, const int s1Len,
        const_char * s2, const int s2Len,
        const int open, const int gap,
        const parasail_matrix_t* matrix);

    parasail_result_t* parasail_sg_scan_32(
        const_char * s1, const int s1Len,
        const_char * s2, const int s2Len,
        const int open, const int gap,
        const parasail_matrix_t* matrix);

    parasail_result_t* parasail_sg_scan_16(
        const_char * s1, const int s1Len,
        const_char * s2, const int s2Len,
        const int open, const int gap,
        const parasail_matrix_t* matrix);

    parasail_result_t* parasail_sg_scan_8(
        const_char * s1, const int s1Len,
        const_char * s2, const int s2Len,
        const int open, const int gap,
        const parasail_matrix_t* matrix);

    parasail_result_t* parasail_sg_striped_64(
        const_char * s1, const int s1Len,
        const_char * s2, const int s2Len,
        const int open, const int gap,
        const parasail_matrix_t* matrix);

    parasail_result_t* parasail_sg_striped_32(
        const_char * s1, const int s1Len,
        const_char * s2, const int s2Len,
        const int open, const int gap,
        const parasail_matrix_t* matrix);

    parasail_result_t* parasail_sg_striped_16(
        const_char * s1, const int s1Len,
        const_char * s2, const int s2Len,
        const int open, const int gap,
        const parasail_matrix_t* matrix);

    parasail_result_t* parasail_sg_striped_8(
        const_char * s1, const int s1Len,
        const_char * s2, const int s2Len,
        const int open, const int gap,
        const parasail_matrix_t* matrix);

    parasail_result_t* parasail_sg_diag_64(
        const_char * s1, const int s1Len,
        const_char * s2, const int s2Len,
        const int open, const int gap,
        const parasail_matrix_t* matrix);

    parasail_result_t* parasail_sg_diag_32(
        const_char * s1, const int s1Len,
        const_char * s2, const int s2Len,
        const int open, const int gap,
        const parasail_matrix_t* matrix);

    parasail_result_t* parasail_sg_diag_16(
        const_char * s1, const int s1Len,
        const_char * s2, const int s2Len,
        const int open, const int gap,
        const parasail_matrix_t* matrix);

    parasail_result_t* parasail_sg_diag_8(
        const_char * s1, const int s1Len,
        const_char * s2, const int s2Len,
        const int open, const int gap,
        const parasail_matrix_t* matrix);

    parasail_result_t* parasail_sg_table_scan_64(
        const_char * s1, const int s1Len,
        const_char * s2, const int s2Len,
        const int open, const int gap,
        const parasail_matrix_t* matrix);

    parasail_result_t* parasail_sg_table_scan_32(
        const_char * s1, const int s1Len,
        const_char * s2, const int s2Len,
        const int open, const int gap,
        const parasail_matrix_t* matrix);

    parasail_result_t* parasail_sg_table_scan_16(
        const_char * s1, const int s1Len,
        const_char * s2, const int s2Len,
        const int open, const int gap,
        const parasail_matrix_t* matrix);

    parasail_result_t* parasail_sg_table_scan_8(
        const_char * s1, const int s1Len,
        const_char * s2, const int s2Len,
        const int open, const int gap,
        const parasail_matrix_t* matrix);

    parasail_result_t* parasail_sg_table_striped_64(
        const_char * s1, const int s1Len,
        const_char * s2, const int s2Len,
        const int open, const int gap,
        const parasail_matrix_t* matrix);

    parasail_result_t* parasail_sg_table_striped_32(
        const_char * s1, const int s1Len,
        const_char * s2, const int s2Len,
        const int open, const int gap,
        const parasail_matrix_t* matrix);

    parasail_result_t* parasail_sg_table_striped_16(
        const_char * s1, const int s1Len,
        const_char * s2, const int s2Len,
        const int open, const int gap,
        const parasail_matrix_t* matrix);

    parasail_result_t* parasail_sg_table_striped_8(
        const_char * s1, const int s1Len,
        const_char * s2, const int s2Len,
        const int open, const int gap,
        const parasail_matrix_t* matrix);

    parasail_result_t* parasail_sg_table_diag_64(
        const_char * s1, const int s1Len,
        const_char * s2, const int s2Len,
        const int open, const int gap,
        const parasail_matrix_t* matrix);

    parasail_result_t* parasail_sg_table_diag_32(
        const_char * s1, const int s1Len,
        const_char * s2, const int s2Len,
        const int open, const int gap,
        const parasail_matrix_t* matrix);

    parasail_result_t* parasail_sg_table_diag_16(
        const_char * s1, const int s1Len,
        const_char * s2, const int s2Len,
        const int open, const int gap,
        const parasail_matrix_t* matrix);

    parasail_result_t* parasail_sg_table_diag_8(
        const_char * s1, const int s1Len,
        const_char * s2, const int s2Len,
        const int open, const int gap,
        const parasail_matrix_t* matrix);

    parasail_result_t* parasail_sg_rowcol_scan_64(
        const_char * s1, const int s1Len,
        const_char * s2, const int s2Len,
        const int open, const int gap,
        const parasail_matrix_t* matrix);

    parasail_result_t* parasail_sg_rowcol_scan_32(
        const_char * s1, const int s1Len,
        const_char * s2, const int s2Len,
        const int open, const int gap,
        const parasail_matrix_t* matrix);

    parasail_result_t* parasail_sg_rowcol_scan_16(
        const_char * s1, const int s1Len,
        const_char * s2, const int s2Len,
        const int open, const int gap,
        const parasail_matrix_t* matrix);

    parasail_result_t* parasail_sg_rowcol_scan_8(
        const_char * s1, const int s1Len,
        const_char * s2, const int s2Len,
        const int open, const int gap,
        const parasail_matrix_t* matrix);

    parasail_result_t* parasail_sg_rowcol_striped_64(
        const_char * s1, const int s1Len,
        const_char * s2, const int s2Len,
        const int open, const int gap,
        const parasail_matrix_t* matrix);

    parasail_result_t* parasail_sg_rowcol_striped_32(
        const_char * s1, const int s1Len,
        const_char * s2, const int s2Len,
        const int open, const int gap,
        const parasail_matrix_t* matrix);

    parasail_result_t* parasail_sg_rowcol_striped_16(
        const_char * s1, const int s1Len,
        const_char * s2, const int s2Len,
        const int open, const int gap,
        const parasail_matrix_t* matrix);

    parasail_result_t* parasail_sg_rowcol_striped_8(
        const_char * s1, const int s1Len,
        const_char * s2, const int s2Len,
        const int open, const int gap,
        const parasail_matrix_t* matrix);

    parasail_result_t* parasail_sg_rowcol_diag_64(
        const_char * s1, const int s1Len,
        const_char * s2, const int s2Len,
        const int open, const int gap,
        const parasail_matrix_t* matrix);

    parasail_result_t* parasail_sg_rowcol_diag_32(
        const_char * s1, const int s1Len,
        const_char * s2, const int s2Len,
        const int open, const int gap,
        const parasail_matrix_t* matrix);

    parasail_result_t* parasail_sg_rowcol_diag_16(
        const_char * s1, const int s1Len,
        const_char * s2, const int s2Len,
        const int open, const int gap,
        const parasail_matrix_t* matrix);

    parasail_result_t* parasail_sg_rowcol_diag_8(
        const_char * s1, const int s1Len,
        const_char * s2, const int s2Len,
        const int open, const int gap,
        const parasail_matrix_t* matrix);

    parasail_result_t* parasail_sg_stats_scan_64(
        const_char * s1, const int s1Len,
        const_char * s2, const int s2Len,
        const int open, const int gap,
        const parasail_matrix_t* matrix);

    parasail_result_t* parasail_sg_stats_scan_32(
        const_char * s1, const int s1Len,
        const_char * s2, const int s2Len,
        const int open, const int gap,
        const parasail_matrix_t* matrix);

    parasail_result_t* parasail_sg_stats_scan_16(
        const_char * s1, const int s1Len,
        const_char * s2, const int s2Len,
        const int open, const int gap,
        const parasail_matrix_t* matrix);

    parasail_result_t* parasail_sg_stats_scan_8(
        const_char * s1, const int s1Len,
        const_char * s2, const int s2Len,
        const int open, const int gap,
        const parasail_matrix_t* matrix);

    parasail_result_t* parasail_sg_stats_striped_64(
        const_char * s1, const int s1Len,
        const_char * s2, const int s2Len,
        const int open, const int gap,
        const parasail_matrix_t* matrix);

    parasail_result_t* parasail_sg_stats_striped_32(
        const_char * s1, const int s1Len,
        const_char * s2, const int s2Len,
        const int open, const int gap,
        const parasail_matrix_t* matrix);

    parasail_result_t* parasail_sg_stats_striped_16(
        const_char * s1, const int s1Len,
        const_char * s2, const int s2Len,
        const int open, const int gap,
        const parasail_matrix_t* matrix);

    parasail_result_t* parasail_sg_stats_striped_8(
        const_char * s1, const int s1Len,
        const_char * s2, const int s2Len,
        const int open, const int gap,
        const parasail_matrix_t* matrix);

    parasail_result_t* parasail_sg_stats_diag_64(
        const_char * s1, const int s1Len,
        const_char * s2, const int s2Len,
        const int open, const int gap,
        const parasail_matrix_t* matrix);

    parasail_result_t* parasail_sg_stats_diag_32(
        const_char * s1, const int s1Len,
        const_char * s2, const int s2Len,
        const int open, const int gap,
        const parasail_matrix_t* matrix);

    parasail_result_t* parasail_sg_stats_diag_16(
        const_char * s1, const int s1Len,
        const_char * s2, const int s2Len,
        const int open, const int gap,
        const parasail_matrix_t* matrix);

    parasail_result_t* parasail_sg_stats_diag_8(
        const_char * s1, const int s1Len,
        const_char * s2, const int s2Len,
        const int open, const int gap,
        const parasail_matrix_t* matrix);

    parasail_result_t* parasail_sg_stats_table_scan_64(
        const_char * s1, const int s1Len,
        const_char * s2, const int s2Len,
        const int open, const int gap,
        const parasail_matrix_t* matrix);

    parasail_result_t* parasail_sg_stats_table_scan_32(
        const_char * s1, const int s1Len,
        const_char * s2, const int s2Len,
        const int open, const int gap,
        const parasail_matrix_t* matrix);

    parasail_result_t* parasail_sg_stats_table_scan_16(
        const_char * s1, const int s1Len,
        const_char * s2, const int s2Len,
        const int open, const int gap,
        const parasail_matrix_t* matrix);

    parasail_result_t* parasail_sg_stats_table_scan_8(
        const_char * s1, const int s1Len,
        const_char * s2, const int s2Len,
        const int open, const int gap,
        const parasail_matrix_t* matrix);

    parasail_result_t* parasail_sg_stats_table_striped_64(
        const_char * s1, const int s1Len,
        const_char * s2, const int s2Len,
        const int open, const int gap,
        const parasail_matrix_t* matrix);

    parasail_result_t* parasail_sg_stats_table_striped_32(
        const_char * s1, const int s1Len,
        const_char * s2, const int s2Len,
        const int open, const int gap,
        const parasail_matrix_t* matrix);

    parasail_result_t* parasail_sg_stats_table_striped_16(
        const_char * s1, const int s1Len,
        const_char * s2, const int s2Len,
        const int open, const int gap,
        const parasail_matrix_t* matrix);

    parasail_result_t* parasail_sg_stats_table_striped_8(
        const_char * s1, const int s1Len,
        const_char * s2, const int s2Len,
        const int open, const int gap,
        const parasail_matrix_t* matrix);

    parasail_result_t* parasail_sg_stats_table_diag_64(
        const_char * s1, const int s1Len,
        const_char * s2, const int s2Len,
        const int open, const int gap,
        const parasail_matrix_t* matrix);

    parasail_result_t* parasail_sg_stats_table_diag_32(
        const_char * s1, const int s1Len,
        const_char * s2, const int s2Len,
        const int open, const int gap,
        const parasail_matrix_t* matrix);

    parasail_result_t* parasail_sg_stats_table_diag_16(
        const_char * s1, const int s1Len,
        const_char * s2, const int s2Len,
        const int open, const int gap,
        const parasail_matrix_t* matrix);

    parasail_result_t* parasail_sg_stats_table_diag_8(
        const_char * s1, const int s1Len,
        const_char * s2, const int s2Len,
        const int open, const int gap,
        const parasail_matrix_t* matrix);

    parasail_result_t* parasail_sg_stats_rowcol_scan_64(
        const_char * s1, const int s1Len,
        const_char * s2, const int s2Len,
        const int open, const int gap,
        const parasail_matrix_t* matrix);

    parasail_result_t* parasail_sg_stats_rowcol_scan_32(
        const_char * s1, const int s1Len,
        const_char * s2, const int s2Len,
        const int open, const int gap,
        const parasail_matrix_t* matrix);

    parasail_result_t* parasail_sg_stats_rowcol_scan_16(
        const_char * s1, const int s1Len,
        const_char * s2, const int s2Len,
        const int open, const int gap,
        const parasail_matrix_t* matrix);

    parasail_result_t* parasail_sg_stats_rowcol_scan_8(
        const_char * s1, const int s1Len,
        const_char * s2, const int s2Len,
        const int open, const int gap,
        const parasail_matrix_t* matrix);

    parasail_result_t* parasail_sg_stats_rowcol_striped_64(
        const_char * s1, const int s1Len,
        const_char * s2, const int s2Len,
        const int open, const int gap,
        const parasail_matrix_t* matrix);

    parasail_result_t* parasail_sg_stats_rowcol_striped_32(
        const_char * s1, const int s1Len,
        const_char * s2, const int s2Len,
        const int open, const int gap,
        const parasail_matrix_t* matrix);

    parasail_result_t* parasail_sg_stats_rowcol_striped_16(
        const_char * s1, const int s1Len,
        const_char * s2, const int s2Len,
        const int open, const int gap,
        const parasail_matrix_t* matrix);

    parasail_result_t* parasail_sg_stats_rowcol_striped_8(
        const_char * s1, const int s1Len,
        const_char * s2, const int s2Len,
        const int open, const int gap,
        const parasail_matrix_t* matrix);

    parasail_result_t* parasail_sg_stats_rowcol_diag_64(
        const_char * s1, const int s1Len,
        const_char * s2, const int s2Len,
        const int open, const int gap,
        const parasail_matrix_t* matrix);

    parasail_result_t* parasail_sg_stats_rowcol_diag_32(
        const_char * s1, const int s1Len,
        const_char * s2, const int s2Len,
        const int open, const int gap,
        const parasail_matrix_t* matrix);

    parasail_result_t* parasail_sg_stats_rowcol_diag_16(
        const_char * s1, const int s1Len,
        const_char * s2, const int s2Len,
        const int open, const int gap,
        const parasail_matrix_t* matrix);

    parasail_result_t* parasail_sg_stats_rowcol_diag_8(
        const_char * s1, const int s1Len,
        const_char * s2, const int s2Len,
        const int open, const int gap,
        const parasail_matrix_t* matrix);

    parasail_result_t* parasail_sw_scan_64(
        const_char * s1, const int s1Len,
        const_char * s2, const int s2Len,
        const int open, const int gap,
        const parasail_matrix_t* matrix);

    parasail_result_t* parasail_sw_scan_32(
        const_char * s1, const int s1Len,
        const_char * s2, const int s2Len,
        const int open, const int gap,
        const parasail_matrix_t* matrix);

    parasail_result_t* parasail_sw_scan_16(
        const_char * s1, const int s1Len,
        const_char * s2, const int s2Len,
        const int open, const int gap,
        const parasail_matrix_t* matrix);

    parasail_result_t* parasail_sw_scan_8(
        const_char * s1, const int s1Len,
        const_char * s2, const int s2Len,
        const int open, const int gap,
        const parasail_matrix_t* matrix);

    parasail_result_t* parasail_sw_striped_64(
        const_char * s1, const int s1Len,
        const_char * s2, const int s2Len,
        const int open, const int gap,
        const parasail_matrix_t* matrix);

    parasail_result_t* parasail_sw_striped_32(
        const_char * s1, const int s1Len,
        const_char * s2, const int s2Len,
        const int open, const int gap,
        const parasail_matrix_t* matrix);

    parasail_result_t* parasail_sw_striped_16(
        const_char * s1, const int s1Len,
        const_char * s2, const int s2Len,
        const int open, const int gap,
        const parasail_matrix_t* matrix);

    parasail_result_t* parasail_sw_striped_8(
        const_char * s1, const int s1Len,
        const_char * s2, const int s2Len,
        const int open, const int gap,
        const parasail_matrix_t* matrix);

    parasail_result_t* parasail_sw_diag_64(
        const_char * s1, const int s1Len,
        const_char * s2, const int s2Len,
        const int open, const int gap,
        const parasail_matrix_t* matrix);

    parasail_result_t* parasail_sw_diag_32(
        const_char * s1, const int s1Len,
        const_char * s2, const int s2Len,
        const int open, const int gap,
        const parasail_matrix_t* matrix);

    parasail_result_t* parasail_sw_diag_16(
        const_char * s1, const int s1Len,
        const_char * s2, const int s2Len,
        const int open, const int gap,
        const parasail_matrix_t* matrix);

    parasail_result_t* parasail_sw_diag_8(
        const_char * s1, const int s1Len,
        const_char * s2, const int s2Len,
        const int open, const int gap,
        const parasail_matrix_t* matrix);

    parasail_result_t* parasail_sw_table_scan_64(
        const_char * s1, const int s1Len,
        const_char * s2, const int s2Len,
        const int open, const int gap,
        const parasail_matrix_t* matrix);

    parasail_result_t* parasail_sw_table_scan_32(
        const_char * s1, const int s1Len,
        const_char * s2, const int s2Len,
        const int open, const int gap,
        const parasail_matrix_t* matrix);

    parasail_result_t* parasail_sw_table_scan_16(
        const_char * s1, const int s1Len,
        const_char * s2, const int s2Len,
        const int open, const int gap,
        const parasail_matrix_t* matrix);

    parasail_result_t* parasail_sw_table_scan_8(
        const_char * s1, const int s1Len,
        const_char * s2, const int s2Len,
        const int open, const int gap,
        const parasail_matrix_t* matrix);

    parasail_result_t* parasail_sw_table_striped_64(
        const_char * s1, const int s1Len,
        const_char * s2, const int s2Len,
        const int open, const int gap,
        const parasail_matrix_t* matrix);

    parasail_result_t* parasail_sw_table_striped_32(
        const_char * s1, const int s1Len,
        const_char * s2, const int s2Len,
        const int open, const int gap,
        const parasail_matrix_t* matrix);

    parasail_result_t* parasail_sw_table_striped_16(
        const_char * s1, const int s1Len,
        const_char * s2, const int s2Len,
        const int open, const int gap,
        const parasail_matrix_t* matrix);

    parasail_result_t* parasail_sw_table_striped_8(
        const_char * s1, const int s1Len,
        const_char * s2, const int s2Len,
        const int open, const int gap,
        const parasail_matrix_t* matrix);

    parasail_result_t* parasail_sw_table_diag_64(
        const_char * s1, const int s1Len,
        const_char * s2, const int s2Len,
        const int open, const int gap,
        const parasail_matrix_t* matrix);

    parasail_result_t* parasail_sw_table_diag_32(
        const_char * s1, const int s1Len,
        const_char * s2, const int s2Len,
        const int open, const int gap,
        const parasail_matrix_t* matrix);

    parasail_result_t* parasail_sw_table_diag_16(
        const_char * s1, const int s1Len,
        const_char * s2, const int s2Len,
        const int open, const int gap,
        const parasail_matrix_t* matrix);

    parasail_result_t* parasail_sw_table_diag_8(
        const_char * s1, const int s1Len,
        const_char * s2, const int s2Len,
        const int open, const int gap,
        const parasail_matrix_t* matrix);

    parasail_result_t* parasail_sw_rowcol_scan_64(
        const_char * s1, const int s1Len,
        const_char * s2, const int s2Len,
        const int open, const int gap,
        const parasail_matrix_t* matrix);

    parasail_result_t* parasail_sw_rowcol_scan_32(
        const_char * s1, const int s1Len,
        const_char * s2, const int s2Len,
        const int open, const int gap,
        const parasail_matrix_t* matrix);

    parasail_result_t* parasail_sw_rowcol_scan_16(
        const_char * s1, const int s1Len,
        const_char * s2, const int s2Len,
        const int open, const int gap,
        const parasail_matrix_t* matrix);

    parasail_result_t* parasail_sw_rowcol_scan_8(
        const_char * s1, const int s1Len,
        const_char * s2, const int s2Len,
        const int open, const int gap,
        const parasail_matrix_t* matrix);

    parasail_result_t* parasail_sw_rowcol_striped_64(
        const_char * s1, const int s1Len,
        const_char * s2, const int s2Len,
        const int open, const int gap,
        const parasail_matrix_t* matrix);

    parasail_result_t* parasail_sw_rowcol_striped_32(
        const_char * s1, const int s1Len,
        const_char * s2, const int s2Len,
        const int open, const int gap,
        const parasail_matrix_t* matrix);

    parasail_result_t* parasail_sw_rowcol_striped_16(
        const_char * s1, const int s1Len,
        const_char * s2, const int s2Len,
        const int open, const int gap,
        const parasail_matrix_t* matrix);

    parasail_result_t* parasail_sw_rowcol_striped_8(
        const_char * s1, const int s1Len,
        const_char * s2, const int s2Len,
        const int open, const int gap,
        const parasail_matrix_t* matrix);

    parasail_result_t* parasail_sw_rowcol_diag_64(
        const_char * s1, const int s1Len,
        const_char * s2, const int s2Len,
        const int open, const int gap,
        const parasail_matrix_t* matrix);

    parasail_result_t* parasail_sw_rowcol_diag_32(
        const_char * s1, const int s1Len,
        const_char * s2, const int s2Len,
        const int open, const int gap,
        const parasail_matrix_t* matrix);

    parasail_result_t* parasail_sw_rowcol_diag_16(
        const_char * s1, const int s1Len,
        const_char * s2, const int s2Len,
        const int open, const int gap,
        const parasail_matrix_t* matrix);

    parasail_result_t* parasail_sw_rowcol_diag_8(
        const_char * s1, const int s1Len,
        const_char * s2, const int s2Len,
        const int open, const int gap,
        const parasail_matrix_t* matrix);

    parasail_result_t* parasail_sw_stats_scan_64(
        const_char * s1, const int s1Len,
        const_char * s2, const int s2Len,
        const int open, const int gap,
        const parasail_matrix_t* matrix);

    parasail_result_t* parasail_sw_stats_scan_32(
        const_char * s1, const int s1Len,
        const_char * s2, const int s2Len,
        const int open, const int gap,
        const parasail_matrix_t* matrix);

    parasail_result_t* parasail_sw_stats_scan_16(
        const_char * s1, const int s1Len,
        const_char * s2, const int s2Len,
        const int open, const int gap,
        const parasail_matrix_t* matrix);

    parasail_result_t* parasail_sw_stats_scan_8(
        const_char * s1, const int s1Len,
        const_char * s2, const int s2Len,
        const int open, const int gap,
        const parasail_matrix_t* matrix);

    parasail_result_t* parasail_sw_stats_striped_64(
        const_char * s1, const int s1Len,
        const_char * s2, const int s2Len,
        const int open, const int gap,
        const parasail_matrix_t* matrix);

    parasail_result_t* parasail_sw_stats_striped_32(
        const_char * s1, const int s1Len,
        const_char * s2, const int s2Len,
        const int open, const int gap,
        const parasail_matrix_t* matrix);

    parasail_result_t* parasail_sw_stats_striped_16(
        const_char * s1, const int s1Len,
        const_char * s2, const int s2Len,
        const int open, const int gap,
        const parasail_matrix_t* matrix);

    parasail_result_t* parasail_sw_stats_striped_8(
        const_char * s1, const int s1Len,
        const_char * s2, const int s2Len,
        const int open, const int gap,
        const parasail_matrix_t* matrix);

    parasail_result_t* parasail_sw_stats_diag_64(
        const_char * s1, const int s1Len,
        const_char * s2, const int s2Len,
        const int open, const int gap,
        const parasail_matrix_t* matrix);

    parasail_result_t* parasail_sw_stats_diag_32(
        const_char * s1, const int s1Len,
        const_char * s2, const int s2Len,
        const int open, const int gap,
        const parasail_matrix_t* matrix);

    parasail_result_t* parasail_sw_stats_diag_16(
        const_char * s1, const int s1Len,
        const_char * s2, const int s2Len,
        const int open, const int gap,
        const parasail_matrix_t* matrix);

    parasail_result_t* parasail_sw_stats_diag_8(
        const_char * s1, const int s1Len,
        const_char * s2, const int s2Len,
        const int open, const int gap,
        const parasail_matrix_t* matrix);

    parasail_result_t* parasail_sw_stats_table_scan_64(
        const_char * s1, const int s1Len,
        const_char * s2, const int s2Len,
        const int open, const int gap,
        const parasail_matrix_t* matrix);

    parasail_result_t* parasail_sw_stats_table_scan_32(
        const_char * s1, const int s1Len,
        const_char * s2, const int s2Len,
        const int open, const int gap,
        const parasail_matrix_t* matrix);

    parasail_result_t* parasail_sw_stats_table_scan_16(
        const_char * s1, const int s1Len,
        const_char * s2, const int s2Len,
        const int open, const int gap,
        const parasail_matrix_t* matrix);

    parasail_result_t* parasail_sw_stats_table_scan_8(
        const_char * s1, const int s1Len,
        const_char * s2, const int s2Len,
        const int open, const int gap,
        const parasail_matrix_t* matrix);

    parasail_result_t* parasail_sw_stats_table_striped_64(
        const_char * s1, const int s1Len,
        const_char * s2, const int s2Len,
        const int open, const int gap,
        const parasail_matrix_t* matrix);

    parasail_result_t* parasail_sw_stats_table_striped_32(
        const_char * s1, const int s1Len,
        const_char * s2, const int s2Len,
        const int open, const int gap,
        const parasail_matrix_t* matrix);

    parasail_result_t* parasail_sw_stats_table_striped_16(
        const_char * s1, const int s1Len,
        const_char * s2, const int s2Len,
        const int open, const int gap,
        const parasail_matrix_t* matrix);

    parasail_result_t* parasail_sw_stats_table_striped_8(
        const_char * s1, const int s1Len,
        const_char * s2, const int s2Len,
        const int open, const int gap,
        const parasail_matrix_t* matrix);

    parasail_result_t* parasail_sw_stats_table_diag_64(
        const_char * s1, const int s1Len,
        const_char * s2, const int s2Len,
        const int open, const int gap,
        const parasail_matrix_t* matrix);

    parasail_result_t* parasail_sw_stats_table_diag_32(
        const_char * s1, const int s1Len,
        const_char * s2, const int s2Len,
        const int open, const int gap,
        const parasail_matrix_t* matrix);

    parasail_result_t* parasail_sw_stats_table_diag_16(
        const_char * s1, const int s1Len,
        const_char * s2, const int s2Len,
        const int open, const int gap,
        const parasail_matrix_t* matrix);

    parasail_result_t* parasail_sw_stats_table_diag_8(
        const_char * s1, const int s1Len,
        const_char * s2, const int s2Len,
        const int open, const int gap,
        const parasail_matrix_t* matrix);

    parasail_result_t* parasail_sw_stats_rowcol_scan_64(
        const_char * s1, const int s1Len,
        const_char * s2, const int s2Len,
        const int open, const int gap,
        const parasail_matrix_t* matrix);

    parasail_result_t* parasail_sw_stats_rowcol_scan_32(
        const_char * s1, const int s1Len,
        const_char * s2, const int s2Len,
        const int open, const int gap,
        const parasail_matrix_t* matrix);

    parasail_result_t* parasail_sw_stats_rowcol_scan_16(
        const_char * s1, const int s1Len,
        const_char * s2, const int s2Len,
        const int open, const int gap,
        const parasail_matrix_t* matrix);

    parasail_result_t* parasail_sw_stats_rowcol_scan_8(
        const_char * s1, const int s1Len,
        const_char * s2, const int s2Len,
        const int open, const int gap,
        const parasail_matrix_t* matrix);

    parasail_result_t* parasail_sw_stats_rowcol_striped_64(
        const_char * s1, const int s1Len,
        const_char * s2, const int s2Len,
        const int open, const int gap,
        const parasail_matrix_t* matrix);

    parasail_result_t* parasail_sw_stats_rowcol_striped_32(
        const_char * s1, const int s1Len,
        const_char * s2, const int s2Len,
        const int open, const int gap,
        const parasail_matrix_t* matrix);

    parasail_result_t* parasail_sw_stats_rowcol_striped_16(
        const_char * s1, const int s1Len,
        const_char * s2, const int s2Len,
        const int open, const int gap,
        const parasail_matrix_t* matrix);

    parasail_result_t* parasail_sw_stats_rowcol_striped_8(
        const_char * s1, const int s1Len,
        const_char * s2, const int s2Len,
        const int open, const int gap,
        const parasail_matrix_t* matrix);

    parasail_result_t* parasail_sw_stats_rowcol_diag_64(
        const_char * s1, const int s1Len,
        const_char * s2, const int s2Len,
        const int open, const int gap,
        const parasail_matrix_t* matrix);

    parasail_result_t* parasail_sw_stats_rowcol_diag_32(
        const_char * s1, const int s1Len,
        const_char * s2, const int s2Len,
        const int open, const int gap,
        const parasail_matrix_t* matrix);

    parasail_result_t* parasail_sw_stats_rowcol_diag_16(
        const_char * s1, const int s1Len,
        const_char * s2, const int s2Len,
        const int open, const int gap,
        const parasail_matrix_t* matrix);

    parasail_result_t* parasail_sw_stats_rowcol_diag_8(
        const_char * s1, const int s1Len,
        const_char * s2, const int s2Len,
        const int open, const int gap,
        const parasail_matrix_t* matrix);

    parasail_result_t* parasail_sw_blocked_32(
        const_char * s1, const int s1Len,
        const_char * s2, const int s2Len,
        const int open, const int gap,
        const parasail_matrix_t* matrix);

    parasail_result_t* parasail_sw_blocked_16(
        const_char * s1, const int s1Len,
        const_char * s2, const int s2Len,
        const int open, const int gap,
        const parasail_matrix_t* matrix);

    parasail_result_t* parasail_sw_table_blocked_32(
        const_char * s1, const int s1Len,
        const_char * s2, const int s2Len,
        const int open, const int gap,
        const parasail_matrix_t* matrix);

    parasail_result_t* parasail_sw_table_blocked_16(
        const_char * s1, const int s1Len,
        const_char * s2, const int s2Len,
        const int open, const int gap,
        const parasail_matrix_t* matrix);

    parasail_result_t* parasail_sw_rowcol_blocked_32(
        const_char * s1, const int s1Len,
        const_char * s2, const int s2Len,
        const int open, const int gap,
        const parasail_matrix_t* matrix);

    parasail_result_t* parasail_sw_rowcol_blocked_16(
        const_char * s1, const int s1Len,
        const_char * s2, const int s2Len,
        const int open, const int gap,
        const parasail_matrix_t* matrix);

cdef class Result:
    cdef parasail_result_t *_c_object
    def __dealloc__(self):
        if self._c_object is not NULL:
            parasail_result_free(self._c_object)
    @staticmethod
    cdef create(parasail_result_t *c_object):
        p = Result()
        p._c_object = c_object
        return p
    property saturated:
        def __get__(self): return self._c_object.saturated
    property score:
        def __get__(self): return self._c_object.score
    property matches:
        def __get__(self): return self._c_object.matches
    property similar:
        def __get__(self): return self._c_object.similar
    property length:
        def __get__(self): return self._c_object.length
    def __str__(self):
        return str(self.score)
    def __int__(self):
        return self.score
    def __long__(self):
        return self.score


cdef class Matrix:
    cdef const parasail_matrix_t *_c_object
    def __dealloc__(self):
        if self._c_object is not NULL:
            if self._c_object.need_free:
                parasail_matrix_free(<parasail_matrix_t*>self._c_object)
    @staticmethod
    cdef create(const parasail_matrix_t *c_object):
        p = Matrix()
        p._c_object = c_object
        return p
    property name:
        def __get__(self): return self._c_object.name
    property size:
        def __get__(self): return self._c_object.size

def matrix_create(const char* alphabet, int match, int mismatch):
    cdef parasail_matrix_t* matrix = parasail_matrix_create(
            alphabet, match, mismatch)
    return Matrix.create(matrix)


cdef extern from "parasail/matrices/blosum100.h":
    const parasail_matrix_t parasail_blosum100
cdef extern from "parasail/matrices/blosum30.h":
    const parasail_matrix_t parasail_blosum30
cdef extern from "parasail/matrices/blosum35.h":
    const parasail_matrix_t parasail_blosum35
cdef extern from "parasail/matrices/blosum40.h":
    const parasail_matrix_t parasail_blosum40
cdef extern from "parasail/matrices/blosum45.h":
    const parasail_matrix_t parasail_blosum45
cdef extern from "parasail/matrices/blosum50.h":
    const parasail_matrix_t parasail_blosum50
cdef extern from "parasail/matrices/blosum55.h":
    const parasail_matrix_t parasail_blosum55
cdef extern from "parasail/matrices/blosum60.h":
    const parasail_matrix_t parasail_blosum60
cdef extern from "parasail/matrices/blosum62.h":
    const parasail_matrix_t parasail_blosum62
cdef extern from "parasail/matrices/blosum65.h":
    const parasail_matrix_t parasail_blosum65
cdef extern from "parasail/matrices/blosum70.h":
    const parasail_matrix_t parasail_blosum70
cdef extern from "parasail/matrices/blosum75.h":
    const parasail_matrix_t parasail_blosum75
cdef extern from "parasail/matrices/blosum80.h":
    const parasail_matrix_t parasail_blosum80
cdef extern from "parasail/matrices/blosum85.h":
    const parasail_matrix_t parasail_blosum85
cdef extern from "parasail/matrices/blosum90.h":
    const parasail_matrix_t parasail_blosum90
cdef extern from "parasail/matrices/pam10.h":
    const parasail_matrix_t parasail_pam10
cdef extern from "parasail/matrices/pam100.h":
    const parasail_matrix_t parasail_pam100
cdef extern from "parasail/matrices/pam110.h":
    const parasail_matrix_t parasail_pam110
cdef extern from "parasail/matrices/pam120.h":
    const parasail_matrix_t parasail_pam120
cdef extern from "parasail/matrices/pam130.h":
    const parasail_matrix_t parasail_pam130
cdef extern from "parasail/matrices/pam140.h":
    const parasail_matrix_t parasail_pam140
cdef extern from "parasail/matrices/pam150.h":
    const parasail_matrix_t parasail_pam150
cdef extern from "parasail/matrices/pam160.h":
    const parasail_matrix_t parasail_pam160
cdef extern from "parasail/matrices/pam170.h":
    const parasail_matrix_t parasail_pam170
cdef extern from "parasail/matrices/pam180.h":
    const parasail_matrix_t parasail_pam180
cdef extern from "parasail/matrices/pam190.h":
    const parasail_matrix_t parasail_pam190
cdef extern from "parasail/matrices/pam20.h":
    const parasail_matrix_t parasail_pam20
cdef extern from "parasail/matrices/pam200.h":
    const parasail_matrix_t parasail_pam200
cdef extern from "parasail/matrices/pam210.h":
    const parasail_matrix_t parasail_pam210
cdef extern from "parasail/matrices/pam220.h":
    const parasail_matrix_t parasail_pam220
cdef extern from "parasail/matrices/pam230.h":
    const parasail_matrix_t parasail_pam230
cdef extern from "parasail/matrices/pam240.h":
    const parasail_matrix_t parasail_pam240
cdef extern from "parasail/matrices/pam250.h":
    const parasail_matrix_t parasail_pam250
cdef extern from "parasail/matrices/pam260.h":
    const parasail_matrix_t parasail_pam260
cdef extern from "parasail/matrices/pam270.h":
    const parasail_matrix_t parasail_pam270
cdef extern from "parasail/matrices/pam280.h":
    const parasail_matrix_t parasail_pam280
cdef extern from "parasail/matrices/pam290.h":
    const parasail_matrix_t parasail_pam290
cdef extern from "parasail/matrices/pam30.h":
    const parasail_matrix_t parasail_pam30
cdef extern from "parasail/matrices/pam300.h":
    const parasail_matrix_t parasail_pam300
cdef extern from "parasail/matrices/pam310.h":
    const parasail_matrix_t parasail_pam310
cdef extern from "parasail/matrices/pam320.h":
    const parasail_matrix_t parasail_pam320
cdef extern from "parasail/matrices/pam330.h":
    const parasail_matrix_t parasail_pam330
cdef extern from "parasail/matrices/pam340.h":
    const parasail_matrix_t parasail_pam340
cdef extern from "parasail/matrices/pam350.h":
    const parasail_matrix_t parasail_pam350
cdef extern from "parasail/matrices/pam360.h":
    const parasail_matrix_t parasail_pam360
cdef extern from "parasail/matrices/pam370.h":
    const parasail_matrix_t parasail_pam370
cdef extern from "parasail/matrices/pam380.h":
    const parasail_matrix_t parasail_pam380
cdef extern from "parasail/matrices/pam390.h":
    const parasail_matrix_t parasail_pam390
cdef extern from "parasail/matrices/pam40.h":
    const parasail_matrix_t parasail_pam40
cdef extern from "parasail/matrices/pam400.h":
    const parasail_matrix_t parasail_pam400
cdef extern from "parasail/matrices/pam410.h":
    const parasail_matrix_t parasail_pam410
cdef extern from "parasail/matrices/pam420.h":
    const parasail_matrix_t parasail_pam420
cdef extern from "parasail/matrices/pam430.h":
    const parasail_matrix_t parasail_pam430
cdef extern from "parasail/matrices/pam440.h":
    const parasail_matrix_t parasail_pam440
cdef extern from "parasail/matrices/pam450.h":
    const parasail_matrix_t parasail_pam450
cdef extern from "parasail/matrices/pam460.h":
    const parasail_matrix_t parasail_pam460
cdef extern from "parasail/matrices/pam470.h":
    const parasail_matrix_t parasail_pam470
cdef extern from "parasail/matrices/pam480.h":
    const parasail_matrix_t parasail_pam480
cdef extern from "parasail/matrices/pam490.h":
    const parasail_matrix_t parasail_pam490
cdef extern from "parasail/matrices/pam50.h":
    const parasail_matrix_t parasail_pam50
cdef extern from "parasail/matrices/pam500.h":
    const parasail_matrix_t parasail_pam500
cdef extern from "parasail/matrices/pam60.h":
    const parasail_matrix_t parasail_pam60
cdef extern from "parasail/matrices/pam70.h":
    const parasail_matrix_t parasail_pam70
cdef extern from "parasail/matrices/pam80.h":
    const parasail_matrix_t parasail_pam80
cdef extern from "parasail/matrices/pam90.h":
    const parasail_matrix_t parasail_pam90
blosum100 = Matrix.create(&parasail_blosum100)
blosum30 = Matrix.create(&parasail_blosum30)
blosum35 = Matrix.create(&parasail_blosum35)
blosum40 = Matrix.create(&parasail_blosum40)
blosum45 = Matrix.create(&parasail_blosum45)
blosum50 = Matrix.create(&parasail_blosum50)
blosum55 = Matrix.create(&parasail_blosum55)
blosum60 = Matrix.create(&parasail_blosum60)
blosum62 = Matrix.create(&parasail_blosum62)
blosum65 = Matrix.create(&parasail_blosum65)
blosum70 = Matrix.create(&parasail_blosum70)
blosum75 = Matrix.create(&parasail_blosum75)
blosum80 = Matrix.create(&parasail_blosum80)
blosum85 = Matrix.create(&parasail_blosum85)
blosum90 = Matrix.create(&parasail_blosum90)
pam10 = Matrix.create(&parasail_pam10)
pam100 = Matrix.create(&parasail_pam100)
pam110 = Matrix.create(&parasail_pam110)
pam120 = Matrix.create(&parasail_pam120)
pam130 = Matrix.create(&parasail_pam130)
pam140 = Matrix.create(&parasail_pam140)
pam150 = Matrix.create(&parasail_pam150)
pam160 = Matrix.create(&parasail_pam160)
pam170 = Matrix.create(&parasail_pam170)
pam180 = Matrix.create(&parasail_pam180)
pam190 = Matrix.create(&parasail_pam190)
pam20 = Matrix.create(&parasail_pam20)
pam200 = Matrix.create(&parasail_pam200)
pam210 = Matrix.create(&parasail_pam210)
pam220 = Matrix.create(&parasail_pam220)
pam230 = Matrix.create(&parasail_pam230)
pam240 = Matrix.create(&parasail_pam240)
pam250 = Matrix.create(&parasail_pam250)
pam260 = Matrix.create(&parasail_pam260)
pam270 = Matrix.create(&parasail_pam270)
pam280 = Matrix.create(&parasail_pam280)
pam290 = Matrix.create(&parasail_pam290)
pam30 = Matrix.create(&parasail_pam30)
pam300 = Matrix.create(&parasail_pam300)
pam310 = Matrix.create(&parasail_pam310)
pam320 = Matrix.create(&parasail_pam320)
pam330 = Matrix.create(&parasail_pam330)
pam340 = Matrix.create(&parasail_pam340)
pam350 = Matrix.create(&parasail_pam350)
pam360 = Matrix.create(&parasail_pam360)
pam370 = Matrix.create(&parasail_pam370)
pam380 = Matrix.create(&parasail_pam380)
pam390 = Matrix.create(&parasail_pam390)
pam40 = Matrix.create(&parasail_pam40)
pam400 = Matrix.create(&parasail_pam400)
pam410 = Matrix.create(&parasail_pam410)
pam420 = Matrix.create(&parasail_pam420)
pam430 = Matrix.create(&parasail_pam430)
pam440 = Matrix.create(&parasail_pam440)
pam450 = Matrix.create(&parasail_pam450)
pam460 = Matrix.create(&parasail_pam460)
pam470 = Matrix.create(&parasail_pam470)
pam480 = Matrix.create(&parasail_pam480)
pam490 = Matrix.create(&parasail_pam490)
pam50 = Matrix.create(&parasail_pam50)
pam500 = Matrix.create(&parasail_pam500)
pam60 = Matrix.create(&parasail_pam60)
pam70 = Matrix.create(&parasail_pam70)
pam80 = Matrix.create(&parasail_pam80)
pam90 = Matrix.create(&parasail_pam90)

def nw(
        const_char * s1,
        const_char * s2,
        const int open, const int gap,
        Matrix matrix not None):
    cdef size_t t1 = len(s1)
    cdef size_t t2 = len(s2)
    cdef int l1 = <int>t1
    cdef int l2 = <int>t2
    assert l1 == t1
    assert l2 == t2
    return Result.create(parasail_nw(s1,l1,s2,l2,open,gap,matrix._c_object))

def nw_scan(
        const_char * s1,
        const_char * s2,
        const int open, const int gap,
        Matrix matrix not None):
    cdef size_t t1 = len(s1)
    cdef size_t t2 = len(s2)
    cdef int l1 = <int>t1
    cdef int l2 = <int>t2
    assert l1 == t1
    assert l2 == t2
    return Result.create(parasail_nw_scan(s1,l1,s2,l2,open,gap,matrix._c_object))

def nw_table(
        const_char * s1,
        const_char * s2,
        const int open, const int gap,
        Matrix matrix not None):
    cdef size_t t1 = len(s1)
    cdef size_t t2 = len(s2)
    cdef int l1 = <int>t1
    cdef int l2 = <int>t2
    assert l1 == t1
    assert l2 == t2
    return Result.create(parasail_nw_table(s1,l1,s2,l2,open,gap,matrix._c_object))

def nw_table_scan(
        const_char * s1,
        const_char * s2,
        const int open, const int gap,
        Matrix matrix not None):
    cdef size_t t1 = len(s1)
    cdef size_t t2 = len(s2)
    cdef int l1 = <int>t1
    cdef int l2 = <int>t2
    assert l1 == t1
    assert l2 == t2
    return Result.create(parasail_nw_table_scan(s1,l1,s2,l2,open,gap,matrix._c_object))

def nw_rowcol(
        const_char * s1,
        const_char * s2,
        const int open, const int gap,
        Matrix matrix not None):
    cdef size_t t1 = len(s1)
    cdef size_t t2 = len(s2)
    cdef int l1 = <int>t1
    cdef int l2 = <int>t2
    assert l1 == t1
    assert l2 == t2
    return Result.create(parasail_nw_rowcol(s1,l1,s2,l2,open,gap,matrix._c_object))

def nw_rowcol_scan(
        const_char * s1,
        const_char * s2,
        const int open, const int gap,
        Matrix matrix not None):
    cdef size_t t1 = len(s1)
    cdef size_t t2 = len(s2)
    cdef int l1 = <int>t1
    cdef int l2 = <int>t2
    assert l1 == t1
    assert l2 == t2
    return Result.create(parasail_nw_rowcol_scan(s1,l1,s2,l2,open,gap,matrix._c_object))

def nw_stats(
        const_char * s1,
        const_char * s2,
        const int open, const int gap,
        Matrix matrix not None):
    cdef size_t t1 = len(s1)
    cdef size_t t2 = len(s2)
    cdef int l1 = <int>t1
    cdef int l2 = <int>t2
    assert l1 == t1
    assert l2 == t2
    return Result.create(parasail_nw_stats(s1,l1,s2,l2,open,gap,matrix._c_object))

def nw_stats_scan(
        const_char * s1,
        const_char * s2,
        const int open, const int gap,
        Matrix matrix not None):
    cdef size_t t1 = len(s1)
    cdef size_t t2 = len(s2)
    cdef int l1 = <int>t1
    cdef int l2 = <int>t2
    assert l1 == t1
    assert l2 == t2
    return Result.create(parasail_nw_stats_scan(s1,l1,s2,l2,open,gap,matrix._c_object))

def nw_stats_table(
        const_char * s1,
        const_char * s2,
        const int open, const int gap,
        Matrix matrix not None):
    cdef size_t t1 = len(s1)
    cdef size_t t2 = len(s2)
    cdef int l1 = <int>t1
    cdef int l2 = <int>t2
    assert l1 == t1
    assert l2 == t2
    return Result.create(parasail_nw_stats_table(s1,l1,s2,l2,open,gap,matrix._c_object))

def nw_stats_table_scan(
        const_char * s1,
        const_char * s2,
        const int open, const int gap,
        Matrix matrix not None):
    cdef size_t t1 = len(s1)
    cdef size_t t2 = len(s2)
    cdef int l1 = <int>t1
    cdef int l2 = <int>t2
    assert l1 == t1
    assert l2 == t2
    return Result.create(parasail_nw_stats_table_scan(s1,l1,s2,l2,open,gap,matrix._c_object))

def nw_stats_rowcol(
        const_char * s1,
        const_char * s2,
        const int open, const int gap,
        Matrix matrix not None):
    cdef size_t t1 = len(s1)
    cdef size_t t2 = len(s2)
    cdef int l1 = <int>t1
    cdef int l2 = <int>t2
    assert l1 == t1
    assert l2 == t2
    return Result.create(parasail_nw_stats_rowcol(s1,l1,s2,l2,open,gap,matrix._c_object))

def nw_stats_rowcol_scan(
        const_char * s1,
        const_char * s2,
        const int open, const int gap,
        Matrix matrix not None):
    cdef size_t t1 = len(s1)
    cdef size_t t2 = len(s2)
    cdef int l1 = <int>t1
    cdef int l2 = <int>t2
    assert l1 == t1
    assert l2 == t2
    return Result.create(parasail_nw_stats_rowcol_scan(s1,l1,s2,l2,open,gap,matrix._c_object))

def sg(
        const_char * s1,
        const_char * s2,
        const int open, const int gap,
        Matrix matrix not None):
    cdef size_t t1 = len(s1)
    cdef size_t t2 = len(s2)
    cdef int l1 = <int>t1
    cdef int l2 = <int>t2
    assert l1 == t1
    assert l2 == t2
    return Result.create(parasail_sg(s1,l1,s2,l2,open,gap,matrix._c_object))

def sg_scan(
        const_char * s1,
        const_char * s2,
        const int open, const int gap,
        Matrix matrix not None):
    cdef size_t t1 = len(s1)
    cdef size_t t2 = len(s2)
    cdef int l1 = <int>t1
    cdef int l2 = <int>t2
    assert l1 == t1
    assert l2 == t2
    return Result.create(parasail_sg_scan(s1,l1,s2,l2,open,gap,matrix._c_object))

def sg_table(
        const_char * s1,
        const_char * s2,
        const int open, const int gap,
        Matrix matrix not None):
    cdef size_t t1 = len(s1)
    cdef size_t t2 = len(s2)
    cdef int l1 = <int>t1
    cdef int l2 = <int>t2
    assert l1 == t1
    assert l2 == t2
    return Result.create(parasail_sg_table(s1,l1,s2,l2,open,gap,matrix._c_object))

def sg_table_scan(
        const_char * s1,
        const_char * s2,
        const int open, const int gap,
        Matrix matrix not None):
    cdef size_t t1 = len(s1)
    cdef size_t t2 = len(s2)
    cdef int l1 = <int>t1
    cdef int l2 = <int>t2
    assert l1 == t1
    assert l2 == t2
    return Result.create(parasail_sg_table_scan(s1,l1,s2,l2,open,gap,matrix._c_object))

def sg_rowcol(
        const_char * s1,
        const_char * s2,
        const int open, const int gap,
        Matrix matrix not None):
    cdef size_t t1 = len(s1)
    cdef size_t t2 = len(s2)
    cdef int l1 = <int>t1
    cdef int l2 = <int>t2
    assert l1 == t1
    assert l2 == t2
    return Result.create(parasail_sg_rowcol(s1,l1,s2,l2,open,gap,matrix._c_object))

def sg_rowcol_scan(
        const_char * s1,
        const_char * s2,
        const int open, const int gap,
        Matrix matrix not None):
    cdef size_t t1 = len(s1)
    cdef size_t t2 = len(s2)
    cdef int l1 = <int>t1
    cdef int l2 = <int>t2
    assert l1 == t1
    assert l2 == t2
    return Result.create(parasail_sg_rowcol_scan(s1,l1,s2,l2,open,gap,matrix._c_object))

def sg_stats(
        const_char * s1,
        const_char * s2,
        const int open, const int gap,
        Matrix matrix not None):
    cdef size_t t1 = len(s1)
    cdef size_t t2 = len(s2)
    cdef int l1 = <int>t1
    cdef int l2 = <int>t2
    assert l1 == t1
    assert l2 == t2
    return Result.create(parasail_sg_stats(s1,l1,s2,l2,open,gap,matrix._c_object))

def sg_stats_scan(
        const_char * s1,
        const_char * s2,
        const int open, const int gap,
        Matrix matrix not None):
    cdef size_t t1 = len(s1)
    cdef size_t t2 = len(s2)
    cdef int l1 = <int>t1
    cdef int l2 = <int>t2
    assert l1 == t1
    assert l2 == t2
    return Result.create(parasail_sg_stats_scan(s1,l1,s2,l2,open,gap,matrix._c_object))

def sg_stats_table(
        const_char * s1,
        const_char * s2,
        const int open, const int gap,
        Matrix matrix not None):
    cdef size_t t1 = len(s1)
    cdef size_t t2 = len(s2)
    cdef int l1 = <int>t1
    cdef int l2 = <int>t2
    assert l1 == t1
    assert l2 == t2
    return Result.create(parasail_sg_stats_table(s1,l1,s2,l2,open,gap,matrix._c_object))

def sg_stats_table_scan(
        const_char * s1,
        const_char * s2,
        const int open, const int gap,
        Matrix matrix not None):
    cdef size_t t1 = len(s1)
    cdef size_t t2 = len(s2)
    cdef int l1 = <int>t1
    cdef int l2 = <int>t2
    assert l1 == t1
    assert l2 == t2
    return Result.create(parasail_sg_stats_table_scan(s1,l1,s2,l2,open,gap,matrix._c_object))

def sg_stats_rowcol(
        const_char * s1,
        const_char * s2,
        const int open, const int gap,
        Matrix matrix not None):
    cdef size_t t1 = len(s1)
    cdef size_t t2 = len(s2)
    cdef int l1 = <int>t1
    cdef int l2 = <int>t2
    assert l1 == t1
    assert l2 == t2
    return Result.create(parasail_sg_stats_rowcol(s1,l1,s2,l2,open,gap,matrix._c_object))

def sg_stats_rowcol_scan(
        const_char * s1,
        const_char * s2,
        const int open, const int gap,
        Matrix matrix not None):
    cdef size_t t1 = len(s1)
    cdef size_t t2 = len(s2)
    cdef int l1 = <int>t1
    cdef int l2 = <int>t2
    assert l1 == t1
    assert l2 == t2
    return Result.create(parasail_sg_stats_rowcol_scan(s1,l1,s2,l2,open,gap,matrix._c_object))

def sw(
        const_char * s1,
        const_char * s2,
        const int open, const int gap,
        Matrix matrix not None):
    cdef size_t t1 = len(s1)
    cdef size_t t2 = len(s2)
    cdef int l1 = <int>t1
    cdef int l2 = <int>t2
    assert l1 == t1
    assert l2 == t2
    return Result.create(parasail_sw(s1,l1,s2,l2,open,gap,matrix._c_object))

def sw_scan(
        const_char * s1,
        const_char * s2,
        const int open, const int gap,
        Matrix matrix not None):
    cdef size_t t1 = len(s1)
    cdef size_t t2 = len(s2)
    cdef int l1 = <int>t1
    cdef int l2 = <int>t2
    assert l1 == t1
    assert l2 == t2
    return Result.create(parasail_sw_scan(s1,l1,s2,l2,open,gap,matrix._c_object))

def sw_table(
        const_char * s1,
        const_char * s2,
        const int open, const int gap,
        Matrix matrix not None):
    cdef size_t t1 = len(s1)
    cdef size_t t2 = len(s2)
    cdef int l1 = <int>t1
    cdef int l2 = <int>t2
    assert l1 == t1
    assert l2 == t2
    return Result.create(parasail_sw_table(s1,l1,s2,l2,open,gap,matrix._c_object))

def sw_table_scan(
        const_char * s1,
        const_char * s2,
        const int open, const int gap,
        Matrix matrix not None):
    cdef size_t t1 = len(s1)
    cdef size_t t2 = len(s2)
    cdef int l1 = <int>t1
    cdef int l2 = <int>t2
    assert l1 == t1
    assert l2 == t2
    return Result.create(parasail_sw_table_scan(s1,l1,s2,l2,open,gap,matrix._c_object))

def sw_rowcol(
        const_char * s1,
        const_char * s2,
        const int open, const int gap,
        Matrix matrix not None):
    cdef size_t t1 = len(s1)
    cdef size_t t2 = len(s2)
    cdef int l1 = <int>t1
    cdef int l2 = <int>t2
    assert l1 == t1
    assert l2 == t2
    return Result.create(parasail_sw_rowcol(s1,l1,s2,l2,open,gap,matrix._c_object))

def sw_rowcol_scan(
        const_char * s1,
        const_char * s2,
        const int open, const int gap,
        Matrix matrix not None):
    cdef size_t t1 = len(s1)
    cdef size_t t2 = len(s2)
    cdef int l1 = <int>t1
    cdef int l2 = <int>t2
    assert l1 == t1
    assert l2 == t2
    return Result.create(parasail_sw_rowcol_scan(s1,l1,s2,l2,open,gap,matrix._c_object))

def sw_stats(
        const_char * s1,
        const_char * s2,
        const int open, const int gap,
        Matrix matrix not None):
    cdef size_t t1 = len(s1)
    cdef size_t t2 = len(s2)
    cdef int l1 = <int>t1
    cdef int l2 = <int>t2
    assert l1 == t1
    assert l2 == t2
    return Result.create(parasail_sw_stats(s1,l1,s2,l2,open,gap,matrix._c_object))

def sw_stats_scan(
        const_char * s1,
        const_char * s2,
        const int open, const int gap,
        Matrix matrix not None):
    cdef size_t t1 = len(s1)
    cdef size_t t2 = len(s2)
    cdef int l1 = <int>t1
    cdef int l2 = <int>t2
    assert l1 == t1
    assert l2 == t2
    return Result.create(parasail_sw_stats_scan(s1,l1,s2,l2,open,gap,matrix._c_object))

def sw_stats_table(
        const_char * s1,
        const_char * s2,
        const int open, const int gap,
        Matrix matrix not None):
    cdef size_t t1 = len(s1)
    cdef size_t t2 = len(s2)
    cdef int l1 = <int>t1
    cdef int l2 = <int>t2
    assert l1 == t1
    assert l2 == t2
    return Result.create(parasail_sw_stats_table(s1,l1,s2,l2,open,gap,matrix._c_object))

def sw_stats_table_scan(
        const_char * s1,
        const_char * s2,
        const int open, const int gap,
        Matrix matrix not None):
    cdef size_t t1 = len(s1)
    cdef size_t t2 = len(s2)
    cdef int l1 = <int>t1
    cdef int l2 = <int>t2
    assert l1 == t1
    assert l2 == t2
    return Result.create(parasail_sw_stats_table_scan(s1,l1,s2,l2,open,gap,matrix._c_object))

def sw_stats_rowcol(
        const_char * s1,
        const_char * s2,
        const int open, const int gap,
        Matrix matrix not None):
    cdef size_t t1 = len(s1)
    cdef size_t t2 = len(s2)
    cdef int l1 = <int>t1
    cdef int l2 = <int>t2
    assert l1 == t1
    assert l2 == t2
    return Result.create(parasail_sw_stats_rowcol(s1,l1,s2,l2,open,gap,matrix._c_object))

def sw_stats_rowcol_scan(
        const_char * s1,
        const_char * s2,
        const int open, const int gap,
        Matrix matrix not None):
    cdef size_t t1 = len(s1)
    cdef size_t t2 = len(s2)
    cdef int l1 = <int>t1
    cdef int l2 = <int>t2
    assert l1 == t1
    assert l2 == t2
    return Result.create(parasail_sw_stats_rowcol_scan(s1,l1,s2,l2,open,gap,matrix._c_object))

def nw_scan_64(
        const_char * s1,
        const_char * s2,
        const int open, const int gap,
        Matrix matrix not None):
    cdef size_t t1 = len(s1)
    cdef size_t t2 = len(s2)
    cdef int l1 = <int>t1
    cdef int l2 = <int>t2
    assert l1 == t1
    assert l2 == t2
    return Result.create(parasail_nw_scan_64(s1,l1,s2,l2,open,gap,matrix._c_object))

def nw_scan_32(
        const_char * s1,
        const_char * s2,
        const int open, const int gap,
        Matrix matrix not None):
    cdef size_t t1 = len(s1)
    cdef size_t t2 = len(s2)
    cdef int l1 = <int>t1
    cdef int l2 = <int>t2
    assert l1 == t1
    assert l2 == t2
    return Result.create(parasail_nw_scan_32(s1,l1,s2,l2,open,gap,matrix._c_object))

def nw_scan_16(
        const_char * s1,
        const_char * s2,
        const int open, const int gap,
        Matrix matrix not None):
    cdef size_t t1 = len(s1)
    cdef size_t t2 = len(s2)
    cdef int l1 = <int>t1
    cdef int l2 = <int>t2
    assert l1 == t1
    assert l2 == t2
    return Result.create(parasail_nw_scan_16(s1,l1,s2,l2,open,gap,matrix._c_object))

def nw_scan_8(
        const_char * s1,
        const_char * s2,
        const int open, const int gap,
        Matrix matrix not None):
    cdef size_t t1 = len(s1)
    cdef size_t t2 = len(s2)
    cdef int l1 = <int>t1
    cdef int l2 = <int>t2
    assert l1 == t1
    assert l2 == t2
    return Result.create(parasail_nw_scan_8(s1,l1,s2,l2,open,gap,matrix._c_object))

def nw_striped_64(
        const_char * s1,
        const_char * s2,
        const int open, const int gap,
        Matrix matrix not None):
    cdef size_t t1 = len(s1)
    cdef size_t t2 = len(s2)
    cdef int l1 = <int>t1
    cdef int l2 = <int>t2
    assert l1 == t1
    assert l2 == t2
    return Result.create(parasail_nw_striped_64(s1,l1,s2,l2,open,gap,matrix._c_object))

def nw_striped_32(
        const_char * s1,
        const_char * s2,
        const int open, const int gap,
        Matrix matrix not None):
    cdef size_t t1 = len(s1)
    cdef size_t t2 = len(s2)
    cdef int l1 = <int>t1
    cdef int l2 = <int>t2
    assert l1 == t1
    assert l2 == t2
    return Result.create(parasail_nw_striped_32(s1,l1,s2,l2,open,gap,matrix._c_object))

def nw_striped_16(
        const_char * s1,
        const_char * s2,
        const int open, const int gap,
        Matrix matrix not None):
    cdef size_t t1 = len(s1)
    cdef size_t t2 = len(s2)
    cdef int l1 = <int>t1
    cdef int l2 = <int>t2
    assert l1 == t1
    assert l2 == t2
    return Result.create(parasail_nw_striped_16(s1,l1,s2,l2,open,gap,matrix._c_object))

def nw_striped_8(
        const_char * s1,
        const_char * s2,
        const int open, const int gap,
        Matrix matrix not None):
    cdef size_t t1 = len(s1)
    cdef size_t t2 = len(s2)
    cdef int l1 = <int>t1
    cdef int l2 = <int>t2
    assert l1 == t1
    assert l2 == t2
    return Result.create(parasail_nw_striped_8(s1,l1,s2,l2,open,gap,matrix._c_object))

def nw_diag_64(
        const_char * s1,
        const_char * s2,
        const int open, const int gap,
        Matrix matrix not None):
    cdef size_t t1 = len(s1)
    cdef size_t t2 = len(s2)
    cdef int l1 = <int>t1
    cdef int l2 = <int>t2
    assert l1 == t1
    assert l2 == t2
    return Result.create(parasail_nw_diag_64(s1,l1,s2,l2,open,gap,matrix._c_object))

def nw_diag_32(
        const_char * s1,
        const_char * s2,
        const int open, const int gap,
        Matrix matrix not None):
    cdef size_t t1 = len(s1)
    cdef size_t t2 = len(s2)
    cdef int l1 = <int>t1
    cdef int l2 = <int>t2
    assert l1 == t1
    assert l2 == t2
    return Result.create(parasail_nw_diag_32(s1,l1,s2,l2,open,gap,matrix._c_object))

def nw_diag_16(
        const_char * s1,
        const_char * s2,
        const int open, const int gap,
        Matrix matrix not None):
    cdef size_t t1 = len(s1)
    cdef size_t t2 = len(s2)
    cdef int l1 = <int>t1
    cdef int l2 = <int>t2
    assert l1 == t1
    assert l2 == t2
    return Result.create(parasail_nw_diag_16(s1,l1,s2,l2,open,gap,matrix._c_object))

def nw_diag_8(
        const_char * s1,
        const_char * s2,
        const int open, const int gap,
        Matrix matrix not None):
    cdef size_t t1 = len(s1)
    cdef size_t t2 = len(s2)
    cdef int l1 = <int>t1
    cdef int l2 = <int>t2
    assert l1 == t1
    assert l2 == t2
    return Result.create(parasail_nw_diag_8(s1,l1,s2,l2,open,gap,matrix._c_object))

def nw_table_scan_64(
        const_char * s1,
        const_char * s2,
        const int open, const int gap,
        Matrix matrix not None):
    cdef size_t t1 = len(s1)
    cdef size_t t2 = len(s2)
    cdef int l1 = <int>t1
    cdef int l2 = <int>t2
    assert l1 == t1
    assert l2 == t2
    return Result.create(parasail_nw_table_scan_64(s1,l1,s2,l2,open,gap,matrix._c_object))

def nw_table_scan_32(
        const_char * s1,
        const_char * s2,
        const int open, const int gap,
        Matrix matrix not None):
    cdef size_t t1 = len(s1)
    cdef size_t t2 = len(s2)
    cdef int l1 = <int>t1
    cdef int l2 = <int>t2
    assert l1 == t1
    assert l2 == t2
    return Result.create(parasail_nw_table_scan_32(s1,l1,s2,l2,open,gap,matrix._c_object))

def nw_table_scan_16(
        const_char * s1,
        const_char * s2,
        const int open, const int gap,
        Matrix matrix not None):
    cdef size_t t1 = len(s1)
    cdef size_t t2 = len(s2)
    cdef int l1 = <int>t1
    cdef int l2 = <int>t2
    assert l1 == t1
    assert l2 == t2
    return Result.create(parasail_nw_table_scan_16(s1,l1,s2,l2,open,gap,matrix._c_object))

def nw_table_scan_8(
        const_char * s1,
        const_char * s2,
        const int open, const int gap,
        Matrix matrix not None):
    cdef size_t t1 = len(s1)
    cdef size_t t2 = len(s2)
    cdef int l1 = <int>t1
    cdef int l2 = <int>t2
    assert l1 == t1
    assert l2 == t2
    return Result.create(parasail_nw_table_scan_8(s1,l1,s2,l2,open,gap,matrix._c_object))

def nw_table_striped_64(
        const_char * s1,
        const_char * s2,
        const int open, const int gap,
        Matrix matrix not None):
    cdef size_t t1 = len(s1)
    cdef size_t t2 = len(s2)
    cdef int l1 = <int>t1
    cdef int l2 = <int>t2
    assert l1 == t1
    assert l2 == t2
    return Result.create(parasail_nw_table_striped_64(s1,l1,s2,l2,open,gap,matrix._c_object))

def nw_table_striped_32(
        const_char * s1,
        const_char * s2,
        const int open, const int gap,
        Matrix matrix not None):
    cdef size_t t1 = len(s1)
    cdef size_t t2 = len(s2)
    cdef int l1 = <int>t1
    cdef int l2 = <int>t2
    assert l1 == t1
    assert l2 == t2
    return Result.create(parasail_nw_table_striped_32(s1,l1,s2,l2,open,gap,matrix._c_object))

def nw_table_striped_16(
        const_char * s1,
        const_char * s2,
        const int open, const int gap,
        Matrix matrix not None):
    cdef size_t t1 = len(s1)
    cdef size_t t2 = len(s2)
    cdef int l1 = <int>t1
    cdef int l2 = <int>t2
    assert l1 == t1
    assert l2 == t2
    return Result.create(parasail_nw_table_striped_16(s1,l1,s2,l2,open,gap,matrix._c_object))

def nw_table_striped_8(
        const_char * s1,
        const_char * s2,
        const int open, const int gap,
        Matrix matrix not None):
    cdef size_t t1 = len(s1)
    cdef size_t t2 = len(s2)
    cdef int l1 = <int>t1
    cdef int l2 = <int>t2
    assert l1 == t1
    assert l2 == t2
    return Result.create(parasail_nw_table_striped_8(s1,l1,s2,l2,open,gap,matrix._c_object))

def nw_table_diag_64(
        const_char * s1,
        const_char * s2,
        const int open, const int gap,
        Matrix matrix not None):
    cdef size_t t1 = len(s1)
    cdef size_t t2 = len(s2)
    cdef int l1 = <int>t1
    cdef int l2 = <int>t2
    assert l1 == t1
    assert l2 == t2
    return Result.create(parasail_nw_table_diag_64(s1,l1,s2,l2,open,gap,matrix._c_object))

def nw_table_diag_32(
        const_char * s1,
        const_char * s2,
        const int open, const int gap,
        Matrix matrix not None):
    cdef size_t t1 = len(s1)
    cdef size_t t2 = len(s2)
    cdef int l1 = <int>t1
    cdef int l2 = <int>t2
    assert l1 == t1
    assert l2 == t2
    return Result.create(parasail_nw_table_diag_32(s1,l1,s2,l2,open,gap,matrix._c_object))

def nw_table_diag_16(
        const_char * s1,
        const_char * s2,
        const int open, const int gap,
        Matrix matrix not None):
    cdef size_t t1 = len(s1)
    cdef size_t t2 = len(s2)
    cdef int l1 = <int>t1
    cdef int l2 = <int>t2
    assert l1 == t1
    assert l2 == t2
    return Result.create(parasail_nw_table_diag_16(s1,l1,s2,l2,open,gap,matrix._c_object))

def nw_table_diag_8(
        const_char * s1,
        const_char * s2,
        const int open, const int gap,
        Matrix matrix not None):
    cdef size_t t1 = len(s1)
    cdef size_t t2 = len(s2)
    cdef int l1 = <int>t1
    cdef int l2 = <int>t2
    assert l1 == t1
    assert l2 == t2
    return Result.create(parasail_nw_table_diag_8(s1,l1,s2,l2,open,gap,matrix._c_object))

def nw_rowcol_scan_64(
        const_char * s1,
        const_char * s2,
        const int open, const int gap,
        Matrix matrix not None):
    cdef size_t t1 = len(s1)
    cdef size_t t2 = len(s2)
    cdef int l1 = <int>t1
    cdef int l2 = <int>t2
    assert l1 == t1
    assert l2 == t2
    return Result.create(parasail_nw_rowcol_scan_64(s1,l1,s2,l2,open,gap,matrix._c_object))

def nw_rowcol_scan_32(
        const_char * s1,
        const_char * s2,
        const int open, const int gap,
        Matrix matrix not None):
    cdef size_t t1 = len(s1)
    cdef size_t t2 = len(s2)
    cdef int l1 = <int>t1
    cdef int l2 = <int>t2
    assert l1 == t1
    assert l2 == t2
    return Result.create(parasail_nw_rowcol_scan_32(s1,l1,s2,l2,open,gap,matrix._c_object))

def nw_rowcol_scan_16(
        const_char * s1,
        const_char * s2,
        const int open, const int gap,
        Matrix matrix not None):
    cdef size_t t1 = len(s1)
    cdef size_t t2 = len(s2)
    cdef int l1 = <int>t1
    cdef int l2 = <int>t2
    assert l1 == t1
    assert l2 == t2
    return Result.create(parasail_nw_rowcol_scan_16(s1,l1,s2,l2,open,gap,matrix._c_object))

def nw_rowcol_scan_8(
        const_char * s1,
        const_char * s2,
        const int open, const int gap,
        Matrix matrix not None):
    cdef size_t t1 = len(s1)
    cdef size_t t2 = len(s2)
    cdef int l1 = <int>t1
    cdef int l2 = <int>t2
    assert l1 == t1
    assert l2 == t2
    return Result.create(parasail_nw_rowcol_scan_8(s1,l1,s2,l2,open,gap,matrix._c_object))

def nw_rowcol_striped_64(
        const_char * s1,
        const_char * s2,
        const int open, const int gap,
        Matrix matrix not None):
    cdef size_t t1 = len(s1)
    cdef size_t t2 = len(s2)
    cdef int l1 = <int>t1
    cdef int l2 = <int>t2
    assert l1 == t1
    assert l2 == t2
    return Result.create(parasail_nw_rowcol_striped_64(s1,l1,s2,l2,open,gap,matrix._c_object))

def nw_rowcol_striped_32(
        const_char * s1,
        const_char * s2,
        const int open, const int gap,
        Matrix matrix not None):
    cdef size_t t1 = len(s1)
    cdef size_t t2 = len(s2)
    cdef int l1 = <int>t1
    cdef int l2 = <int>t2
    assert l1 == t1
    assert l2 == t2
    return Result.create(parasail_nw_rowcol_striped_32(s1,l1,s2,l2,open,gap,matrix._c_object))

def nw_rowcol_striped_16(
        const_char * s1,
        const_char * s2,
        const int open, const int gap,
        Matrix matrix not None):
    cdef size_t t1 = len(s1)
    cdef size_t t2 = len(s2)
    cdef int l1 = <int>t1
    cdef int l2 = <int>t2
    assert l1 == t1
    assert l2 == t2
    return Result.create(parasail_nw_rowcol_striped_16(s1,l1,s2,l2,open,gap,matrix._c_object))

def nw_rowcol_striped_8(
        const_char * s1,
        const_char * s2,
        const int open, const int gap,
        Matrix matrix not None):
    cdef size_t t1 = len(s1)
    cdef size_t t2 = len(s2)
    cdef int l1 = <int>t1
    cdef int l2 = <int>t2
    assert l1 == t1
    assert l2 == t2
    return Result.create(parasail_nw_rowcol_striped_8(s1,l1,s2,l2,open,gap,matrix._c_object))

def nw_rowcol_diag_64(
        const_char * s1,
        const_char * s2,
        const int open, const int gap,
        Matrix matrix not None):
    cdef size_t t1 = len(s1)
    cdef size_t t2 = len(s2)
    cdef int l1 = <int>t1
    cdef int l2 = <int>t2
    assert l1 == t1
    assert l2 == t2
    return Result.create(parasail_nw_rowcol_diag_64(s1,l1,s2,l2,open,gap,matrix._c_object))

def nw_rowcol_diag_32(
        const_char * s1,
        const_char * s2,
        const int open, const int gap,
        Matrix matrix not None):
    cdef size_t t1 = len(s1)
    cdef size_t t2 = len(s2)
    cdef int l1 = <int>t1
    cdef int l2 = <int>t2
    assert l1 == t1
    assert l2 == t2
    return Result.create(parasail_nw_rowcol_diag_32(s1,l1,s2,l2,open,gap,matrix._c_object))

def nw_rowcol_diag_16(
        const_char * s1,
        const_char * s2,
        const int open, const int gap,
        Matrix matrix not None):
    cdef size_t t1 = len(s1)
    cdef size_t t2 = len(s2)
    cdef int l1 = <int>t1
    cdef int l2 = <int>t2
    assert l1 == t1
    assert l2 == t2
    return Result.create(parasail_nw_rowcol_diag_16(s1,l1,s2,l2,open,gap,matrix._c_object))

def nw_rowcol_diag_8(
        const_char * s1,
        const_char * s2,
        const int open, const int gap,
        Matrix matrix not None):
    cdef size_t t1 = len(s1)
    cdef size_t t2 = len(s2)
    cdef int l1 = <int>t1
    cdef int l2 = <int>t2
    assert l1 == t1
    assert l2 == t2
    return Result.create(parasail_nw_rowcol_diag_8(s1,l1,s2,l2,open,gap,matrix._c_object))

def nw_stats_scan_64(
        const_char * s1,
        const_char * s2,
        const int open, const int gap,
        Matrix matrix not None):
    cdef size_t t1 = len(s1)
    cdef size_t t2 = len(s2)
    cdef int l1 = <int>t1
    cdef int l2 = <int>t2
    assert l1 == t1
    assert l2 == t2
    return Result.create(parasail_nw_stats_scan_64(s1,l1,s2,l2,open,gap,matrix._c_object))

def nw_stats_scan_32(
        const_char * s1,
        const_char * s2,
        const int open, const int gap,
        Matrix matrix not None):
    cdef size_t t1 = len(s1)
    cdef size_t t2 = len(s2)
    cdef int l1 = <int>t1
    cdef int l2 = <int>t2
    assert l1 == t1
    assert l2 == t2
    return Result.create(parasail_nw_stats_scan_32(s1,l1,s2,l2,open,gap,matrix._c_object))

def nw_stats_scan_16(
        const_char * s1,
        const_char * s2,
        const int open, const int gap,
        Matrix matrix not None):
    cdef size_t t1 = len(s1)
    cdef size_t t2 = len(s2)
    cdef int l1 = <int>t1
    cdef int l2 = <int>t2
    assert l1 == t1
    assert l2 == t2
    return Result.create(parasail_nw_stats_scan_16(s1,l1,s2,l2,open,gap,matrix._c_object))

def nw_stats_scan_8(
        const_char * s1,
        const_char * s2,
        const int open, const int gap,
        Matrix matrix not None):
    cdef size_t t1 = len(s1)
    cdef size_t t2 = len(s2)
    cdef int l1 = <int>t1
    cdef int l2 = <int>t2
    assert l1 == t1
    assert l2 == t2
    return Result.create(parasail_nw_stats_scan_8(s1,l1,s2,l2,open,gap,matrix._c_object))

def nw_stats_striped_64(
        const_char * s1,
        const_char * s2,
        const int open, const int gap,
        Matrix matrix not None):
    cdef size_t t1 = len(s1)
    cdef size_t t2 = len(s2)
    cdef int l1 = <int>t1
    cdef int l2 = <int>t2
    assert l1 == t1
    assert l2 == t2
    return Result.create(parasail_nw_stats_striped_64(s1,l1,s2,l2,open,gap,matrix._c_object))

def nw_stats_striped_32(
        const_char * s1,
        const_char * s2,
        const int open, const int gap,
        Matrix matrix not None):
    cdef size_t t1 = len(s1)
    cdef size_t t2 = len(s2)
    cdef int l1 = <int>t1
    cdef int l2 = <int>t2
    assert l1 == t1
    assert l2 == t2
    return Result.create(parasail_nw_stats_striped_32(s1,l1,s2,l2,open,gap,matrix._c_object))

def nw_stats_striped_16(
        const_char * s1,
        const_char * s2,
        const int open, const int gap,
        Matrix matrix not None):
    cdef size_t t1 = len(s1)
    cdef size_t t2 = len(s2)
    cdef int l1 = <int>t1
    cdef int l2 = <int>t2
    assert l1 == t1
    assert l2 == t2
    return Result.create(parasail_nw_stats_striped_16(s1,l1,s2,l2,open,gap,matrix._c_object))

def nw_stats_striped_8(
        const_char * s1,
        const_char * s2,
        const int open, const int gap,
        Matrix matrix not None):
    cdef size_t t1 = len(s1)
    cdef size_t t2 = len(s2)
    cdef int l1 = <int>t1
    cdef int l2 = <int>t2
    assert l1 == t1
    assert l2 == t2
    return Result.create(parasail_nw_stats_striped_8(s1,l1,s2,l2,open,gap,matrix._c_object))

def nw_stats_diag_64(
        const_char * s1,
        const_char * s2,
        const int open, const int gap,
        Matrix matrix not None):
    cdef size_t t1 = len(s1)
    cdef size_t t2 = len(s2)
    cdef int l1 = <int>t1
    cdef int l2 = <int>t2
    assert l1 == t1
    assert l2 == t2
    return Result.create(parasail_nw_stats_diag_64(s1,l1,s2,l2,open,gap,matrix._c_object))

def nw_stats_diag_32(
        const_char * s1,
        const_char * s2,
        const int open, const int gap,
        Matrix matrix not None):
    cdef size_t t1 = len(s1)
    cdef size_t t2 = len(s2)
    cdef int l1 = <int>t1
    cdef int l2 = <int>t2
    assert l1 == t1
    assert l2 == t2
    return Result.create(parasail_nw_stats_diag_32(s1,l1,s2,l2,open,gap,matrix._c_object))

def nw_stats_diag_16(
        const_char * s1,
        const_char * s2,
        const int open, const int gap,
        Matrix matrix not None):
    cdef size_t t1 = len(s1)
    cdef size_t t2 = len(s2)
    cdef int l1 = <int>t1
    cdef int l2 = <int>t2
    assert l1 == t1
    assert l2 == t2
    return Result.create(parasail_nw_stats_diag_16(s1,l1,s2,l2,open,gap,matrix._c_object))

def nw_stats_diag_8(
        const_char * s1,
        const_char * s2,
        const int open, const int gap,
        Matrix matrix not None):
    cdef size_t t1 = len(s1)
    cdef size_t t2 = len(s2)
    cdef int l1 = <int>t1
    cdef int l2 = <int>t2
    assert l1 == t1
    assert l2 == t2
    return Result.create(parasail_nw_stats_diag_8(s1,l1,s2,l2,open,gap,matrix._c_object))

def nw_stats_table_scan_64(
        const_char * s1,
        const_char * s2,
        const int open, const int gap,
        Matrix matrix not None):
    cdef size_t t1 = len(s1)
    cdef size_t t2 = len(s2)
    cdef int l1 = <int>t1
    cdef int l2 = <int>t2
    assert l1 == t1
    assert l2 == t2
    return Result.create(parasail_nw_stats_table_scan_64(s1,l1,s2,l2,open,gap,matrix._c_object))

def nw_stats_table_scan_32(
        const_char * s1,
        const_char * s2,
        const int open, const int gap,
        Matrix matrix not None):
    cdef size_t t1 = len(s1)
    cdef size_t t2 = len(s2)
    cdef int l1 = <int>t1
    cdef int l2 = <int>t2
    assert l1 == t1
    assert l2 == t2
    return Result.create(parasail_nw_stats_table_scan_32(s1,l1,s2,l2,open,gap,matrix._c_object))

def nw_stats_table_scan_16(
        const_char * s1,
        const_char * s2,
        const int open, const int gap,
        Matrix matrix not None):
    cdef size_t t1 = len(s1)
    cdef size_t t2 = len(s2)
    cdef int l1 = <int>t1
    cdef int l2 = <int>t2
    assert l1 == t1
    assert l2 == t2
    return Result.create(parasail_nw_stats_table_scan_16(s1,l1,s2,l2,open,gap,matrix._c_object))

def nw_stats_table_scan_8(
        const_char * s1,
        const_char * s2,
        const int open, const int gap,
        Matrix matrix not None):
    cdef size_t t1 = len(s1)
    cdef size_t t2 = len(s2)
    cdef int l1 = <int>t1
    cdef int l2 = <int>t2
    assert l1 == t1
    assert l2 == t2
    return Result.create(parasail_nw_stats_table_scan_8(s1,l1,s2,l2,open,gap,matrix._c_object))

def nw_stats_table_striped_64(
        const_char * s1,
        const_char * s2,
        const int open, const int gap,
        Matrix matrix not None):
    cdef size_t t1 = len(s1)
    cdef size_t t2 = len(s2)
    cdef int l1 = <int>t1
    cdef int l2 = <int>t2
    assert l1 == t1
    assert l2 == t2
    return Result.create(parasail_nw_stats_table_striped_64(s1,l1,s2,l2,open,gap,matrix._c_object))

def nw_stats_table_striped_32(
        const_char * s1,
        const_char * s2,
        const int open, const int gap,
        Matrix matrix not None):
    cdef size_t t1 = len(s1)
    cdef size_t t2 = len(s2)
    cdef int l1 = <int>t1
    cdef int l2 = <int>t2
    assert l1 == t1
    assert l2 == t2
    return Result.create(parasail_nw_stats_table_striped_32(s1,l1,s2,l2,open,gap,matrix._c_object))

def nw_stats_table_striped_16(
        const_char * s1,
        const_char * s2,
        const int open, const int gap,
        Matrix matrix not None):
    cdef size_t t1 = len(s1)
    cdef size_t t2 = len(s2)
    cdef int l1 = <int>t1
    cdef int l2 = <int>t2
    assert l1 == t1
    assert l2 == t2
    return Result.create(parasail_nw_stats_table_striped_16(s1,l1,s2,l2,open,gap,matrix._c_object))

def nw_stats_table_striped_8(
        const_char * s1,
        const_char * s2,
        const int open, const int gap,
        Matrix matrix not None):
    cdef size_t t1 = len(s1)
    cdef size_t t2 = len(s2)
    cdef int l1 = <int>t1
    cdef int l2 = <int>t2
    assert l1 == t1
    assert l2 == t2
    return Result.create(parasail_nw_stats_table_striped_8(s1,l1,s2,l2,open,gap,matrix._c_object))

def nw_stats_table_diag_64(
        const_char * s1,
        const_char * s2,
        const int open, const int gap,
        Matrix matrix not None):
    cdef size_t t1 = len(s1)
    cdef size_t t2 = len(s2)
    cdef int l1 = <int>t1
    cdef int l2 = <int>t2
    assert l1 == t1
    assert l2 == t2
    return Result.create(parasail_nw_stats_table_diag_64(s1,l1,s2,l2,open,gap,matrix._c_object))

def nw_stats_table_diag_32(
        const_char * s1,
        const_char * s2,
        const int open, const int gap,
        Matrix matrix not None):
    cdef size_t t1 = len(s1)
    cdef size_t t2 = len(s2)
    cdef int l1 = <int>t1
    cdef int l2 = <int>t2
    assert l1 == t1
    assert l2 == t2
    return Result.create(parasail_nw_stats_table_diag_32(s1,l1,s2,l2,open,gap,matrix._c_object))

def nw_stats_table_diag_16(
        const_char * s1,
        const_char * s2,
        const int open, const int gap,
        Matrix matrix not None):
    cdef size_t t1 = len(s1)
    cdef size_t t2 = len(s2)
    cdef int l1 = <int>t1
    cdef int l2 = <int>t2
    assert l1 == t1
    assert l2 == t2
    return Result.create(parasail_nw_stats_table_diag_16(s1,l1,s2,l2,open,gap,matrix._c_object))

def nw_stats_table_diag_8(
        const_char * s1,
        const_char * s2,
        const int open, const int gap,
        Matrix matrix not None):
    cdef size_t t1 = len(s1)
    cdef size_t t2 = len(s2)
    cdef int l1 = <int>t1
    cdef int l2 = <int>t2
    assert l1 == t1
    assert l2 == t2
    return Result.create(parasail_nw_stats_table_diag_8(s1,l1,s2,l2,open,gap,matrix._c_object))

def nw_stats_rowcol_scan_64(
        const_char * s1,
        const_char * s2,
        const int open, const int gap,
        Matrix matrix not None):
    cdef size_t t1 = len(s1)
    cdef size_t t2 = len(s2)
    cdef int l1 = <int>t1
    cdef int l2 = <int>t2
    assert l1 == t1
    assert l2 == t2
    return Result.create(parasail_nw_stats_rowcol_scan_64(s1,l1,s2,l2,open,gap,matrix._c_object))

def nw_stats_rowcol_scan_32(
        const_char * s1,
        const_char * s2,
        const int open, const int gap,
        Matrix matrix not None):
    cdef size_t t1 = len(s1)
    cdef size_t t2 = len(s2)
    cdef int l1 = <int>t1
    cdef int l2 = <int>t2
    assert l1 == t1
    assert l2 == t2
    return Result.create(parasail_nw_stats_rowcol_scan_32(s1,l1,s2,l2,open,gap,matrix._c_object))

def nw_stats_rowcol_scan_16(
        const_char * s1,
        const_char * s2,
        const int open, const int gap,
        Matrix matrix not None):
    cdef size_t t1 = len(s1)
    cdef size_t t2 = len(s2)
    cdef int l1 = <int>t1
    cdef int l2 = <int>t2
    assert l1 == t1
    assert l2 == t2
    return Result.create(parasail_nw_stats_rowcol_scan_16(s1,l1,s2,l2,open,gap,matrix._c_object))

def nw_stats_rowcol_scan_8(
        const_char * s1,
        const_char * s2,
        const int open, const int gap,
        Matrix matrix not None):
    cdef size_t t1 = len(s1)
    cdef size_t t2 = len(s2)
    cdef int l1 = <int>t1
    cdef int l2 = <int>t2
    assert l1 == t1
    assert l2 == t2
    return Result.create(parasail_nw_stats_rowcol_scan_8(s1,l1,s2,l2,open,gap,matrix._c_object))

def nw_stats_rowcol_striped_64(
        const_char * s1,
        const_char * s2,
        const int open, const int gap,
        Matrix matrix not None):
    cdef size_t t1 = len(s1)
    cdef size_t t2 = len(s2)
    cdef int l1 = <int>t1
    cdef int l2 = <int>t2
    assert l1 == t1
    assert l2 == t2
    return Result.create(parasail_nw_stats_rowcol_striped_64(s1,l1,s2,l2,open,gap,matrix._c_object))

def nw_stats_rowcol_striped_32(
        const_char * s1,
        const_char * s2,
        const int open, const int gap,
        Matrix matrix not None):
    cdef size_t t1 = len(s1)
    cdef size_t t2 = len(s2)
    cdef int l1 = <int>t1
    cdef int l2 = <int>t2
    assert l1 == t1
    assert l2 == t2
    return Result.create(parasail_nw_stats_rowcol_striped_32(s1,l1,s2,l2,open,gap,matrix._c_object))

def nw_stats_rowcol_striped_16(
        const_char * s1,
        const_char * s2,
        const int open, const int gap,
        Matrix matrix not None):
    cdef size_t t1 = len(s1)
    cdef size_t t2 = len(s2)
    cdef int l1 = <int>t1
    cdef int l2 = <int>t2
    assert l1 == t1
    assert l2 == t2
    return Result.create(parasail_nw_stats_rowcol_striped_16(s1,l1,s2,l2,open,gap,matrix._c_object))

def nw_stats_rowcol_striped_8(
        const_char * s1,
        const_char * s2,
        const int open, const int gap,
        Matrix matrix not None):
    cdef size_t t1 = len(s1)
    cdef size_t t2 = len(s2)
    cdef int l1 = <int>t1
    cdef int l2 = <int>t2
    assert l1 == t1
    assert l2 == t2
    return Result.create(parasail_nw_stats_rowcol_striped_8(s1,l1,s2,l2,open,gap,matrix._c_object))

def nw_stats_rowcol_diag_64(
        const_char * s1,
        const_char * s2,
        const int open, const int gap,
        Matrix matrix not None):
    cdef size_t t1 = len(s1)
    cdef size_t t2 = len(s2)
    cdef int l1 = <int>t1
    cdef int l2 = <int>t2
    assert l1 == t1
    assert l2 == t2
    return Result.create(parasail_nw_stats_rowcol_diag_64(s1,l1,s2,l2,open,gap,matrix._c_object))

def nw_stats_rowcol_diag_32(
        const_char * s1,
        const_char * s2,
        const int open, const int gap,
        Matrix matrix not None):
    cdef size_t t1 = len(s1)
    cdef size_t t2 = len(s2)
    cdef int l1 = <int>t1
    cdef int l2 = <int>t2
    assert l1 == t1
    assert l2 == t2
    return Result.create(parasail_nw_stats_rowcol_diag_32(s1,l1,s2,l2,open,gap,matrix._c_object))

def nw_stats_rowcol_diag_16(
        const_char * s1,
        const_char * s2,
        const int open, const int gap,
        Matrix matrix not None):
    cdef size_t t1 = len(s1)
    cdef size_t t2 = len(s2)
    cdef int l1 = <int>t1
    cdef int l2 = <int>t2
    assert l1 == t1
    assert l2 == t2
    return Result.create(parasail_nw_stats_rowcol_diag_16(s1,l1,s2,l2,open,gap,matrix._c_object))

def nw_stats_rowcol_diag_8(
        const_char * s1,
        const_char * s2,
        const int open, const int gap,
        Matrix matrix not None):
    cdef size_t t1 = len(s1)
    cdef size_t t2 = len(s2)
    cdef int l1 = <int>t1
    cdef int l2 = <int>t2
    assert l1 == t1
    assert l2 == t2
    return Result.create(parasail_nw_stats_rowcol_diag_8(s1,l1,s2,l2,open,gap,matrix._c_object))

def sg_scan_64(
        const_char * s1,
        const_char * s2,
        const int open, const int gap,
        Matrix matrix not None):
    cdef size_t t1 = len(s1)
    cdef size_t t2 = len(s2)
    cdef int l1 = <int>t1
    cdef int l2 = <int>t2
    assert l1 == t1
    assert l2 == t2
    return Result.create(parasail_sg_scan_64(s1,l1,s2,l2,open,gap,matrix._c_object))

def sg_scan_32(
        const_char * s1,
        const_char * s2,
        const int open, const int gap,
        Matrix matrix not None):
    cdef size_t t1 = len(s1)
    cdef size_t t2 = len(s2)
    cdef int l1 = <int>t1
    cdef int l2 = <int>t2
    assert l1 == t1
    assert l2 == t2
    return Result.create(parasail_sg_scan_32(s1,l1,s2,l2,open,gap,matrix._c_object))

def sg_scan_16(
        const_char * s1,
        const_char * s2,
        const int open, const int gap,
        Matrix matrix not None):
    cdef size_t t1 = len(s1)
    cdef size_t t2 = len(s2)
    cdef int l1 = <int>t1
    cdef int l2 = <int>t2
    assert l1 == t1
    assert l2 == t2
    return Result.create(parasail_sg_scan_16(s1,l1,s2,l2,open,gap,matrix._c_object))

def sg_scan_8(
        const_char * s1,
        const_char * s2,
        const int open, const int gap,
        Matrix matrix not None):
    cdef size_t t1 = len(s1)
    cdef size_t t2 = len(s2)
    cdef int l1 = <int>t1
    cdef int l2 = <int>t2
    assert l1 == t1
    assert l2 == t2
    return Result.create(parasail_sg_scan_8(s1,l1,s2,l2,open,gap,matrix._c_object))

def sg_striped_64(
        const_char * s1,
        const_char * s2,
        const int open, const int gap,
        Matrix matrix not None):
    cdef size_t t1 = len(s1)
    cdef size_t t2 = len(s2)
    cdef int l1 = <int>t1
    cdef int l2 = <int>t2
    assert l1 == t1
    assert l2 == t2
    return Result.create(parasail_sg_striped_64(s1,l1,s2,l2,open,gap,matrix._c_object))

def sg_striped_32(
        const_char * s1,
        const_char * s2,
        const int open, const int gap,
        Matrix matrix not None):
    cdef size_t t1 = len(s1)
    cdef size_t t2 = len(s2)
    cdef int l1 = <int>t1
    cdef int l2 = <int>t2
    assert l1 == t1
    assert l2 == t2
    return Result.create(parasail_sg_striped_32(s1,l1,s2,l2,open,gap,matrix._c_object))

def sg_striped_16(
        const_char * s1,
        const_char * s2,
        const int open, const int gap,
        Matrix matrix not None):
    cdef size_t t1 = len(s1)
    cdef size_t t2 = len(s2)
    cdef int l1 = <int>t1
    cdef int l2 = <int>t2
    assert l1 == t1
    assert l2 == t2
    return Result.create(parasail_sg_striped_16(s1,l1,s2,l2,open,gap,matrix._c_object))

def sg_striped_8(
        const_char * s1,
        const_char * s2,
        const int open, const int gap,
        Matrix matrix not None):
    cdef size_t t1 = len(s1)
    cdef size_t t2 = len(s2)
    cdef int l1 = <int>t1
    cdef int l2 = <int>t2
    assert l1 == t1
    assert l2 == t2
    return Result.create(parasail_sg_striped_8(s1,l1,s2,l2,open,gap,matrix._c_object))

def sg_diag_64(
        const_char * s1,
        const_char * s2,
        const int open, const int gap,
        Matrix matrix not None):
    cdef size_t t1 = len(s1)
    cdef size_t t2 = len(s2)
    cdef int l1 = <int>t1
    cdef int l2 = <int>t2
    assert l1 == t1
    assert l2 == t2
    return Result.create(parasail_sg_diag_64(s1,l1,s2,l2,open,gap,matrix._c_object))

def sg_diag_32(
        const_char * s1,
        const_char * s2,
        const int open, const int gap,
        Matrix matrix not None):
    cdef size_t t1 = len(s1)
    cdef size_t t2 = len(s2)
    cdef int l1 = <int>t1
    cdef int l2 = <int>t2
    assert l1 == t1
    assert l2 == t2
    return Result.create(parasail_sg_diag_32(s1,l1,s2,l2,open,gap,matrix._c_object))

def sg_diag_16(
        const_char * s1,
        const_char * s2,
        const int open, const int gap,
        Matrix matrix not None):
    cdef size_t t1 = len(s1)
    cdef size_t t2 = len(s2)
    cdef int l1 = <int>t1
    cdef int l2 = <int>t2
    assert l1 == t1
    assert l2 == t2
    return Result.create(parasail_sg_diag_16(s1,l1,s2,l2,open,gap,matrix._c_object))

def sg_diag_8(
        const_char * s1,
        const_char * s2,
        const int open, const int gap,
        Matrix matrix not None):
    cdef size_t t1 = len(s1)
    cdef size_t t2 = len(s2)
    cdef int l1 = <int>t1
    cdef int l2 = <int>t2
    assert l1 == t1
    assert l2 == t2
    return Result.create(parasail_sg_diag_8(s1,l1,s2,l2,open,gap,matrix._c_object))

def sg_table_scan_64(
        const_char * s1,
        const_char * s2,
        const int open, const int gap,
        Matrix matrix not None):
    cdef size_t t1 = len(s1)
    cdef size_t t2 = len(s2)
    cdef int l1 = <int>t1
    cdef int l2 = <int>t2
    assert l1 == t1
    assert l2 == t2
    return Result.create(parasail_sg_table_scan_64(s1,l1,s2,l2,open,gap,matrix._c_object))

def sg_table_scan_32(
        const_char * s1,
        const_char * s2,
        const int open, const int gap,
        Matrix matrix not None):
    cdef size_t t1 = len(s1)
    cdef size_t t2 = len(s2)
    cdef int l1 = <int>t1
    cdef int l2 = <int>t2
    assert l1 == t1
    assert l2 == t2
    return Result.create(parasail_sg_table_scan_32(s1,l1,s2,l2,open,gap,matrix._c_object))

def sg_table_scan_16(
        const_char * s1,
        const_char * s2,
        const int open, const int gap,
        Matrix matrix not None):
    cdef size_t t1 = len(s1)
    cdef size_t t2 = len(s2)
    cdef int l1 = <int>t1
    cdef int l2 = <int>t2
    assert l1 == t1
    assert l2 == t2
    return Result.create(parasail_sg_table_scan_16(s1,l1,s2,l2,open,gap,matrix._c_object))

def sg_table_scan_8(
        const_char * s1,
        const_char * s2,
        const int open, const int gap,
        Matrix matrix not None):
    cdef size_t t1 = len(s1)
    cdef size_t t2 = len(s2)
    cdef int l1 = <int>t1
    cdef int l2 = <int>t2
    assert l1 == t1
    assert l2 == t2
    return Result.create(parasail_sg_table_scan_8(s1,l1,s2,l2,open,gap,matrix._c_object))

def sg_table_striped_64(
        const_char * s1,
        const_char * s2,
        const int open, const int gap,
        Matrix matrix not None):
    cdef size_t t1 = len(s1)
    cdef size_t t2 = len(s2)
    cdef int l1 = <int>t1
    cdef int l2 = <int>t2
    assert l1 == t1
    assert l2 == t2
    return Result.create(parasail_sg_table_striped_64(s1,l1,s2,l2,open,gap,matrix._c_object))

def sg_table_striped_32(
        const_char * s1,
        const_char * s2,
        const int open, const int gap,
        Matrix matrix not None):
    cdef size_t t1 = len(s1)
    cdef size_t t2 = len(s2)
    cdef int l1 = <int>t1
    cdef int l2 = <int>t2
    assert l1 == t1
    assert l2 == t2
    return Result.create(parasail_sg_table_striped_32(s1,l1,s2,l2,open,gap,matrix._c_object))

def sg_table_striped_16(
        const_char * s1,
        const_char * s2,
        const int open, const int gap,
        Matrix matrix not None):
    cdef size_t t1 = len(s1)
    cdef size_t t2 = len(s2)
    cdef int l1 = <int>t1
    cdef int l2 = <int>t2
    assert l1 == t1
    assert l2 == t2
    return Result.create(parasail_sg_table_striped_16(s1,l1,s2,l2,open,gap,matrix._c_object))

def sg_table_striped_8(
        const_char * s1,
        const_char * s2,
        const int open, const int gap,
        Matrix matrix not None):
    cdef size_t t1 = len(s1)
    cdef size_t t2 = len(s2)
    cdef int l1 = <int>t1
    cdef int l2 = <int>t2
    assert l1 == t1
    assert l2 == t2
    return Result.create(parasail_sg_table_striped_8(s1,l1,s2,l2,open,gap,matrix._c_object))

def sg_table_diag_64(
        const_char * s1,
        const_char * s2,
        const int open, const int gap,
        Matrix matrix not None):
    cdef size_t t1 = len(s1)
    cdef size_t t2 = len(s2)
    cdef int l1 = <int>t1
    cdef int l2 = <int>t2
    assert l1 == t1
    assert l2 == t2
    return Result.create(parasail_sg_table_diag_64(s1,l1,s2,l2,open,gap,matrix._c_object))

def sg_table_diag_32(
        const_char * s1,
        const_char * s2,
        const int open, const int gap,
        Matrix matrix not None):
    cdef size_t t1 = len(s1)
    cdef size_t t2 = len(s2)
    cdef int l1 = <int>t1
    cdef int l2 = <int>t2
    assert l1 == t1
    assert l2 == t2
    return Result.create(parasail_sg_table_diag_32(s1,l1,s2,l2,open,gap,matrix._c_object))

def sg_table_diag_16(
        const_char * s1,
        const_char * s2,
        const int open, const int gap,
        Matrix matrix not None):
    cdef size_t t1 = len(s1)
    cdef size_t t2 = len(s2)
    cdef int l1 = <int>t1
    cdef int l2 = <int>t2
    assert l1 == t1
    assert l2 == t2
    return Result.create(parasail_sg_table_diag_16(s1,l1,s2,l2,open,gap,matrix._c_object))

def sg_table_diag_8(
        const_char * s1,
        const_char * s2,
        const int open, const int gap,
        Matrix matrix not None):
    cdef size_t t1 = len(s1)
    cdef size_t t2 = len(s2)
    cdef int l1 = <int>t1
    cdef int l2 = <int>t2
    assert l1 == t1
    assert l2 == t2
    return Result.create(parasail_sg_table_diag_8(s1,l1,s2,l2,open,gap,matrix._c_object))

def sg_rowcol_scan_64(
        const_char * s1,
        const_char * s2,
        const int open, const int gap,
        Matrix matrix not None):
    cdef size_t t1 = len(s1)
    cdef size_t t2 = len(s2)
    cdef int l1 = <int>t1
    cdef int l2 = <int>t2
    assert l1 == t1
    assert l2 == t2
    return Result.create(parasail_sg_rowcol_scan_64(s1,l1,s2,l2,open,gap,matrix._c_object))

def sg_rowcol_scan_32(
        const_char * s1,
        const_char * s2,
        const int open, const int gap,
        Matrix matrix not None):
    cdef size_t t1 = len(s1)
    cdef size_t t2 = len(s2)
    cdef int l1 = <int>t1
    cdef int l2 = <int>t2
    assert l1 == t1
    assert l2 == t2
    return Result.create(parasail_sg_rowcol_scan_32(s1,l1,s2,l2,open,gap,matrix._c_object))

def sg_rowcol_scan_16(
        const_char * s1,
        const_char * s2,
        const int open, const int gap,
        Matrix matrix not None):
    cdef size_t t1 = len(s1)
    cdef size_t t2 = len(s2)
    cdef int l1 = <int>t1
    cdef int l2 = <int>t2
    assert l1 == t1
    assert l2 == t2
    return Result.create(parasail_sg_rowcol_scan_16(s1,l1,s2,l2,open,gap,matrix._c_object))

def sg_rowcol_scan_8(
        const_char * s1,
        const_char * s2,
        const int open, const int gap,
        Matrix matrix not None):
    cdef size_t t1 = len(s1)
    cdef size_t t2 = len(s2)
    cdef int l1 = <int>t1
    cdef int l2 = <int>t2
    assert l1 == t1
    assert l2 == t2
    return Result.create(parasail_sg_rowcol_scan_8(s1,l1,s2,l2,open,gap,matrix._c_object))

def sg_rowcol_striped_64(
        const_char * s1,
        const_char * s2,
        const int open, const int gap,
        Matrix matrix not None):
    cdef size_t t1 = len(s1)
    cdef size_t t2 = len(s2)
    cdef int l1 = <int>t1
    cdef int l2 = <int>t2
    assert l1 == t1
    assert l2 == t2
    return Result.create(parasail_sg_rowcol_striped_64(s1,l1,s2,l2,open,gap,matrix._c_object))

def sg_rowcol_striped_32(
        const_char * s1,
        const_char * s2,
        const int open, const int gap,
        Matrix matrix not None):
    cdef size_t t1 = len(s1)
    cdef size_t t2 = len(s2)
    cdef int l1 = <int>t1
    cdef int l2 = <int>t2
    assert l1 == t1
    assert l2 == t2
    return Result.create(parasail_sg_rowcol_striped_32(s1,l1,s2,l2,open,gap,matrix._c_object))

def sg_rowcol_striped_16(
        const_char * s1,
        const_char * s2,
        const int open, const int gap,
        Matrix matrix not None):
    cdef size_t t1 = len(s1)
    cdef size_t t2 = len(s2)
    cdef int l1 = <int>t1
    cdef int l2 = <int>t2
    assert l1 == t1
    assert l2 == t2
    return Result.create(parasail_sg_rowcol_striped_16(s1,l1,s2,l2,open,gap,matrix._c_object))

def sg_rowcol_striped_8(
        const_char * s1,
        const_char * s2,
        const int open, const int gap,
        Matrix matrix not None):
    cdef size_t t1 = len(s1)
    cdef size_t t2 = len(s2)
    cdef int l1 = <int>t1
    cdef int l2 = <int>t2
    assert l1 == t1
    assert l2 == t2
    return Result.create(parasail_sg_rowcol_striped_8(s1,l1,s2,l2,open,gap,matrix._c_object))

def sg_rowcol_diag_64(
        const_char * s1,
        const_char * s2,
        const int open, const int gap,
        Matrix matrix not None):
    cdef size_t t1 = len(s1)
    cdef size_t t2 = len(s2)
    cdef int l1 = <int>t1
    cdef int l2 = <int>t2
    assert l1 == t1
    assert l2 == t2
    return Result.create(parasail_sg_rowcol_diag_64(s1,l1,s2,l2,open,gap,matrix._c_object))

def sg_rowcol_diag_32(
        const_char * s1,
        const_char * s2,
        const int open, const int gap,
        Matrix matrix not None):
    cdef size_t t1 = len(s1)
    cdef size_t t2 = len(s2)
    cdef int l1 = <int>t1
    cdef int l2 = <int>t2
    assert l1 == t1
    assert l2 == t2
    return Result.create(parasail_sg_rowcol_diag_32(s1,l1,s2,l2,open,gap,matrix._c_object))

def sg_rowcol_diag_16(
        const_char * s1,
        const_char * s2,
        const int open, const int gap,
        Matrix matrix not None):
    cdef size_t t1 = len(s1)
    cdef size_t t2 = len(s2)
    cdef int l1 = <int>t1
    cdef int l2 = <int>t2
    assert l1 == t1
    assert l2 == t2
    return Result.create(parasail_sg_rowcol_diag_16(s1,l1,s2,l2,open,gap,matrix._c_object))

def sg_rowcol_diag_8(
        const_char * s1,
        const_char * s2,
        const int open, const int gap,
        Matrix matrix not None):
    cdef size_t t1 = len(s1)
    cdef size_t t2 = len(s2)
    cdef int l1 = <int>t1
    cdef int l2 = <int>t2
    assert l1 == t1
    assert l2 == t2
    return Result.create(parasail_sg_rowcol_diag_8(s1,l1,s2,l2,open,gap,matrix._c_object))

def sg_stats_scan_64(
        const_char * s1,
        const_char * s2,
        const int open, const int gap,
        Matrix matrix not None):
    cdef size_t t1 = len(s1)
    cdef size_t t2 = len(s2)
    cdef int l1 = <int>t1
    cdef int l2 = <int>t2
    assert l1 == t1
    assert l2 == t2
    return Result.create(parasail_sg_stats_scan_64(s1,l1,s2,l2,open,gap,matrix._c_object))

def sg_stats_scan_32(
        const_char * s1,
        const_char * s2,
        const int open, const int gap,
        Matrix matrix not None):
    cdef size_t t1 = len(s1)
    cdef size_t t2 = len(s2)
    cdef int l1 = <int>t1
    cdef int l2 = <int>t2
    assert l1 == t1
    assert l2 == t2
    return Result.create(parasail_sg_stats_scan_32(s1,l1,s2,l2,open,gap,matrix._c_object))

def sg_stats_scan_16(
        const_char * s1,
        const_char * s2,
        const int open, const int gap,
        Matrix matrix not None):
    cdef size_t t1 = len(s1)
    cdef size_t t2 = len(s2)
    cdef int l1 = <int>t1
    cdef int l2 = <int>t2
    assert l1 == t1
    assert l2 == t2
    return Result.create(parasail_sg_stats_scan_16(s1,l1,s2,l2,open,gap,matrix._c_object))

def sg_stats_scan_8(
        const_char * s1,
        const_char * s2,
        const int open, const int gap,
        Matrix matrix not None):
    cdef size_t t1 = len(s1)
    cdef size_t t2 = len(s2)
    cdef int l1 = <int>t1
    cdef int l2 = <int>t2
    assert l1 == t1
    assert l2 == t2
    return Result.create(parasail_sg_stats_scan_8(s1,l1,s2,l2,open,gap,matrix._c_object))

def sg_stats_striped_64(
        const_char * s1,
        const_char * s2,
        const int open, const int gap,
        Matrix matrix not None):
    cdef size_t t1 = len(s1)
    cdef size_t t2 = len(s2)
    cdef int l1 = <int>t1
    cdef int l2 = <int>t2
    assert l1 == t1
    assert l2 == t2
    return Result.create(parasail_sg_stats_striped_64(s1,l1,s2,l2,open,gap,matrix._c_object))

def sg_stats_striped_32(
        const_char * s1,
        const_char * s2,
        const int open, const int gap,
        Matrix matrix not None):
    cdef size_t t1 = len(s1)
    cdef size_t t2 = len(s2)
    cdef int l1 = <int>t1
    cdef int l2 = <int>t2
    assert l1 == t1
    assert l2 == t2
    return Result.create(parasail_sg_stats_striped_32(s1,l1,s2,l2,open,gap,matrix._c_object))

def sg_stats_striped_16(
        const_char * s1,
        const_char * s2,
        const int open, const int gap,
        Matrix matrix not None):
    cdef size_t t1 = len(s1)
    cdef size_t t2 = len(s2)
    cdef int l1 = <int>t1
    cdef int l2 = <int>t2
    assert l1 == t1
    assert l2 == t2
    return Result.create(parasail_sg_stats_striped_16(s1,l1,s2,l2,open,gap,matrix._c_object))

def sg_stats_striped_8(
        const_char * s1,
        const_char * s2,
        const int open, const int gap,
        Matrix matrix not None):
    cdef size_t t1 = len(s1)
    cdef size_t t2 = len(s2)
    cdef int l1 = <int>t1
    cdef int l2 = <int>t2
    assert l1 == t1
    assert l2 == t2
    return Result.create(parasail_sg_stats_striped_8(s1,l1,s2,l2,open,gap,matrix._c_object))

def sg_stats_diag_64(
        const_char * s1,
        const_char * s2,
        const int open, const int gap,
        Matrix matrix not None):
    cdef size_t t1 = len(s1)
    cdef size_t t2 = len(s2)
    cdef int l1 = <int>t1
    cdef int l2 = <int>t2
    assert l1 == t1
    assert l2 == t2
    return Result.create(parasail_sg_stats_diag_64(s1,l1,s2,l2,open,gap,matrix._c_object))

def sg_stats_diag_32(
        const_char * s1,
        const_char * s2,
        const int open, const int gap,
        Matrix matrix not None):
    cdef size_t t1 = len(s1)
    cdef size_t t2 = len(s2)
    cdef int l1 = <int>t1
    cdef int l2 = <int>t2
    assert l1 == t1
    assert l2 == t2
    return Result.create(parasail_sg_stats_diag_32(s1,l1,s2,l2,open,gap,matrix._c_object))

def sg_stats_diag_16(
        const_char * s1,
        const_char * s2,
        const int open, const int gap,
        Matrix matrix not None):
    cdef size_t t1 = len(s1)
    cdef size_t t2 = len(s2)
    cdef int l1 = <int>t1
    cdef int l2 = <int>t2
    assert l1 == t1
    assert l2 == t2
    return Result.create(parasail_sg_stats_diag_16(s1,l1,s2,l2,open,gap,matrix._c_object))

def sg_stats_diag_8(
        const_char * s1,
        const_char * s2,
        const int open, const int gap,
        Matrix matrix not None):
    cdef size_t t1 = len(s1)
    cdef size_t t2 = len(s2)
    cdef int l1 = <int>t1
    cdef int l2 = <int>t2
    assert l1 == t1
    assert l2 == t2
    return Result.create(parasail_sg_stats_diag_8(s1,l1,s2,l2,open,gap,matrix._c_object))

def sg_stats_table_scan_64(
        const_char * s1,
        const_char * s2,
        const int open, const int gap,
        Matrix matrix not None):
    cdef size_t t1 = len(s1)
    cdef size_t t2 = len(s2)
    cdef int l1 = <int>t1
    cdef int l2 = <int>t2
    assert l1 == t1
    assert l2 == t2
    return Result.create(parasail_sg_stats_table_scan_64(s1,l1,s2,l2,open,gap,matrix._c_object))

def sg_stats_table_scan_32(
        const_char * s1,
        const_char * s2,
        const int open, const int gap,
        Matrix matrix not None):
    cdef size_t t1 = len(s1)
    cdef size_t t2 = len(s2)
    cdef int l1 = <int>t1
    cdef int l2 = <int>t2
    assert l1 == t1
    assert l2 == t2
    return Result.create(parasail_sg_stats_table_scan_32(s1,l1,s2,l2,open,gap,matrix._c_object))

def sg_stats_table_scan_16(
        const_char * s1,
        const_char * s2,
        const int open, const int gap,
        Matrix matrix not None):
    cdef size_t t1 = len(s1)
    cdef size_t t2 = len(s2)
    cdef int l1 = <int>t1
    cdef int l2 = <int>t2
    assert l1 == t1
    assert l2 == t2
    return Result.create(parasail_sg_stats_table_scan_16(s1,l1,s2,l2,open,gap,matrix._c_object))

def sg_stats_table_scan_8(
        const_char * s1,
        const_char * s2,
        const int open, const int gap,
        Matrix matrix not None):
    cdef size_t t1 = len(s1)
    cdef size_t t2 = len(s2)
    cdef int l1 = <int>t1
    cdef int l2 = <int>t2
    assert l1 == t1
    assert l2 == t2
    return Result.create(parasail_sg_stats_table_scan_8(s1,l1,s2,l2,open,gap,matrix._c_object))

def sg_stats_table_striped_64(
        const_char * s1,
        const_char * s2,
        const int open, const int gap,
        Matrix matrix not None):
    cdef size_t t1 = len(s1)
    cdef size_t t2 = len(s2)
    cdef int l1 = <int>t1
    cdef int l2 = <int>t2
    assert l1 == t1
    assert l2 == t2
    return Result.create(parasail_sg_stats_table_striped_64(s1,l1,s2,l2,open,gap,matrix._c_object))

def sg_stats_table_striped_32(
        const_char * s1,
        const_char * s2,
        const int open, const int gap,
        Matrix matrix not None):
    cdef size_t t1 = len(s1)
    cdef size_t t2 = len(s2)
    cdef int l1 = <int>t1
    cdef int l2 = <int>t2
    assert l1 == t1
    assert l2 == t2
    return Result.create(parasail_sg_stats_table_striped_32(s1,l1,s2,l2,open,gap,matrix._c_object))

def sg_stats_table_striped_16(
        const_char * s1,
        const_char * s2,
        const int open, const int gap,
        Matrix matrix not None):
    cdef size_t t1 = len(s1)
    cdef size_t t2 = len(s2)
    cdef int l1 = <int>t1
    cdef int l2 = <int>t2
    assert l1 == t1
    assert l2 == t2
    return Result.create(parasail_sg_stats_table_striped_16(s1,l1,s2,l2,open,gap,matrix._c_object))

def sg_stats_table_striped_8(
        const_char * s1,
        const_char * s2,
        const int open, const int gap,
        Matrix matrix not None):
    cdef size_t t1 = len(s1)
    cdef size_t t2 = len(s2)
    cdef int l1 = <int>t1
    cdef int l2 = <int>t2
    assert l1 == t1
    assert l2 == t2
    return Result.create(parasail_sg_stats_table_striped_8(s1,l1,s2,l2,open,gap,matrix._c_object))

def sg_stats_table_diag_64(
        const_char * s1,
        const_char * s2,
        const int open, const int gap,
        Matrix matrix not None):
    cdef size_t t1 = len(s1)
    cdef size_t t2 = len(s2)
    cdef int l1 = <int>t1
    cdef int l2 = <int>t2
    assert l1 == t1
    assert l2 == t2
    return Result.create(parasail_sg_stats_table_diag_64(s1,l1,s2,l2,open,gap,matrix._c_object))

def sg_stats_table_diag_32(
        const_char * s1,
        const_char * s2,
        const int open, const int gap,
        Matrix matrix not None):
    cdef size_t t1 = len(s1)
    cdef size_t t2 = len(s2)
    cdef int l1 = <int>t1
    cdef int l2 = <int>t2
    assert l1 == t1
    assert l2 == t2
    return Result.create(parasail_sg_stats_table_diag_32(s1,l1,s2,l2,open,gap,matrix._c_object))

def sg_stats_table_diag_16(
        const_char * s1,
        const_char * s2,
        const int open, const int gap,
        Matrix matrix not None):
    cdef size_t t1 = len(s1)
    cdef size_t t2 = len(s2)
    cdef int l1 = <int>t1
    cdef int l2 = <int>t2
    assert l1 == t1
    assert l2 == t2
    return Result.create(parasail_sg_stats_table_diag_16(s1,l1,s2,l2,open,gap,matrix._c_object))

def sg_stats_table_diag_8(
        const_char * s1,
        const_char * s2,
        const int open, const int gap,
        Matrix matrix not None):
    cdef size_t t1 = len(s1)
    cdef size_t t2 = len(s2)
    cdef int l1 = <int>t1
    cdef int l2 = <int>t2
    assert l1 == t1
    assert l2 == t2
    return Result.create(parasail_sg_stats_table_diag_8(s1,l1,s2,l2,open,gap,matrix._c_object))

def sg_stats_rowcol_scan_64(
        const_char * s1,
        const_char * s2,
        const int open, const int gap,
        Matrix matrix not None):
    cdef size_t t1 = len(s1)
    cdef size_t t2 = len(s2)
    cdef int l1 = <int>t1
    cdef int l2 = <int>t2
    assert l1 == t1
    assert l2 == t2
    return Result.create(parasail_sg_stats_rowcol_scan_64(s1,l1,s2,l2,open,gap,matrix._c_object))

def sg_stats_rowcol_scan_32(
        const_char * s1,
        const_char * s2,
        const int open, const int gap,
        Matrix matrix not None):
    cdef size_t t1 = len(s1)
    cdef size_t t2 = len(s2)
    cdef int l1 = <int>t1
    cdef int l2 = <int>t2
    assert l1 == t1
    assert l2 == t2
    return Result.create(parasail_sg_stats_rowcol_scan_32(s1,l1,s2,l2,open,gap,matrix._c_object))

def sg_stats_rowcol_scan_16(
        const_char * s1,
        const_char * s2,
        const int open, const int gap,
        Matrix matrix not None):
    cdef size_t t1 = len(s1)
    cdef size_t t2 = len(s2)
    cdef int l1 = <int>t1
    cdef int l2 = <int>t2
    assert l1 == t1
    assert l2 == t2
    return Result.create(parasail_sg_stats_rowcol_scan_16(s1,l1,s2,l2,open,gap,matrix._c_object))

def sg_stats_rowcol_scan_8(
        const_char * s1,
        const_char * s2,
        const int open, const int gap,
        Matrix matrix not None):
    cdef size_t t1 = len(s1)
    cdef size_t t2 = len(s2)
    cdef int l1 = <int>t1
    cdef int l2 = <int>t2
    assert l1 == t1
    assert l2 == t2
    return Result.create(parasail_sg_stats_rowcol_scan_8(s1,l1,s2,l2,open,gap,matrix._c_object))

def sg_stats_rowcol_striped_64(
        const_char * s1,
        const_char * s2,
        const int open, const int gap,
        Matrix matrix not None):
    cdef size_t t1 = len(s1)
    cdef size_t t2 = len(s2)
    cdef int l1 = <int>t1
    cdef int l2 = <int>t2
    assert l1 == t1
    assert l2 == t2
    return Result.create(parasail_sg_stats_rowcol_striped_64(s1,l1,s2,l2,open,gap,matrix._c_object))

def sg_stats_rowcol_striped_32(
        const_char * s1,
        const_char * s2,
        const int open, const int gap,
        Matrix matrix not None):
    cdef size_t t1 = len(s1)
    cdef size_t t2 = len(s2)
    cdef int l1 = <int>t1
    cdef int l2 = <int>t2
    assert l1 == t1
    assert l2 == t2
    return Result.create(parasail_sg_stats_rowcol_striped_32(s1,l1,s2,l2,open,gap,matrix._c_object))

def sg_stats_rowcol_striped_16(
        const_char * s1,
        const_char * s2,
        const int open, const int gap,
        Matrix matrix not None):
    cdef size_t t1 = len(s1)
    cdef size_t t2 = len(s2)
    cdef int l1 = <int>t1
    cdef int l2 = <int>t2
    assert l1 == t1
    assert l2 == t2
    return Result.create(parasail_sg_stats_rowcol_striped_16(s1,l1,s2,l2,open,gap,matrix._c_object))

def sg_stats_rowcol_striped_8(
        const_char * s1,
        const_char * s2,
        const int open, const int gap,
        Matrix matrix not None):
    cdef size_t t1 = len(s1)
    cdef size_t t2 = len(s2)
    cdef int l1 = <int>t1
    cdef int l2 = <int>t2
    assert l1 == t1
    assert l2 == t2
    return Result.create(parasail_sg_stats_rowcol_striped_8(s1,l1,s2,l2,open,gap,matrix._c_object))

def sg_stats_rowcol_diag_64(
        const_char * s1,
        const_char * s2,
        const int open, const int gap,
        Matrix matrix not None):
    cdef size_t t1 = len(s1)
    cdef size_t t2 = len(s2)
    cdef int l1 = <int>t1
    cdef int l2 = <int>t2
    assert l1 == t1
    assert l2 == t2
    return Result.create(parasail_sg_stats_rowcol_diag_64(s1,l1,s2,l2,open,gap,matrix._c_object))

def sg_stats_rowcol_diag_32(
        const_char * s1,
        const_char * s2,
        const int open, const int gap,
        Matrix matrix not None):
    cdef size_t t1 = len(s1)
    cdef size_t t2 = len(s2)
    cdef int l1 = <int>t1
    cdef int l2 = <int>t2
    assert l1 == t1
    assert l2 == t2
    return Result.create(parasail_sg_stats_rowcol_diag_32(s1,l1,s2,l2,open,gap,matrix._c_object))

def sg_stats_rowcol_diag_16(
        const_char * s1,
        const_char * s2,
        const int open, const int gap,
        Matrix matrix not None):
    cdef size_t t1 = len(s1)
    cdef size_t t2 = len(s2)
    cdef int l1 = <int>t1
    cdef int l2 = <int>t2
    assert l1 == t1
    assert l2 == t2
    return Result.create(parasail_sg_stats_rowcol_diag_16(s1,l1,s2,l2,open,gap,matrix._c_object))

def sg_stats_rowcol_diag_8(
        const_char * s1,
        const_char * s2,
        const int open, const int gap,
        Matrix matrix not None):
    cdef size_t t1 = len(s1)
    cdef size_t t2 = len(s2)
    cdef int l1 = <int>t1
    cdef int l2 = <int>t2
    assert l1 == t1
    assert l2 == t2
    return Result.create(parasail_sg_stats_rowcol_diag_8(s1,l1,s2,l2,open,gap,matrix._c_object))

def sw_scan_64(
        const_char * s1,
        const_char * s2,
        const int open, const int gap,
        Matrix matrix not None):
    cdef size_t t1 = len(s1)
    cdef size_t t2 = len(s2)
    cdef int l1 = <int>t1
    cdef int l2 = <int>t2
    assert l1 == t1
    assert l2 == t2
    return Result.create(parasail_sw_scan_64(s1,l1,s2,l2,open,gap,matrix._c_object))

def sw_scan_32(
        const_char * s1,
        const_char * s2,
        const int open, const int gap,
        Matrix matrix not None):
    cdef size_t t1 = len(s1)
    cdef size_t t2 = len(s2)
    cdef int l1 = <int>t1
    cdef int l2 = <int>t2
    assert l1 == t1
    assert l2 == t2
    return Result.create(parasail_sw_scan_32(s1,l1,s2,l2,open,gap,matrix._c_object))

def sw_scan_16(
        const_char * s1,
        const_char * s2,
        const int open, const int gap,
        Matrix matrix not None):
    cdef size_t t1 = len(s1)
    cdef size_t t2 = len(s2)
    cdef int l1 = <int>t1
    cdef int l2 = <int>t2
    assert l1 == t1
    assert l2 == t2
    return Result.create(parasail_sw_scan_16(s1,l1,s2,l2,open,gap,matrix._c_object))

def sw_scan_8(
        const_char * s1,
        const_char * s2,
        const int open, const int gap,
        Matrix matrix not None):
    cdef size_t t1 = len(s1)
    cdef size_t t2 = len(s2)
    cdef int l1 = <int>t1
    cdef int l2 = <int>t2
    assert l1 == t1
    assert l2 == t2
    return Result.create(parasail_sw_scan_8(s1,l1,s2,l2,open,gap,matrix._c_object))

def sw_striped_64(
        const_char * s1,
        const_char * s2,
        const int open, const int gap,
        Matrix matrix not None):
    cdef size_t t1 = len(s1)
    cdef size_t t2 = len(s2)
    cdef int l1 = <int>t1
    cdef int l2 = <int>t2
    assert l1 == t1
    assert l2 == t2
    return Result.create(parasail_sw_striped_64(s1,l1,s2,l2,open,gap,matrix._c_object))

def sw_striped_32(
        const_char * s1,
        const_char * s2,
        const int open, const int gap,
        Matrix matrix not None):
    cdef size_t t1 = len(s1)
    cdef size_t t2 = len(s2)
    cdef int l1 = <int>t1
    cdef int l2 = <int>t2
    assert l1 == t1
    assert l2 == t2
    return Result.create(parasail_sw_striped_32(s1,l1,s2,l2,open,gap,matrix._c_object))

def sw_striped_16(
        const_char * s1,
        const_char * s2,
        const int open, const int gap,
        Matrix matrix not None):
    cdef size_t t1 = len(s1)
    cdef size_t t2 = len(s2)
    cdef int l1 = <int>t1
    cdef int l2 = <int>t2
    assert l1 == t1
    assert l2 == t2
    return Result.create(parasail_sw_striped_16(s1,l1,s2,l2,open,gap,matrix._c_object))

def sw_striped_8(
        const_char * s1,
        const_char * s2,
        const int open, const int gap,
        Matrix matrix not None):
    cdef size_t t1 = len(s1)
    cdef size_t t2 = len(s2)
    cdef int l1 = <int>t1
    cdef int l2 = <int>t2
    assert l1 == t1
    assert l2 == t2
    return Result.create(parasail_sw_striped_8(s1,l1,s2,l2,open,gap,matrix._c_object))

def sw_diag_64(
        const_char * s1,
        const_char * s2,
        const int open, const int gap,
        Matrix matrix not None):
    cdef size_t t1 = len(s1)
    cdef size_t t2 = len(s2)
    cdef int l1 = <int>t1
    cdef int l2 = <int>t2
    assert l1 == t1
    assert l2 == t2
    return Result.create(parasail_sw_diag_64(s1,l1,s2,l2,open,gap,matrix._c_object))

def sw_diag_32(
        const_char * s1,
        const_char * s2,
        const int open, const int gap,
        Matrix matrix not None):
    cdef size_t t1 = len(s1)
    cdef size_t t2 = len(s2)
    cdef int l1 = <int>t1
    cdef int l2 = <int>t2
    assert l1 == t1
    assert l2 == t2
    return Result.create(parasail_sw_diag_32(s1,l1,s2,l2,open,gap,matrix._c_object))

def sw_diag_16(
        const_char * s1,
        const_char * s2,
        const int open, const int gap,
        Matrix matrix not None):
    cdef size_t t1 = len(s1)
    cdef size_t t2 = len(s2)
    cdef int l1 = <int>t1
    cdef int l2 = <int>t2
    assert l1 == t1
    assert l2 == t2
    return Result.create(parasail_sw_diag_16(s1,l1,s2,l2,open,gap,matrix._c_object))

def sw_diag_8(
        const_char * s1,
        const_char * s2,
        const int open, const int gap,
        Matrix matrix not None):
    cdef size_t t1 = len(s1)
    cdef size_t t2 = len(s2)
    cdef int l1 = <int>t1
    cdef int l2 = <int>t2
    assert l1 == t1
    assert l2 == t2
    return Result.create(parasail_sw_diag_8(s1,l1,s2,l2,open,gap,matrix._c_object))

def sw_table_scan_64(
        const_char * s1,
        const_char * s2,
        const int open, const int gap,
        Matrix matrix not None):
    cdef size_t t1 = len(s1)
    cdef size_t t2 = len(s2)
    cdef int l1 = <int>t1
    cdef int l2 = <int>t2
    assert l1 == t1
    assert l2 == t2
    return Result.create(parasail_sw_table_scan_64(s1,l1,s2,l2,open,gap,matrix._c_object))

def sw_table_scan_32(
        const_char * s1,
        const_char * s2,
        const int open, const int gap,
        Matrix matrix not None):
    cdef size_t t1 = len(s1)
    cdef size_t t2 = len(s2)
    cdef int l1 = <int>t1
    cdef int l2 = <int>t2
    assert l1 == t1
    assert l2 == t2
    return Result.create(parasail_sw_table_scan_32(s1,l1,s2,l2,open,gap,matrix._c_object))

def sw_table_scan_16(
        const_char * s1,
        const_char * s2,
        const int open, const int gap,
        Matrix matrix not None):
    cdef size_t t1 = len(s1)
    cdef size_t t2 = len(s2)
    cdef int l1 = <int>t1
    cdef int l2 = <int>t2
    assert l1 == t1
    assert l2 == t2
    return Result.create(parasail_sw_table_scan_16(s1,l1,s2,l2,open,gap,matrix._c_object))

def sw_table_scan_8(
        const_char * s1,
        const_char * s2,
        const int open, const int gap,
        Matrix matrix not None):
    cdef size_t t1 = len(s1)
    cdef size_t t2 = len(s2)
    cdef int l1 = <int>t1
    cdef int l2 = <int>t2
    assert l1 == t1
    assert l2 == t2
    return Result.create(parasail_sw_table_scan_8(s1,l1,s2,l2,open,gap,matrix._c_object))

def sw_table_striped_64(
        const_char * s1,
        const_char * s2,
        const int open, const int gap,
        Matrix matrix not None):
    cdef size_t t1 = len(s1)
    cdef size_t t2 = len(s2)
    cdef int l1 = <int>t1
    cdef int l2 = <int>t2
    assert l1 == t1
    assert l2 == t2
    return Result.create(parasail_sw_table_striped_64(s1,l1,s2,l2,open,gap,matrix._c_object))

def sw_table_striped_32(
        const_char * s1,
        const_char * s2,
        const int open, const int gap,
        Matrix matrix not None):
    cdef size_t t1 = len(s1)
    cdef size_t t2 = len(s2)
    cdef int l1 = <int>t1
    cdef int l2 = <int>t2
    assert l1 == t1
    assert l2 == t2
    return Result.create(parasail_sw_table_striped_32(s1,l1,s2,l2,open,gap,matrix._c_object))

def sw_table_striped_16(
        const_char * s1,
        const_char * s2,
        const int open, const int gap,
        Matrix matrix not None):
    cdef size_t t1 = len(s1)
    cdef size_t t2 = len(s2)
    cdef int l1 = <int>t1
    cdef int l2 = <int>t2
    assert l1 == t1
    assert l2 == t2
    return Result.create(parasail_sw_table_striped_16(s1,l1,s2,l2,open,gap,matrix._c_object))

def sw_table_striped_8(
        const_char * s1,
        const_char * s2,
        const int open, const int gap,
        Matrix matrix not None):
    cdef size_t t1 = len(s1)
    cdef size_t t2 = len(s2)
    cdef int l1 = <int>t1
    cdef int l2 = <int>t2
    assert l1 == t1
    assert l2 == t2
    return Result.create(parasail_sw_table_striped_8(s1,l1,s2,l2,open,gap,matrix._c_object))

def sw_table_diag_64(
        const_char * s1,
        const_char * s2,
        const int open, const int gap,
        Matrix matrix not None):
    cdef size_t t1 = len(s1)
    cdef size_t t2 = len(s2)
    cdef int l1 = <int>t1
    cdef int l2 = <int>t2
    assert l1 == t1
    assert l2 == t2
    return Result.create(parasail_sw_table_diag_64(s1,l1,s2,l2,open,gap,matrix._c_object))

def sw_table_diag_32(
        const_char * s1,
        const_char * s2,
        const int open, const int gap,
        Matrix matrix not None):
    cdef size_t t1 = len(s1)
    cdef size_t t2 = len(s2)
    cdef int l1 = <int>t1
    cdef int l2 = <int>t2
    assert l1 == t1
    assert l2 == t2
    return Result.create(parasail_sw_table_diag_32(s1,l1,s2,l2,open,gap,matrix._c_object))

def sw_table_diag_16(
        const_char * s1,
        const_char * s2,
        const int open, const int gap,
        Matrix matrix not None):
    cdef size_t t1 = len(s1)
    cdef size_t t2 = len(s2)
    cdef int l1 = <int>t1
    cdef int l2 = <int>t2
    assert l1 == t1
    assert l2 == t2
    return Result.create(parasail_sw_table_diag_16(s1,l1,s2,l2,open,gap,matrix._c_object))

def sw_table_diag_8(
        const_char * s1,
        const_char * s2,
        const int open, const int gap,
        Matrix matrix not None):
    cdef size_t t1 = len(s1)
    cdef size_t t2 = len(s2)
    cdef int l1 = <int>t1
    cdef int l2 = <int>t2
    assert l1 == t1
    assert l2 == t2
    return Result.create(parasail_sw_table_diag_8(s1,l1,s2,l2,open,gap,matrix._c_object))

def sw_rowcol_scan_64(
        const_char * s1,
        const_char * s2,
        const int open, const int gap,
        Matrix matrix not None):
    cdef size_t t1 = len(s1)
    cdef size_t t2 = len(s2)
    cdef int l1 = <int>t1
    cdef int l2 = <int>t2
    assert l1 == t1
    assert l2 == t2
    return Result.create(parasail_sw_rowcol_scan_64(s1,l1,s2,l2,open,gap,matrix._c_object))

def sw_rowcol_scan_32(
        const_char * s1,
        const_char * s2,
        const int open, const int gap,
        Matrix matrix not None):
    cdef size_t t1 = len(s1)
    cdef size_t t2 = len(s2)
    cdef int l1 = <int>t1
    cdef int l2 = <int>t2
    assert l1 == t1
    assert l2 == t2
    return Result.create(parasail_sw_rowcol_scan_32(s1,l1,s2,l2,open,gap,matrix._c_object))

def sw_rowcol_scan_16(
        const_char * s1,
        const_char * s2,
        const int open, const int gap,
        Matrix matrix not None):
    cdef size_t t1 = len(s1)
    cdef size_t t2 = len(s2)
    cdef int l1 = <int>t1
    cdef int l2 = <int>t2
    assert l1 == t1
    assert l2 == t2
    return Result.create(parasail_sw_rowcol_scan_16(s1,l1,s2,l2,open,gap,matrix._c_object))

def sw_rowcol_scan_8(
        const_char * s1,
        const_char * s2,
        const int open, const int gap,
        Matrix matrix not None):
    cdef size_t t1 = len(s1)
    cdef size_t t2 = len(s2)
    cdef int l1 = <int>t1
    cdef int l2 = <int>t2
    assert l1 == t1
    assert l2 == t2
    return Result.create(parasail_sw_rowcol_scan_8(s1,l1,s2,l2,open,gap,matrix._c_object))

def sw_rowcol_striped_64(
        const_char * s1,
        const_char * s2,
        const int open, const int gap,
        Matrix matrix not None):
    cdef size_t t1 = len(s1)
    cdef size_t t2 = len(s2)
    cdef int l1 = <int>t1
    cdef int l2 = <int>t2
    assert l1 == t1
    assert l2 == t2
    return Result.create(parasail_sw_rowcol_striped_64(s1,l1,s2,l2,open,gap,matrix._c_object))

def sw_rowcol_striped_32(
        const_char * s1,
        const_char * s2,
        const int open, const int gap,
        Matrix matrix not None):
    cdef size_t t1 = len(s1)
    cdef size_t t2 = len(s2)
    cdef int l1 = <int>t1
    cdef int l2 = <int>t2
    assert l1 == t1
    assert l2 == t2
    return Result.create(parasail_sw_rowcol_striped_32(s1,l1,s2,l2,open,gap,matrix._c_object))

def sw_rowcol_striped_16(
        const_char * s1,
        const_char * s2,
        const int open, const int gap,
        Matrix matrix not None):
    cdef size_t t1 = len(s1)
    cdef size_t t2 = len(s2)
    cdef int l1 = <int>t1
    cdef int l2 = <int>t2
    assert l1 == t1
    assert l2 == t2
    return Result.create(parasail_sw_rowcol_striped_16(s1,l1,s2,l2,open,gap,matrix._c_object))

def sw_rowcol_striped_8(
        const_char * s1,
        const_char * s2,
        const int open, const int gap,
        Matrix matrix not None):
    cdef size_t t1 = len(s1)
    cdef size_t t2 = len(s2)
    cdef int l1 = <int>t1
    cdef int l2 = <int>t2
    assert l1 == t1
    assert l2 == t2
    return Result.create(parasail_sw_rowcol_striped_8(s1,l1,s2,l2,open,gap,matrix._c_object))

def sw_rowcol_diag_64(
        const_char * s1,
        const_char * s2,
        const int open, const int gap,
        Matrix matrix not None):
    cdef size_t t1 = len(s1)
    cdef size_t t2 = len(s2)
    cdef int l1 = <int>t1
    cdef int l2 = <int>t2
    assert l1 == t1
    assert l2 == t2
    return Result.create(parasail_sw_rowcol_diag_64(s1,l1,s2,l2,open,gap,matrix._c_object))

def sw_rowcol_diag_32(
        const_char * s1,
        const_char * s2,
        const int open, const int gap,
        Matrix matrix not None):
    cdef size_t t1 = len(s1)
    cdef size_t t2 = len(s2)
    cdef int l1 = <int>t1
    cdef int l2 = <int>t2
    assert l1 == t1
    assert l2 == t2
    return Result.create(parasail_sw_rowcol_diag_32(s1,l1,s2,l2,open,gap,matrix._c_object))

def sw_rowcol_diag_16(
        const_char * s1,
        const_char * s2,
        const int open, const int gap,
        Matrix matrix not None):
    cdef size_t t1 = len(s1)
    cdef size_t t2 = len(s2)
    cdef int l1 = <int>t1
    cdef int l2 = <int>t2
    assert l1 == t1
    assert l2 == t2
    return Result.create(parasail_sw_rowcol_diag_16(s1,l1,s2,l2,open,gap,matrix._c_object))

def sw_rowcol_diag_8(
        const_char * s1,
        const_char * s2,
        const int open, const int gap,
        Matrix matrix not None):
    cdef size_t t1 = len(s1)
    cdef size_t t2 = len(s2)
    cdef int l1 = <int>t1
    cdef int l2 = <int>t2
    assert l1 == t1
    assert l2 == t2
    return Result.create(parasail_sw_rowcol_diag_8(s1,l1,s2,l2,open,gap,matrix._c_object))

def sw_stats_scan_64(
        const_char * s1,
        const_char * s2,
        const int open, const int gap,
        Matrix matrix not None):
    cdef size_t t1 = len(s1)
    cdef size_t t2 = len(s2)
    cdef int l1 = <int>t1
    cdef int l2 = <int>t2
    assert l1 == t1
    assert l2 == t2
    return Result.create(parasail_sw_stats_scan_64(s1,l1,s2,l2,open,gap,matrix._c_object))

def sw_stats_scan_32(
        const_char * s1,
        const_char * s2,
        const int open, const int gap,
        Matrix matrix not None):
    cdef size_t t1 = len(s1)
    cdef size_t t2 = len(s2)
    cdef int l1 = <int>t1
    cdef int l2 = <int>t2
    assert l1 == t1
    assert l2 == t2
    return Result.create(parasail_sw_stats_scan_32(s1,l1,s2,l2,open,gap,matrix._c_object))

def sw_stats_scan_16(
        const_char * s1,
        const_char * s2,
        const int open, const int gap,
        Matrix matrix not None):
    cdef size_t t1 = len(s1)
    cdef size_t t2 = len(s2)
    cdef int l1 = <int>t1
    cdef int l2 = <int>t2
    assert l1 == t1
    assert l2 == t2
    return Result.create(parasail_sw_stats_scan_16(s1,l1,s2,l2,open,gap,matrix._c_object))

def sw_stats_scan_8(
        const_char * s1,
        const_char * s2,
        const int open, const int gap,
        Matrix matrix not None):
    cdef size_t t1 = len(s1)
    cdef size_t t2 = len(s2)
    cdef int l1 = <int>t1
    cdef int l2 = <int>t2
    assert l1 == t1
    assert l2 == t2
    return Result.create(parasail_sw_stats_scan_8(s1,l1,s2,l2,open,gap,matrix._c_object))

def sw_stats_striped_64(
        const_char * s1,
        const_char * s2,
        const int open, const int gap,
        Matrix matrix not None):
    cdef size_t t1 = len(s1)
    cdef size_t t2 = len(s2)
    cdef int l1 = <int>t1
    cdef int l2 = <int>t2
    assert l1 == t1
    assert l2 == t2
    return Result.create(parasail_sw_stats_striped_64(s1,l1,s2,l2,open,gap,matrix._c_object))

def sw_stats_striped_32(
        const_char * s1,
        const_char * s2,
        const int open, const int gap,
        Matrix matrix not None):
    cdef size_t t1 = len(s1)
    cdef size_t t2 = len(s2)
    cdef int l1 = <int>t1
    cdef int l2 = <int>t2
    assert l1 == t1
    assert l2 == t2
    return Result.create(parasail_sw_stats_striped_32(s1,l1,s2,l2,open,gap,matrix._c_object))

def sw_stats_striped_16(
        const_char * s1,
        const_char * s2,
        const int open, const int gap,
        Matrix matrix not None):
    cdef size_t t1 = len(s1)
    cdef size_t t2 = len(s2)
    cdef int l1 = <int>t1
    cdef int l2 = <int>t2
    assert l1 == t1
    assert l2 == t2
    return Result.create(parasail_sw_stats_striped_16(s1,l1,s2,l2,open,gap,matrix._c_object))

def sw_stats_striped_8(
        const_char * s1,
        const_char * s2,
        const int open, const int gap,
        Matrix matrix not None):
    cdef size_t t1 = len(s1)
    cdef size_t t2 = len(s2)
    cdef int l1 = <int>t1
    cdef int l2 = <int>t2
    assert l1 == t1
    assert l2 == t2
    return Result.create(parasail_sw_stats_striped_8(s1,l1,s2,l2,open,gap,matrix._c_object))

def sw_stats_diag_64(
        const_char * s1,
        const_char * s2,
        const int open, const int gap,
        Matrix matrix not None):
    cdef size_t t1 = len(s1)
    cdef size_t t2 = len(s2)
    cdef int l1 = <int>t1
    cdef int l2 = <int>t2
    assert l1 == t1
    assert l2 == t2
    return Result.create(parasail_sw_stats_diag_64(s1,l1,s2,l2,open,gap,matrix._c_object))

def sw_stats_diag_32(
        const_char * s1,
        const_char * s2,
        const int open, const int gap,
        Matrix matrix not None):
    cdef size_t t1 = len(s1)
    cdef size_t t2 = len(s2)
    cdef int l1 = <int>t1
    cdef int l2 = <int>t2
    assert l1 == t1
    assert l2 == t2
    return Result.create(parasail_sw_stats_diag_32(s1,l1,s2,l2,open,gap,matrix._c_object))

def sw_stats_diag_16(
        const_char * s1,
        const_char * s2,
        const int open, const int gap,
        Matrix matrix not None):
    cdef size_t t1 = len(s1)
    cdef size_t t2 = len(s2)
    cdef int l1 = <int>t1
    cdef int l2 = <int>t2
    assert l1 == t1
    assert l2 == t2
    return Result.create(parasail_sw_stats_diag_16(s1,l1,s2,l2,open,gap,matrix._c_object))

def sw_stats_diag_8(
        const_char * s1,
        const_char * s2,
        const int open, const int gap,
        Matrix matrix not None):
    cdef size_t t1 = len(s1)
    cdef size_t t2 = len(s2)
    cdef int l1 = <int>t1
    cdef int l2 = <int>t2
    assert l1 == t1
    assert l2 == t2
    return Result.create(parasail_sw_stats_diag_8(s1,l1,s2,l2,open,gap,matrix._c_object))

def sw_stats_table_scan_64(
        const_char * s1,
        const_char * s2,
        const int open, const int gap,
        Matrix matrix not None):
    cdef size_t t1 = len(s1)
    cdef size_t t2 = len(s2)
    cdef int l1 = <int>t1
    cdef int l2 = <int>t2
    assert l1 == t1
    assert l2 == t2
    return Result.create(parasail_sw_stats_table_scan_64(s1,l1,s2,l2,open,gap,matrix._c_object))

def sw_stats_table_scan_32(
        const_char * s1,
        const_char * s2,
        const int open, const int gap,
        Matrix matrix not None):
    cdef size_t t1 = len(s1)
    cdef size_t t2 = len(s2)
    cdef int l1 = <int>t1
    cdef int l2 = <int>t2
    assert l1 == t1
    assert l2 == t2
    return Result.create(parasail_sw_stats_table_scan_32(s1,l1,s2,l2,open,gap,matrix._c_object))

def sw_stats_table_scan_16(
        const_char * s1,
        const_char * s2,
        const int open, const int gap,
        Matrix matrix not None):
    cdef size_t t1 = len(s1)
    cdef size_t t2 = len(s2)
    cdef int l1 = <int>t1
    cdef int l2 = <int>t2
    assert l1 == t1
    assert l2 == t2
    return Result.create(parasail_sw_stats_table_scan_16(s1,l1,s2,l2,open,gap,matrix._c_object))

def sw_stats_table_scan_8(
        const_char * s1,
        const_char * s2,
        const int open, const int gap,
        Matrix matrix not None):
    cdef size_t t1 = len(s1)
    cdef size_t t2 = len(s2)
    cdef int l1 = <int>t1
    cdef int l2 = <int>t2
    assert l1 == t1
    assert l2 == t2
    return Result.create(parasail_sw_stats_table_scan_8(s1,l1,s2,l2,open,gap,matrix._c_object))

def sw_stats_table_striped_64(
        const_char * s1,
        const_char * s2,
        const int open, const int gap,
        Matrix matrix not None):
    cdef size_t t1 = len(s1)
    cdef size_t t2 = len(s2)
    cdef int l1 = <int>t1
    cdef int l2 = <int>t2
    assert l1 == t1
    assert l2 == t2
    return Result.create(parasail_sw_stats_table_striped_64(s1,l1,s2,l2,open,gap,matrix._c_object))

def sw_stats_table_striped_32(
        const_char * s1,
        const_char * s2,
        const int open, const int gap,
        Matrix matrix not None):
    cdef size_t t1 = len(s1)
    cdef size_t t2 = len(s2)
    cdef int l1 = <int>t1
    cdef int l2 = <int>t2
    assert l1 == t1
    assert l2 == t2
    return Result.create(parasail_sw_stats_table_striped_32(s1,l1,s2,l2,open,gap,matrix._c_object))

def sw_stats_table_striped_16(
        const_char * s1,
        const_char * s2,
        const int open, const int gap,
        Matrix matrix not None):
    cdef size_t t1 = len(s1)
    cdef size_t t2 = len(s2)
    cdef int l1 = <int>t1
    cdef int l2 = <int>t2
    assert l1 == t1
    assert l2 == t2
    return Result.create(parasail_sw_stats_table_striped_16(s1,l1,s2,l2,open,gap,matrix._c_object))

def sw_stats_table_striped_8(
        const_char * s1,
        const_char * s2,
        const int open, const int gap,
        Matrix matrix not None):
    cdef size_t t1 = len(s1)
    cdef size_t t2 = len(s2)
    cdef int l1 = <int>t1
    cdef int l2 = <int>t2
    assert l1 == t1
    assert l2 == t2
    return Result.create(parasail_sw_stats_table_striped_8(s1,l1,s2,l2,open,gap,matrix._c_object))

def sw_stats_table_diag_64(
        const_char * s1,
        const_char * s2,
        const int open, const int gap,
        Matrix matrix not None):
    cdef size_t t1 = len(s1)
    cdef size_t t2 = len(s2)
    cdef int l1 = <int>t1
    cdef int l2 = <int>t2
    assert l1 == t1
    assert l2 == t2
    return Result.create(parasail_sw_stats_table_diag_64(s1,l1,s2,l2,open,gap,matrix._c_object))

def sw_stats_table_diag_32(
        const_char * s1,
        const_char * s2,
        const int open, const int gap,
        Matrix matrix not None):
    cdef size_t t1 = len(s1)
    cdef size_t t2 = len(s2)
    cdef int l1 = <int>t1
    cdef int l2 = <int>t2
    assert l1 == t1
    assert l2 == t2
    return Result.create(parasail_sw_stats_table_diag_32(s1,l1,s2,l2,open,gap,matrix._c_object))

def sw_stats_table_diag_16(
        const_char * s1,
        const_char * s2,
        const int open, const int gap,
        Matrix matrix not None):
    cdef size_t t1 = len(s1)
    cdef size_t t2 = len(s2)
    cdef int l1 = <int>t1
    cdef int l2 = <int>t2
    assert l1 == t1
    assert l2 == t2
    return Result.create(parasail_sw_stats_table_diag_16(s1,l1,s2,l2,open,gap,matrix._c_object))

def sw_stats_table_diag_8(
        const_char * s1,
        const_char * s2,
        const int open, const int gap,
        Matrix matrix not None):
    cdef size_t t1 = len(s1)
    cdef size_t t2 = len(s2)
    cdef int l1 = <int>t1
    cdef int l2 = <int>t2
    assert l1 == t1
    assert l2 == t2
    return Result.create(parasail_sw_stats_table_diag_8(s1,l1,s2,l2,open,gap,matrix._c_object))

def sw_stats_rowcol_scan_64(
        const_char * s1,
        const_char * s2,
        const int open, const int gap,
        Matrix matrix not None):
    cdef size_t t1 = len(s1)
    cdef size_t t2 = len(s2)
    cdef int l1 = <int>t1
    cdef int l2 = <int>t2
    assert l1 == t1
    assert l2 == t2
    return Result.create(parasail_sw_stats_rowcol_scan_64(s1,l1,s2,l2,open,gap,matrix._c_object))

def sw_stats_rowcol_scan_32(
        const_char * s1,
        const_char * s2,
        const int open, const int gap,
        Matrix matrix not None):
    cdef size_t t1 = len(s1)
    cdef size_t t2 = len(s2)
    cdef int l1 = <int>t1
    cdef int l2 = <int>t2
    assert l1 == t1
    assert l2 == t2
    return Result.create(parasail_sw_stats_rowcol_scan_32(s1,l1,s2,l2,open,gap,matrix._c_object))

def sw_stats_rowcol_scan_16(
        const_char * s1,
        const_char * s2,
        const int open, const int gap,
        Matrix matrix not None):
    cdef size_t t1 = len(s1)
    cdef size_t t2 = len(s2)
    cdef int l1 = <int>t1
    cdef int l2 = <int>t2
    assert l1 == t1
    assert l2 == t2
    return Result.create(parasail_sw_stats_rowcol_scan_16(s1,l1,s2,l2,open,gap,matrix._c_object))

def sw_stats_rowcol_scan_8(
        const_char * s1,
        const_char * s2,
        const int open, const int gap,
        Matrix matrix not None):
    cdef size_t t1 = len(s1)
    cdef size_t t2 = len(s2)
    cdef int l1 = <int>t1
    cdef int l2 = <int>t2
    assert l1 == t1
    assert l2 == t2
    return Result.create(parasail_sw_stats_rowcol_scan_8(s1,l1,s2,l2,open,gap,matrix._c_object))

def sw_stats_rowcol_striped_64(
        const_char * s1,
        const_char * s2,
        const int open, const int gap,
        Matrix matrix not None):
    cdef size_t t1 = len(s1)
    cdef size_t t2 = len(s2)
    cdef int l1 = <int>t1
    cdef int l2 = <int>t2
    assert l1 == t1
    assert l2 == t2
    return Result.create(parasail_sw_stats_rowcol_striped_64(s1,l1,s2,l2,open,gap,matrix._c_object))

def sw_stats_rowcol_striped_32(
        const_char * s1,
        const_char * s2,
        const int open, const int gap,
        Matrix matrix not None):
    cdef size_t t1 = len(s1)
    cdef size_t t2 = len(s2)
    cdef int l1 = <int>t1
    cdef int l2 = <int>t2
    assert l1 == t1
    assert l2 == t2
    return Result.create(parasail_sw_stats_rowcol_striped_32(s1,l1,s2,l2,open,gap,matrix._c_object))

def sw_stats_rowcol_striped_16(
        const_char * s1,
        const_char * s2,
        const int open, const int gap,
        Matrix matrix not None):
    cdef size_t t1 = len(s1)
    cdef size_t t2 = len(s2)
    cdef int l1 = <int>t1
    cdef int l2 = <int>t2
    assert l1 == t1
    assert l2 == t2
    return Result.create(parasail_sw_stats_rowcol_striped_16(s1,l1,s2,l2,open,gap,matrix._c_object))

def sw_stats_rowcol_striped_8(
        const_char * s1,
        const_char * s2,
        const int open, const int gap,
        Matrix matrix not None):
    cdef size_t t1 = len(s1)
    cdef size_t t2 = len(s2)
    cdef int l1 = <int>t1
    cdef int l2 = <int>t2
    assert l1 == t1
    assert l2 == t2
    return Result.create(parasail_sw_stats_rowcol_striped_8(s1,l1,s2,l2,open,gap,matrix._c_object))

def sw_stats_rowcol_diag_64(
        const_char * s1,
        const_char * s2,
        const int open, const int gap,
        Matrix matrix not None):
    cdef size_t t1 = len(s1)
    cdef size_t t2 = len(s2)
    cdef int l1 = <int>t1
    cdef int l2 = <int>t2
    assert l1 == t1
    assert l2 == t2
    return Result.create(parasail_sw_stats_rowcol_diag_64(s1,l1,s2,l2,open,gap,matrix._c_object))

def sw_stats_rowcol_diag_32(
        const_char * s1,
        const_char * s2,
        const int open, const int gap,
        Matrix matrix not None):
    cdef size_t t1 = len(s1)
    cdef size_t t2 = len(s2)
    cdef int l1 = <int>t1
    cdef int l2 = <int>t2
    assert l1 == t1
    assert l2 == t2
    return Result.create(parasail_sw_stats_rowcol_diag_32(s1,l1,s2,l2,open,gap,matrix._c_object))

def sw_stats_rowcol_diag_16(
        const_char * s1,
        const_char * s2,
        const int open, const int gap,
        Matrix matrix not None):
    cdef size_t t1 = len(s1)
    cdef size_t t2 = len(s2)
    cdef int l1 = <int>t1
    cdef int l2 = <int>t2
    assert l1 == t1
    assert l2 == t2
    return Result.create(parasail_sw_stats_rowcol_diag_16(s1,l1,s2,l2,open,gap,matrix._c_object))

def sw_stats_rowcol_diag_8(
        const_char * s1,
        const_char * s2,
        const int open, const int gap,
        Matrix matrix not None):
    cdef size_t t1 = len(s1)
    cdef size_t t2 = len(s2)
    cdef int l1 = <int>t1
    cdef int l2 = <int>t2
    assert l1 == t1
    assert l2 == t2
    return Result.create(parasail_sw_stats_rowcol_diag_8(s1,l1,s2,l2,open,gap,matrix._c_object))
