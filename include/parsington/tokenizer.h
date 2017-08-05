/**
 * @file tokenizer.h
 * @author Erik Bergsten
 * @date Thu 03 Aug 2017
 * @brief The tokenizer generates tokens from files or strings of valid
 * crisp code.
 */
#ifndef __parsington_tokenizer_h__
#define __parsington_tokenizer_h__

#define PT_TOKENIZER_RUNNING 0
#define PT_TOKENIZER_INIT 1
#define PT_TOKENIZER_SUCCESS 2
#define PT_TOKENIZER_FAILED 3

#include <stdio.h>
#include <parsington/token.h>

typedef enum {
  pt_file_source,
  pt_string_source
} pt_tokenizer_source;
typedef enum {
  pt_default_mode,
  pt_integer_mode,
  pt_symbol_mode,
  pt_float_mode,
  pt_string_mode,
  pt_char_mode,
  pt_lookup_mode
} pt_tokenizer_mode;
typedef struct {
  int row, col;
  int status;
  pt_tokenizer_mode mode;
  char * error;
  char peek;
  char buf[128];
  int buf_at;
  pt_tokenizer_source source;
} pt_tokenizer;
typedef struct {
  pt_tokenizer tokenizer;
  int index;
  char * str;
} pt_string_tokenizer;
typedef struct {
  pt_tokenizer tokenizer;
  FILE * file;
} pt_file_tokenizer;

pt_tokenizer * pt_tokenizer_read_string(char * str);
pt_tokenizer * pt_tokenizer_read_file(FILE * file);
pt_tokenizer * pt_tokenizer_read_path(char * src);

pt_token * pt_tokenizer_next(pt_tokenizer * tz);

void pt_tokenizer_free(pt_tokenizer * tz);

#define pt_tokenizer_row(tz) (tz->row)
#define pt_tokenizer_col(tz) (tz->col)
#define pt_tokenizer_running(tz) (tz->status == PT_TOKENIZER_RUNNING)
#define pt_tokenizer_failed(tz) (tz->status == PT_TOKENIZER_FAILED)
#define pt_tokenizer_finished(tz) (tz->status == PT_TOKENIZER_SUCCESS)
#define pt_tokenizer_get_error(tz) (tz->error)



#endif /* __parsington_tokenizer_h__ */
