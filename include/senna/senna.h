/* Copyright(C) 2004 Brazil

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/
#ifndef SENNA_H
#define SENNA_H

#ifdef  __cplusplus
extern "C" {
#endif

/* available bit for sen_index_create flag */
#define SEN_INDEX_NORMALIZE                     0x0001
#define SEN_INDEX_SPLIT_ALPHA                   0x0002
#define SEN_INDEX_SPLIT_DIGIT                   0x0004
#define SEN_INDEX_SPLIT_SYMBOL                  0x0008
#define SEN_INDEX_MORPH_ANALYSE                 0x0000
#define SEN_INDEX_NGRAM                         0x0010
#define SEN_INDEX_DELIMITED                     0x0020
#define SEN_INDEX_ENABLE_SUFFIX_SEARCH          0x0100
#define SEN_INDEX_DISABLE_SUFFIX_SEARCH         0x0200
#define SEN_INDEX_WITH_VGRAM                    0x1000
#define SEN_INDEX_SHARED_LEXICON                0x2000
#define SEN_INDEX_WITH_VACUUM                   0x8000

/* 16 tokenizers can be registered */
#define SEN_INDEX_TOKENIZER_MASK                0x00f0

#define SEN_SYM_MAX_KEY_SIZE                    8192

#define SEN_SYM_WITH_SIS                        0x80000000

#define SEN_SNIP_NORMALIZE                      0x0001
#define SEN_SNIP_COPY_TAG                       0x0002
#define SEN_SNIP_SKIP_LEADING_SPACES            0x0004
#define SEN_QUERY_SCAN_NORMALIZE                SEN_SNIP_NORMALIZE

#define SEN_LEX_NGRAM_UNIT_SIZE                 2

#ifndef SEN_STACK_SIZE
#define SEN_STACK_SIZE                          0x10000000
#endif /* SEN_STACK_SIZE */

#ifndef SEN_QUERY_AND
#define SEN_QUERY_AND '+'
#endif /* SEN_QUERY_AND */
#ifndef SEN_QUERY_BUT
#define SEN_QUERY_BUT '-'
#endif /* SEN_QUERY_BUT */
#ifndef SEN_QUERY_ADJ_INC
#define SEN_QUERY_ADJ_INC '>'
#endif /* SEN_QUERY_ADJ_POS2 */
#ifndef SEN_QUERY_ADJ_DEC
#define SEN_QUERY_ADJ_DEC '<'
#endif /* SEN_QUERY_ADJ_POS1 */
#ifndef SEN_QUERY_ADJ_NEG
#define SEN_QUERY_ADJ_NEG '~'
#endif /* SEN_QUERY_ADJ_NEG */
#ifndef SEN_QUERY_PREFIX
#define SEN_QUERY_PREFIX '*'
#endif /* SEN_QUERY_PREFIX */
#ifndef SEN_QUERY_PARENL
#define SEN_QUERY_PARENL '('
#endif /* SEN_QUERY_PARENL */
#ifndef SEN_QUERY_PARENR
#define SEN_QUERY_PARENR ')'
#endif /* SEN_QUERY_PARENR */
#ifndef SEN_QUERY_QUOTEL
#define SEN_QUERY_QUOTEL '"'
#endif /* SEN_QUERY_QUOTEL */
#ifndef SEN_QUERY_QUOTER
#define SEN_QUERY_QUOTER '"'
#endif /* SEN_QUERY_QUOTER */
#ifndef SEN_QUERY_ESCAPE
#define SEN_QUERY_ESCAPE '\\'
#endif /* SEN_QUERY_ESCAPE */

#define SEN_SYM_NIL 0

typedef enum {
  sen_success = 0,
  sen_memory_exhausted,
  sen_invalid_format,
  sen_file_operation_error,
  sen_invalid_argument,
  sen_other_error,
  sen_external_error,
  sen_internal_error,
  sen_abnormal_error,
  sen_end_of_data
} sen_rc;

typedef enum {
  sen_enc_default = 0,
  sen_enc_none,
  sen_enc_euc_jp,
  sen_enc_utf8,
  sen_enc_sjis,
  sen_enc_latin1,
  sen_enc_koi8r
} sen_encoding;

typedef enum {
  sen_rec_document = 0,
  sen_rec_section,
  sen_rec_position,
  sen_rec_userdef,
  sen_rec_none
} sen_rec_unit;

