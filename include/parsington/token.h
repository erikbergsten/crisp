/**
 * @file token.h
 * @author Erik Bergsten
 * @date Thu 03 Aug 2017
 * @brief Contains the pt_token type and its subtypes.
 * The constant tokens need to be initialized (pt_token_init) and can be
 * freed (they use 20 bytes of memory so maybe its not that important)
 * using pt_token_finalize().
 */

#ifndef __parsington_token_h__
#define __parsington_token_h__

#include <stddef.h>

/**
 * @brief Enumeration of the parsington token types.
 */
typedef enum {
  pt_integer_tt,
  pt_float_tt,
  pt_symbol_tt,
  pt_string_tt,
  pt_tuple_start_tt,
  pt_tuple_end_tt,
  pt_list_start_tt,
  pt_quote_tt,
  pt_list_end_tt,
  pt_lookup_tt /**< Lookup tokens are references to symbols in modules, e.g foo/bar. */
} pt_token_type;

/**
 * @brief Base struct for pt_tokens
 */
typedef struct {
  pt_token_type type;
} pt_token;

typedef struct {
  pt_token_type type;
  size_t value;
} pt_integer_token;
typedef struct {
  pt_token_type type;
  double value;
} pt_float_token;
typedef struct {
  pt_token_type type;
  char * value;
} pt_symbol_token;
typedef struct {
  pt_token_type type;
  char * module, * name;
} pt_lookup_token;

/**
 * @brief pt_token_init initializes the constant tokens
 */
void pt_token_init();
/**
 * @brief pt_token_finalize frees up the constant tokens and only needs to be called if
 * no more parsing is to be done. Mostly used for memchecking.
 */
void pt_token_finalize();

/**
 * @brief Create a new pt_integer_token, cast to a pt_token
 */
pt_token * pt_integer_token_new(size_t value);
/**
 * @brief Create a new pt_float_token, cast to a pt_token
 */
pt_token * pt_float_token_new(double value);
pt_token * pt_symbol_token_new(char * value);
pt_token * pt_string_token_new(char * value);
pt_token * pt_lookup_token_new(char * module, char * name);


/**
 * @brief The pt_token_free function is safe to call on every token created using
 * the functions and on the constant tokens.
 */
void pt_token_free(pt_token * token);

#define pt_token_freeS(token) pt_token_free((pt_token *) token)

typedef pt_symbol_token pt_string_token;


/**
 * @brief Constant tokens with no unique values attached are declared here and
 * defined in pt_token_init
 */
extern pt_token * pt_list_start_token,
                * pt_list_end_token,
                * pt_tuple_start_token,
                * pt_tuple_end_token,
                * pt_quote_token;


#endif /* __parsington_token_h__ */
