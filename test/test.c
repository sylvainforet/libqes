/*
 * ============================================================================
 *
 *       Filename:  test.c
 *
 *    Description:  Tests for libqes
 *
 *        Version:  1.0
 *        Created:  11/08/13 21:34:38
 *       Revision:  none
 *        License:  GPLv3+
 *       Compiler:  gcc, clang
 *
 *         Author:  Kevin Murray, spam@kdmurray.id.au
 *
 * ============================================================================
 */

#include "tests.h"


struct testgroup_t libqes_tests[] = {
    {"qes/util/", qes_util_tests},
    {"qes/match/", qes_match_tests},
    {"qes/file/", qes_file_tests},
    {"qes/seqfile/", qes_seqfile_tests},
    {"qes/seq/", qes_seq_tests},
    {"qes/sequtil/", qes_sequtil_tests},
    {"testdata/", data_tests},
    END_OF_GROUPS
};


/*
 * ===  FUNCTION  =============================================================
 *         Name:  main
 *  Description:  Run all tests
 * ============================================================================
 */

int
main (int argc, const char *argv[])
{
    int res;
    int our_argc = argc;
    const char **our_argv = argv;

    data_prefix = NULL;
    if (argc>1) {
       data_prefix = strdup(argv[1]);
       our_argc -= 1;
       our_argv += 1;
    }
    if (data_prefix == NULL) {
        data_prefix = strdup(".");
    }
    if (access(data_prefix, W_OK | X_OK | R_OK) != 0) {
        fprintf(stderr, "Could not access data prefix dir '%s'\n", data_prefix);
        fprintf(stderr, "Please set the LIBQES_TEST_DATA_DIR environmental variable appropriately\n");
        free(data_prefix);
        exit(EXIT_FAILURE);
    }
    res = tinytest_main(argc-1, argv+1, libqes_tests);
    free(data_prefix);
    return res;
}
