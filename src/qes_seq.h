/*
 * ============================================================================
 *
 *       Filename:  qes_seq.h
 *
 *    Description:  Sequence structures
 *
 *        Version:  1.0
 *        Created:  31/07/13 12:51:02
 *       Revision:  none
 *        License:  GPLv3+
 *       Compiler:  gcc, clang
 *
 *         Author:  Kevin Murray, spam@kdmurray.id.au
 *
 * ============================================================================
 */

#ifndef QES_SEQ_H
#define QES_SEQ_H

#include <qes_util.h>
#include <qes_str.h>

/*---------------------------------------------------------------------------
  | qes_seq module -- data structures to hold NGS sequences                 |
  ---------------------------------------------------------------------------*/

/* TYPE DEFINITIONS */
struct qes_seq {
    struct qes_str name;
    struct qes_str comment;
    struct qes_str seq;
    struct qes_str qual;
};

/* PROTOTYPES */

/*===  FUNCTION  ============================================================*
Name:           qes_seq_create
Paramters:      void
Description:    Create a ``struct qes_seq`` object on the heap, creating and
                initialising all members. qes_seq_create_no_X functions do not
                create or initialise the members in their names.
Returns:        struct qes_seq *: A non-null memory address on success, otherwise NULL.
 *===========================================================================*/
struct qes_seq *qes_seq_create (void);
struct qes_seq *qes_seq_create_no_qual (void);
struct qes_seq *qes_seq_create_no_qual_or_comment (void);


/*===  FUNCTION  ============================================================*
Name:           qes_seq_ok
Paramters:      struct qes_seq *: seq to check
Description:    Check if ``seq`` is a usable struct qes_seq struct. qes_seq_ok_no-X
                functions permit the member(s) in their names to be unusable.
Returns:        1 if usable, 0 otherwise.
 *===========================================================================*/
static inline int
qes_seq_ok (const struct qes_seq *seq)
{
    return \
       seq != NULL && \
       qes_str_ok(&seq->name) && \
       qes_str_ok(&seq->comment) && \
       qes_str_ok(&seq->seq) && \
       qes_str_ok(&seq->qual);
}

static inline int
qes_seq_ok_no_comment (const struct qes_seq *seq)
{
    return \
       seq != NULL && \
       qes_str_ok(&seq->name) && \
       qes_str_ok(&seq->seq) && \
       qes_str_ok(&seq->qual);
}

static inline int
qes_seq_ok_no_qual (const struct qes_seq *seq)
{
    return \
        seq != NULL && \
        qes_str_ok(&seq->name) && \
        qes_str_ok(&seq->comment) && \
        qes_str_ok(&seq->seq);
}

static inline int
qes_seq_ok_no_comment_or_qual (const struct qes_seq *seq)
{
    return \
        seq != NULL && \
        qes_str_ok(&seq->name) && \
        qes_str_ok(&seq->seq);
}

static inline int
qes_seq_has_comment (const struct qes_seq *seq)
{
    return qes_seq_ok(seq) && seq->comment.len > 0;
}

static inline int
qes_seq_has_qual (const struct qes_seq *seq)
{
    return qes_seq_ok(seq) && seq->qual.len > 0;
}

static inline int
qes_seq_n_bytes (const struct qes_seq *seq)
{
    if (!qes_seq_ok(seq)) {
        return -1;
    }
    /* Arragned per line in a fastq */
    return 1 + seq->name.len + \
           (qes_seq_has_comment(seq) ? 1 + seq->comment.len : 0) + 1 + \
           seq->seq.len + 1 +\
           qes_seq_has_qual(seq) ? 2 + seq->qual.len + 1 : 0;
}

/*===  FUNCTION  ============================================================*
Name:           qes_seq_fill_header
Paramters:      struct qes_seq *seqobj: Seq object that will receive the header.
Description:    Fills the name and comment members of a ``struct qes_seq`` from the
                header line of a fasta/fastq file.
Returns:        int: 1 on success, otherwise 0 for failure.
 *===========================================================================*/
extern int qes_seq_fill_header(struct qes_seq *seqobj, char *header, size_t len);


/*===  FUNCTION  ============================================================*
Name:           qes_seq_fill_X
Paramters:      These functions take a ``struct qes_seq``, a char array and
                the length of the char array as a size_t.
Description:    Fill a struct qes_seq's name, comment, seq or qual member from
                a char array. If a non-zero value is given to ``len``, it is
                assumed to be the length of the string, otherwise the length of
                the string is calculated using strlen.
Returns:        int: 1 on success, 0 on failure.
 *===========================================================================*/
extern int qes_seq_fill_name(struct qes_seq *seqobj, const char *name,
                             size_t len);
extern int qes_seq_fill_comment(struct qes_seq *seqobj, const char *comment,
                                size_t len);
extern int qes_seq_fill_seq(struct qes_seq *seqobj, const char *seq,
                            size_t len);
extern int qes_seq_fill_qual(struct qes_seq *seqobj, const char *qual,
                             size_t len);
extern int qes_seq_fill(struct qes_seq *seqobj, const char *name,
                        const char *comment, const char *seq, const char *qual);

#if 0
/*===  FUNCTION  ============================================================*
Name:           qes_seq_print
Paramters:      const struct qes_seq *: seq to print
                qes_seqfile_format_t: file format to print in.
                FILE *: open file stream to print to.
Description:    Print ``seq`` in formatted per ``format`` to ``stream``.
Returns:        int: 1 on success, 0 on failure.
 *===========================================================================*/
extern int qes_seq_print (const struct qes_seq *seq,
                          enum qes_seqfile_format fmt, FILE *stream);
#endif

/*===  FUNCTION  ============================================================*
Name:           qes_seq_destroy
Paramters:      struct qes_seq *: seq to destroy.
Description:    Deallocate and set to NULL a struct qes_seq on the heap.
Returns:        void.
 *===========================================================================*/
void qes_seq_destroy_(struct qes_seq *seq);
#define qes_seq_destroy(seq) do {       \
            qes_seq_destroy_(seq);      \
            seq = NULL;             \
        } while(0)

static inline int
qes_seq_copy(struct qes_seq *dest, const struct qes_seq *src)
{
    if (dest == src || !qes_seq_ok(dest) || !qes_seq_ok(src)) return 1;

    if (qes_str_copy(&dest->name, &src->name) != 0) return 1;
    if (qes_str_copy(&dest->comment, &src->comment) != 0) return 1;
    if (qes_str_copy(&dest->seq, &src->seq) != 0) return 1;
    if (qes_str_copy(&dest->qual, &src->qual) != 0) return 1;
    return 0;
}

#endif /* QES_SEQ_H */