typedef enum {
  sen_sel_or = 0,
  sen_sel_and,
  sen_sel_but,
  sen_sel_adjust
} sen_sel_operator;

typedef enum {
  sen_sel_exact = 0,
  sen_sel_partial,
  sen_sel_unsplit,
  sen_sel_near,
  sen_sel_near2,
  sen_sel_similar,
  sen_sel_term_extract,
  sen_sel_prefix,
  sen_sel_suffix
} sen_sel_mode;

typedef enum {
  sen_sort_descending = 0,
  sen_sort_ascending = 1
} sen_sort_mode;

typedef enum {
  sen_log_none = 0,
  sen_log_emerg,
  sen_log_alert,
  sen_log_crit,
  sen_log_error,
  sen_log_warning,
  sen_log_notice,
  sen_log_info,
  sen_log_debug,
  sen_log_dump
} sen_log_level;

typedef struct _sen_db sen_db;
typedef struct _sen_ctx sen_ctx;
typedef struct _sen_ctx_info sen_ctx_info;
typedef struct _sen_set sen_set;
typedef struct _sen_sym sen_sym;
typedef struct _sen_inv sen_inv;
typedef struct _sen_index sen_index;
typedef struct _sen_set_cursor sen_set_cursor;
typedef struct _sen_set_element *sen_set_eh;
typedef struct _sen_value sen_value;
typedef struct _sen_values sen_values;
typedef struct _sen_select_optarg sen_select_optarg;
typedef struct _sen_group_optarg sen_group_optarg;
typedef struct _sen_sort_optarg sen_sort_optarg;
typedef struct _sen_snip sen_snip;
typedef struct _sen_query sen_query;
typedef struct _sen_logger_info sen_logger_info;
typedef struct _sen_snip_mapping sen_snip_mapping;
typedef struct _sen_records_heap sen_records_heap;
typedef struct _sen_vgram sen_vgram;
typedef struct _sen_vgram_buf sen_vgram_buf;
typedef struct _sen_sym_scan_hit sen_sym_scan_hit;
typedef struct _sen_sym_cursor sen_sym_cursor;

typedef unsigned sen_id;

#define SEN_ID_MAX 0x3fffffff

typedef sen_set_eh sen_recordh;
typedef sen_set sen_records;
typedef sen_sort_optarg sen_set_sort_optarg;

struct _sen_ctx_info {
  int fd;
  unsigned int com_status;
  unsigned int com_info;
  struct _sen_rbuf *outbuf;
  unsigned char stat;
};

struct _sen_index {
  int foreign_flags;
  sen_sym *keys;
  sen_sym *lexicon;
  sen_inv *inv;
  sen_vgram *vgram;
};

struct _sen_value {
  const char *str;
  unsigned int str_len;
  unsigned int weight;
};

struct _sen_values {
  int n_values;
  sen_value *values;
};

struct _sen_select_optarg {
  sen_sel_mode mode;
  int similarity_threshold;
  int max_interval;
  int *weight_vector;
  int vector_size;
  int (*func)(sen_records *, const void *, int, void *);
  void *func_arg;
  int max_size;
};

struct _sen_group_optarg {
  sen_sort_mode mode;
  int (*func)(sen_records *, const sen_recordh *, void *, void *);
  void *func_arg;
  int key_size;
};

struct _sen_sort_optarg {
  sen_sort_mode mode;
  int (*compar)(sen_records *, sen_recordh *, sen_records *, sen_recordh *, void *);
  void *compar_arg;
};

struct _sen_snip_mapping {
  void *dummy;
};

#define SEN_LOG_TIME      1
#define SEN_LOG_TITLE     2
#define SEN_LOG_MESSAGE   4
#define SEN_LOG_LOCATION  8

struct _sen_logger_info {
  sen_log_level max_level;
  int flags;
  void (*func)(int, const char *, const char *, const char *, const char *, void *);
  void *func_arg;
};

struct _sen_sym_scan_hit {
  sen_id id;
  unsigned int offset;
  unsigned int length;
};

typedef int (*query_term_callback)(const char *, unsigned int, void *);


/******** query language API ********/

sen_db *sen_db_create(const char *path, int flags, sen_encoding encoding);
sen_db *sen_db_open(const char *path);
sen_rc sen_db_close(sen_db *s);

