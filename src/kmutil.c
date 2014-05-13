/*
 * ============================================================================
 *
 *       Filename:  kmutil.c
 *
 *    Description:  Wrappers around std library functions
 *
 *        Version:  1.0
 *        Created:  04/05/14 14:10:50
 *       Revision:  none
 *        License:  GPLv3+
 *       Compiler:  gcc, clang
 *
 *         Author:  Kevin Murray, spam@kdmurray.id.au
 *
 * ============================================================================
 */

#include "kmutil.h"


/* kmroundupz:
 *   Round up a `size_t` to the next highest power of two.
 */
inline size_t
kmroundupz (size_t sz)
{
    /* Decrement v only if v is not already a power of 2 */
    if (km_unlikely((sz & (sz - 1)) != 0)) sz--;
    /* mask all bits below MSB to 1 */
    sz |= sz>>1;
    sz |= sz>>2;
    sz |= sz>>4;
    sz |= sz>>8;
    sz |= sz>>16;
#if UINTPTR_MAX == 0xffffffffffffffff /* 64-bit system */
    sz |= sz>>32;
#endif
    return sz + 1;
}


/* Valid non-function to pass to libkdm functions */
void
errnil (ERRFN_ARGS)
{
    (void) (msg);
    (void) (file);
    (void) (line);
}

/* Function to pass to libkdm functions which prints out errors to stderr */
void
errprint (ERRFN_ARGS)
{
    char msg_fmt[1<<8] = "";
    if (msg == NULL) {
        msg = "GENERIC ERROR WITH NO MESSAGE";
    }
    va_list args;
    va_start (args, line);
    vsnprintf(msg_fmt, 1<<8, msg, args);
    va_end (args);
    fprintf(stderr, "[%s: %d]: %s\n", file, line, msg_fmt);
}

/* Function to pass to libkdm functions which prints out errors to stderr and
   calls `exit(EXIT_FAILURE)` */
void
errprintexit (ERRFN_ARGS)
{
    char msg_fmt[1<<8] = "";
    if (msg == NULL) {
        msg = "GENERIC ERROR WITH NO MESSAGE";
    }
    va_list args;
    va_start (args, line);
    vsnprintf(msg_fmt, 1<<8, msg, args);
    va_end (args);
    fprintf(stderr, "[%s: %d]: %s\n", file, line, msg_fmt);
    KM_EXIT_FN(EXIT_FAILURE);
}

/*
 * Memory allocation/deallocation
 */
inline void *
km_calloc_ (size_t n, size_t size, errhandler_t onerr, char *file, int line)
{
    void *ret = calloc(n, size);
    if (ret == NULL) {
        (*onerr)("calloc returned NULL -- Out of memory", file, line);
        return NULL;
    } else {
        return ret;
    }
}

inline void *
km_malloc_ (size_t size, errhandler_t onerr, char *file, int line)
{
    void *ret = malloc(size);
    if (ret == NULL) {
        (*onerr)("calloc returned NULL -- Out of memory", file, line);
        return NULL;
    } else {
        return ret;
    }
}

inline void *
km_realloc_ (void *data, size_t size, errhandler_t onerr, char *file, int line)
{
    void *ret = realloc(data, size);
    if (ret == NULL) {
        (*onerr)("calloc returned NULL -- Out of memory", file, line);
        return NULL;
    } else {
        return ret;
    }
}
