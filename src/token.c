#include <parsington/token.h>
#include <stdlib.h>
#include <string.h>
#include <crisp/debug.h>

int tokens_allocated = 0;
pt_token  * pt_list_start_token,
          * pt_list_end_token,
          * pt_tuple_start_token,
          * pt_tuple_end_token,
          * pt_quote_token;

pt_token * _pt_init_token(pt_token_type type){
  pt_token * token = (pt_token *) malloc(sizeof(pt_token));
  token->type = type;
  return token;
}
void pt_token_init(){
  pt_list_start_token = _pt_init_token(pt_list_start_tt);
  pt_list_end_token = _pt_init_token(pt_list_end_tt);
  pt_tuple_start_token = _pt_init_token(pt_tuple_start_tt);
  pt_tuple_end_token = _pt_init_token(pt_tuple_end_tt);
  pt_quote_token = _pt_init_token(pt_quote_tt);
}
void pt_token_finalize(){
  cr_debug_info("tokens allocated: %i", tokens_allocated);
  free(pt_list_start_token);
  free(pt_list_end_token);
  free(pt_tuple_start_token);
  free(pt_tuple_end_token);
  free(pt_quote_token);
}
pt_token * pt_integer_token_new(size_t value){
  tokens_allocated++;
  pt_integer_token * token = (pt_integer_token *) malloc(sizeof(pt_integer_token));
  token->type = pt_integer_tt;
  token->value = value;
  return (pt_token *) token;
}
pt_token * pt_float_token_new(double value){
  tokens_allocated++;
  pt_float_token * token = (pt_float_token *) malloc(sizeof(pt_float_token));
  token->type = pt_float_tt;
  token->value = value;
  return (pt_token *) token;
}
pt_token * pt_symbol_token_new(char * value){
  tokens_allocated++;
  pt_symbol_token * token = (pt_symbol_token *) malloc(sizeof(pt_symbol_token));
  token->value = (char *) malloc(sizeof(char) * (strlen(value) + 1));;
  token->type = pt_symbol_tt;
  strcpy(token->value, value);
  return (pt_token *) token;
}
pt_token * pt_string_token_new(char * value){
  tokens_allocated++;
  pt_symbol_token * token = (pt_symbol_token *) malloc(sizeof(pt_symbol_token));
  token->value = (char *) malloc(sizeof(char) * (strlen(value) + 1));;
  token->type = pt_string_tt;
  strcpy(token->value, value);
  return (pt_token *) token;
}
pt_token * pt_lookup_token_new(char * module, char * name){
  tokens_allocated++;
  pt_lookup_token * token = (pt_lookup_token *) malloc(sizeof(pt_lookup_token));
  token->type = pt_lookup_tt;
  token->module = (char *) malloc(sizeof(char) * (strlen(module) + 1));;
  strcpy(token->module, module);
  token->name = (char *) malloc(sizeof(char) * (strlen(name) + 1));;
  strcpy(token->name, name);
  return (pt_token *) token;
}
void pt_token_free(pt_token * token){
  tokens_allocated--;
  switch(token->type){
    case pt_lookup_tt:
      free(((pt_lookup_token *) token)->name);
    case pt_string_tt:
    case pt_symbol_tt:
      free(((pt_symbol_token *) token)->value);
    case pt_float_tt:
    case pt_integer_tt:
      free(token);
      break;
    default:
      tokens_allocated++;
      break;
  }
}
