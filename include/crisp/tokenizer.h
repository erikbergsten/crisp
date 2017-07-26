#ifndef __crisp_tokens_h__
#define __crisp_tokens_h__

#include <crisp/list.h>

typedef enum {
  cr_int_token_type,
  cr_float_token_type,
  cr_symbol_token_type,
  cr_string_token_type,
  cr_tuple_start,
  cr_tuple_end,
  cr_quote_token_type,
  cr_list_start,
  cr_list_end
} cr_token_type;

typedef struct {
  cr_token_type type;
} cr_token;

typedef struct {
  cr_token_type type;
  char * str;
} cr_text_token;
typedef struct {
  cr_token_type type;
  int value;
} cr_int_token;
typedef struct {
  cr_token_type type;
  double value;
} cr_float_token;

cr_list * cr_tokenize_file(char * src);
cr_list * cr_tokenize_str(char * str);

#endif /* __crisp_tokens_h__ */