#define SEN_CTX_MORE  0x01
#define SEN_CTX_TAIL  0x02
#define SEN_CTX_HEAD  0x04
#define SEN_CTX_QUIET 0x08
#define SEN_CTX_QUIT  0x10

#define SEN_CTX_USEQL 1
#define SEN_CTX_BATCHMODE 2

sen_ctx *sen_ctx_open(sen_db *db, int flags);
sen_ctx *sen_ctx_connect(const char *host, int port, int flags);
sen_rc sen_ctx_load(sen_ctx *c, const char *path);
sen_rc sen_ctx_send(sen_ctx *c, char *str, unsigned int str_len, int flags);
sen_rc sen_ctx_recv(sen_ctx *c, char **str, unsigned int *str_len, int *flags);
sen_rc sen_ctx_close(sen_ctx *c);
sen_rc sen_ctx_info_get(sen_ctx *c, sen_ctx_info *info);

/******** basic API ********/

sen_rc sen_init(void);
sen_rc sen_fin(void);
sen_rc sen_info(char **version,
                char **configure_options,
                char **config_path,
                sen_encoding *default_encoding,
                unsigned int *initial_n_segments,
                unsigned int *partial_match_threshold);

sen_index *sen_index_create(const char *path, int key_size, int flags,
                            int initial_n_segments, sen_encoding encoding);
sen_index *sen_index_open(const char *path);
sen_rc sen_index_close(sen_index *i);
sen_rc sen_index_remove(const char *path);
sen_rc sen_index_rename(const char *old_name, const char *new_name);
sen_rc sen_index_upd(sen_index *i, const void *key,
                     const char *oldvalue, unsigned int oldvalue_len,
                     const char *newvalue, unsigned int newvalue_len);
sen_records *sen_index_sel(sen_index *i,
                           const char *string, unsigned int string_len);
#ifdef USE_QUERY_ABORT
void sen_index_set_abort_callback(sen_index *i, int (*cb)(void*), void *arg);
#endif /* USE_QUERY_ABORT */
int sen_records_next(sen_records *r, void *keybuf, int buf_size, int *score);
sen_rc sen_records_rewind(sen_records *r);
int sen_records_curr_score(sen_records *r);
int sen_records_curr_key(sen_records *r, void *keybuf, int buf_size);
int sen_records_nhits(sen_records *r);
int sen_records_find(sen_records *r, const void *key);
sen_rc sen_records_close(sen_records *r);

/******** advanced API ********/

sen_values *sen_values_open(void);
sen_rc sen_values_close(sen_values *v);
sen_rc sen_values_add(sen_values *v,
                      const char *str, unsigned int str_len,
                      unsigned int weight);

sen_records *sen_records_open(sen_rec_unit record_unit,
                              sen_rec_unit subrec_unit,
                              unsigned int max_n_subrecs);
sen_records *sen_records_union(sen_records *a, sen_records *b);
sen_records *sen_records_subtract(sen_records *a, sen_records *b);
sen_records *sen_records_intersect(sen_records *a, sen_records *b);
int sen_records_difference(sen_records *a, sen_records *b);
sen_rc sen_records_sort(sen_records *r, int limit, sen_sort_optarg *optarg);
sen_rc sen_records_group(sen_records *r, int limit, sen_group_optarg *optarg);
const sen_recordh *sen_records_curr_rec(sen_records *r);
const sen_recordh *sen_records_at(sen_records *r, const void *key,
                                   unsigned section, unsigned pos,
                                   int *score, int *n_subrecs);
sen_rc sen_record_info(sen_records *r, const sen_recordh *rh,
                       void *keybuf, int buf_size, int *key_size,
                       int *section, int *pos, int *score, int *n_subrecs);
sen_rc sen_record_subrec_info(sen_records *r, const sen_recordh *rh,
                              int index, void *keybuf, int buf_size,
                              int *key_size, int *section, int *pos, int *score);
sen_index *sen_index_create_with_keys(const char *path, sen_sym *keys, int flags,
                                      int initial_n_segments, sen_encoding encoding);
sen_index *sen_index_open_with_keys(const char *path, sen_sym *keys);
sen_index *sen_index_create_with_keys_lexicon(const char *path, sen_sym *keys,
                                              sen_sym *lexicon, int initial_n_segments);
