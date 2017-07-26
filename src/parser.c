#include <crisp/parser.h>
#include <crisp/tokenizer.h>
#include <crisp/debug.h>
#include <crisp/list.h>
#include <crisp/tuple.h>
#include <crisp/imlist.h>
#include <crisp/symbol.h>
#include <crisp/quote.h>
#include <crisp/object.h>
#include <crisp/gc.h>
#include <crisp/integer.h>
#include <crisp/double.h>
cr_object * _read_object(cr_token * token, cr_list * tokens);

cr_object * _parse_string(cr_text_token * token){
  cr_object * obj = (cr_object *) cr_imlist_from_str(token->str);
  free(token->str);
  free(token);
  return obj;
}
cr_object * _parse_tuple(cr_list * tokens){
  cr_token * token = NULL;
  cr_list * build = cr_list_newP();
  int n = 0;
  while((token = cr_list_shift(tokens)) != NULL){
    if(token->type == cr_tuple_end){
      cr_object ** objects = malloc(sizeof(cr_object*) * n);
      for(int i = 0; i < n; i++){
        objects[i] = cr_list_shift(build);
      }
      cr_tuple * tup = cr_tuple_new(n, objects);
      free(objects);
      cr_list_destroy(build);
      return (cr_object *) tup;
    }else{
      cr_object * elem = _read_object(token, tokens);
      n++;
      cr_list_append(build, elem);
    }
  }
  cr_debug_warn("FAILED TUPLE PARSE! NULL TOKNE\n");
  return NULL;
}

cr_object * _parse_list(cr_list * tokens){
  cr_token * token = NULL;
  cr_list * build = cr_list_newP();
  cr_debug_info("parsing list");
  while((token = cr_list_shift(tokens)) != NULL){
    if(token->type == cr_list_end){
      cr_imlist * list = cr_imlist_empty;
      cr_object * object = NULL;
      while((object = cr_list_pop(build)) != NULL){
        cr_debug_info("adding to imlist..");
        list = cr_imlist_prepend(list, object);
      }
      cr_debug_info("imlist finished!");
      cr_list_destroy(build);
      return (cr_object *) list;
    }else{
      cr_debug_info("adding to list build");
      cr_object * elem = _read_object(token, tokens);
      cr_list_append(build, elem);
    }
  }
  cr_debug_warn("FAILED LIST PARSE - NULL TOKEN");
  return NULL;
}

cr_object * _parse_quote(cr_list * tokens){
  cr_token * token = cr_list_shift(tokens);
  cr_object * obj = _read_object(token, tokens);
  return (cr_object *) cr_quote_new(obj);
}
cr_object * _read_object(cr_token * token, cr_list * tokens){
  switch(token->type){
    case cr_int_token_type:
      {
        cr_int_token * int_tok = (cr_int_token *) token;
        cr_integer * i = cr_integer_new(int_tok->value);
        free(int_tok);
        return (cr_object *) i;
        break;
      }
    case cr_float_token_type:
      {
        cr_float_token * f_tok = (cr_float_token *) token;
        cr_double * d = cr_double_new(f_tok->value);
        free(f_tok);
        return (cr_object *) d;
        break;
      }
    case cr_symbol_token_type:
      {
        cr_text_token * sym_tok = (cr_text_token *) token;
        cr_symbol * s = cr_symbol_new(sym_tok->str);
        free(sym_tok->str);
        free(sym_tok);
        return (cr_object *) s;
        break;
      }
    case cr_string_token_type:
      {
        return _parse_string((cr_text_token *) token);
        break;
      }
    case cr_list_start:
      {
        return _parse_list(tokens);
        break;
      }
    case cr_tuple_start:
      {
        return _parse_tuple(tokens);
        break;
      }
    case cr_quote_token_type:
      {
        return _parse_quote(tokens);
        break;
      }
    default:
      {
        cr_debug_fail("UNKNOWN TOKEN!\n");
        return NULL;
        break;
      }
  }
}

cr_list * cr_parse(cr_list * tokens){
  cr_list * objects = cr_list_newP();
  cr_token * token = NULL;
  while((token = cr_list_shift(tokens)) != NULL){
    cr_object * obj = _read_object(token, tokens);
    cr_list_append(objects, obj);
  }
  return objects;
}

