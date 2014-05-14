/*
 * ============================================================================
 *
 *       Filename:  kmzfile.c
 *
 *    Description:  Compressed file IO
 *
 *        Version:  1.0
 *        Created:  09/05/14 12:20:57
 *       Revision:  none
 *        License:  GPLv3+
 *       Compiler:  gcc, clang
 *
 *         Author:  Kevin Murray, spam@kdmurray.id.au
 *
 * ============================================================================
 */

#include "kmzfile.h"

zfile_t *
zfopen_ (const char *path, const char *mode, errhandler_t onerr, char *file,
        int line)
{
    /* create file struct */
    zfile_t *zf = km_calloc(1, sizeof(*zf), onerr);
    /* Open file, handling any errors */
    zf->fp = KM_ZOPEN(path, mode);
    if (zf->fp == NULL) {
        (*onerr)("Opening file %s failed:\n%s\n", file, line,
                path, strerror(errno));
        km_free(zf);
        return(NULL);
    }
    /* Use a larger than default IO buffer, speeds things up */
    KM_ZBUFFER(zf->fp, KM_FILEBUFFER_LEN);
    /* init struct fields */
    zf->eof = 0;
    zf->filepos = 0;
    zf->path = strndup(path, KM_MAX_FN_LEN);
    return(zf);
}

void
zfclose (zfile_t *file)
{
    if (file != NULL) {
        if (file->fp != NULL) {
            KM_ZCLOSE(file->fp);
        }
        km_free(file->path);
    }
    km_free(file);
}

inline ssize_t
zfreadline_realloc_ (zfile_t *file, char **buf, size_t *size,
              errhandler_t onerr, const char *src, const int line)
{
    size_t len = 0;
    if (buf == NULL ||  file == NULL || size == NULL) {
        return -2; /* EOF is normally == -1, so use -2 to differentiate them */
    }
    if (*buf == NULL) {
        buf = km_calloc_(__INIT_LINE_LEN, sizeof(*buf), onerr, src, line);
        *size = __INIT_LINE_LEN;
    }
    while(((*buf)[len] = KM_ZFGETC(file->fp)) != EOF && (*buf)[len++] != '\n') {
        while (len + 1 >= (*size) - 1) {
            *size = kmroundupz((*size) + 1);
            char *newbuf = km_realloc_(*buf, sizeof(**buf) * (*size), onerr,
                    src, line);
            if (newbuf == NULL) { /* In case we only print the error */
                return -2;
            } else {
                (*buf) = newbuf;
            }
        }
    }
    (*buf)[len] = '\0';
    file->filepos += len;
    if (KM_ZEOF(file->fp)) {
        file->eof = 1;
        return EOF;
    } else {
        return len;
    }
}

inline ssize_t zfreadline (zfile_t *file, char **dest, size_t maxlen)
{
    size_t len = 0;
    if (dest == NULL || *dest == NULL || file == NULL) {
        /* EOF is normally == -1, so use -2 to differentiate them */
        return -2;
    }
    while ( ((*dest)[len] = KM_ZFGETC(file->fp)) != EOF && \
            (*dest)[len] != '\n' && \
            len < maxlen) {
        len++;
    }
    (*dest)[len] = '\0';
    file->filepos += len;
    if (KM_ZEOF(file->fp)) {
        file->eof = 1;
        return EOF;
    } else {
        return len;
    }
}