sen_index *sen_index_open_with_keys_lexicon(const char *path, sen_sym *keys,
                                            sen_sym *lexicon);
sen_rc sen_index_update(sen_index *i, const void *key, unsigned int section,
                        sen_values *oldvalues, sen_values *newvalues);
sen_rc sen_index_select(sen_index *i,
                        const char *string, unsigned int string_len,
                        sen_records *r,
                        sen_sel_operator op, sen_select_optarg *optarg);
sen_rc sen_index_info(sen_index *i, int *key_size, int *flags,
                      int *initial_n_segments, sen_encoding *encoding,
                      unsigned *nrecords_keys, unsigned *file_size_keys,
                      unsigned *nrecords_lexicon, unsigned *file_size_lexicon,
                      unsigned long long *inv_seg_size,
                      unsigned long long *inv_chunk_size);
int sen_index_path(sen_index *i, char *pathbuf, int buf_size);

sen_query *sen_query_open(const char *str, unsigned int str_len,
                          sen_sel_operator default_op,
                          int max_exprs, sen_encoding encoding);
unsigned int sen_query_rest(sen_query *q, const char ** const rest);
sen_rc sen_query_close(sen_query *q);
sen_rc sen_query_exec(sen_index *i, sen_query *q, sen_records *r, sen_sel_operator op);
void sen_query_term(sen_query *q, query_term_callback func, void *func_arg);
sen_rc sen_query_scan(sen_query *q, const char **strs, unsigned int *str_lens,
                      unsigned int nstrs, int flags, int *found, int *score);
sen_snip *sen_query_snip(sen_query *query, int flags,
                         unsigned int width, unsigned int max_results,
                         unsigned int n_tags,
                         const char **opentags, unsigned int *opentag_lens,
                         const char **closetags, unsigned int *closetag_lens,
                         sen_snip_mapping *mapping);

sen_rc sen_index_del(sen_index *i, const void *key);

/******** low level API ********/

sen_set *sen_set_open(unsigned key_size, unsigned value_size, unsigned init_size);
sen_rc sen_set_close(sen_set *set);
sen_rc sen_set_info(sen_set *set, unsigned *key_size,
                    unsigned *value_size, unsigned *n_entries);
sen_set_eh *sen_set_get(sen_set *set, const void *key, void **value);
sen_set_eh *sen_set_at(sen_set *set, const void *key, void **value);
sen_rc sen_set_del(sen_set *set, sen_set_eh *e);
sen_set_cursor *sen_set_cursor_open(sen_set *set);
sen_set_eh *sen_set_cursor_next(sen_set_cursor *cursor, void **key, void **value);
sen_rc sen_set_cursor_close(sen_set_cursor *cursor);
sen_rc sen_set_element_info(sen_set *set, const sen_set_eh *e,
                            void **key, void **value);
sen_set *sen_set_union(sen_set *a, sen_set *b);
sen_set *sen_set_subtract(sen_set *a, sen_set *b);
sen_set *sen_set_intersect(sen_set *a, sen_set *b);
int sen_set_difference(sen_set *a, sen_set *b);
sen_set_eh *sen_set_sort(sen_set *set, int limit, sen_set_sort_optarg *optarg);

sen_sym *sen_sym_create(const char *path, unsigned key_size,
                         unsigned flags, sen_encoding encoding);
sen_sym *sen_sym_open(const char *path);
sen_rc sen_sym_info(sen_sym *sym, int *key_size, unsigned *flags,
                    sen_encoding *encoding, unsigned *nrecords, unsigned *file_size);
sen_rc sen_sym_close(sen_sym *sym);
sen_rc sen_sym_remove(const char *path);

/* Lookup the sym table and find the id of the corresponding entry.
 * If no matches are found, create a new entry, and return that ID
 */
sen_id sen_sym_get(sen_sym *sym, const void *key);

/* Lookup the sym table and find the id of the corresponding entry.
 * If no matches are found return SEN_SYM_NIL
 */
sen_id sen_sym_at(sen_sym *sym, const void *key);
sen_rc sen_sym_del(sen_sym *sym, const void *key);
unsigned int sen_sym_size(sen_sym *sym);
int sen_sym_key(sen_sym *sym, sen_id id, void *keybuf, int buf_size);
sen_set *sen_sym_prefix_search(sen_sym *sym, const void *key);
sen_set *sen_sym_suffix_search(sen_sym *sym, const void *key);
sen_id sen_sym_common_prefix_search(sen_sym *sym, const void *key);
int sen_sym_pocket_get(sen_sym *sym, sen_id id);
sen_rc sen_sym_pocket_set(sen_sym *sym, sen_id id, unsigned int value);
sen_id sen_sym_next(sen_sym *sym, sen_id id);
int sen_sym_scan(sen_sym *sym, const char *str, unsigned int str_len,
                 sen_sym_scan_hit *sh, unsigned int sh_size, const char **rest);

#define SEN_SYM_DESCENDING 0
#define SEN_SYM_ASCENDING  1
#define SEN_SYM_GE 0
#define SEN_SYM_GT 2
#define SEN_SYM_LE 0
#define SEN_SYM_LT 4

sen_sym_cursor *sen_sym_cursor_open(sen_sym *sym, sen_ctx *ctx,
                                    const void *min, const void *max, int flags);
sen_id sen_sym_cursor_next(sen_sym_cursor *c);
void sen_sym_cursor_close(sen_sym_cursor *c);

/******** utility API ********/
sen_snip *sen_snip_open(sen_encoding encoding, int flags, unsigned int width,
                        unsigned int max_results,
                        const char *defaultopentag, unsigned int defaultopentag_len,
                        const char *defaultclosetag, unsigned int defaultclosetag_len,
                        sen_snip_mapping *mapping);
sen_rc sen_snip_close(sen_snip *snip);
sen_rc sen_snip_add_cond(sen_snip *snip,
                         const char *keyword, unsigned int keyword_len,
                         const char *opentag, unsigned int opentag_len,
                         const char *closetag, unsigned int closetag_len);
sen_rc sen_snip_exec(sen_snip *snip,
                     const char *string, unsigned int string_len,
                     unsigned int *nresults, unsigned int *max_tagged_len);
sen_rc sen_snip_get_result(sen_snip *snip, const unsigned int index,
                           char *result, unsigned int *result_len);

sen_records_heap *sen_records_heap_open(int size, int limit, sen_sort_optarg *optarg);
sen_rc sen_records_heap_add(sen_records_heap *h, sen_records *r);
int sen_records_heap_next(sen_records_heap *h);
sen_records *sen_records_heap_head(sen_records_heap *h);
sen_rc sen_records_heap_close(sen_records_heap *h);

int sen_inv_entry_info(sen_inv *inv, sen_id key, unsigned *a, unsigned *pocket,
                       unsigned *chunk, unsigned *chunk_size, unsigned *buffer_free,
                       unsigned *nterms, unsigned *nterms_void, unsigned *tid,
                       unsigned *size_in_chunk, unsigned *pos_in_chunk,
                       unsigned *size_in_buffer, unsigned *pos_in_buffer);

/* flags for sen_str_normalize */
#define SEN_STR_REMOVEBLANK 1
#define SEN_STR_WITH_CTYPES 2
#define SEN_STR_WITH_CHECKS 4
int sen_str_normalize(const char *str, unsigned int str_len,
                      sen_encoding encoding, int flags,
                      char *nstrbuf, int buf_size);
unsigned int sen_str_charlen(const char *str, sen_encoding encoding);

/* misc */

sen_rc sen_logger_info_set(const sen_logger_info *info);

void sen_logger_put(sen_log_level level,
                    const char *file, int line, const char *func, char *fmt, ...);

int sen_logger_pass(sen_log_level level);

#define SEN_LOG(level,...) \
if (sen_logger_pass(level)) {\
  sen_logger_put((level), __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__);\
}

#ifndef SEN_LOG_DEFAULT_LEVEL
#define SEN_LOG_DEFAULT_LEVEL sen_log_notice
#endif /* SEN_LOG_DEFAULT_LEVEL */

#define sen_log(...) \
if (sen_logger_pass(SEN_LOG_DEFAULT_LEVEL)) {\
  sen_logger_put(SEN_LOG_DEFAULT_LEVEL, __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__);\
}

sen_rc sen_lex_set_mecab_args(int argc, char **argv);

#ifdef __cplusplus
}
#endif

#endif /* SENNA_H */
