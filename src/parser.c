#include <parsington/parser.h>
#include <parsington/tokenizer.h>
#include <parsington/token.h>
#include <crisp/debug.h>
#include <crisp/list.h>
#include <crisp/tuple.h>
#include <crisp/imlist.h>
#include <crisp/symbol.h>
#include <crisp/quote.h>
#include <crisp/object.h>
#include <crisp/lookup.h>
#include <crisp/gc.h>
#include <crisp/integer.h>
#include <crisp/double.h>

cr_object * _parse_object(pt_tokenizer * tz, pt_token * token);

cr_object * _parse_lookup(pt_lookup_token * token){
  cr_lookup * lu = cr_lookup_new(token->module, token->name);
  pt_token_free(token);
  return (cr_object *) lu;
}
cr_object * _parse_string(pt_string_token * token){
  cr_imlist * list = cr_imlist_empty;
  char * c = token->value;
  do{
    list = cr_imlist_prependS(list, cr_integer_new((int) *c));
  }while((*(++c)) != '\0');
  pt_token_freeS(token);
  return (cr_object *) list;
}
cr_object * _parse_symbol(pt_symbol_token * token){
  cr_symbol * sym = cr_symbol_new(token->value);
  pt_token_freeS(token);
  return (cr_object *) sym;
}
cr_object * _parse_double(pt_float_token * token){
  cr_double * dubbs = cr_double_new(token->value);
  pt_token_freeS(token);
  return (cr_object *) dubbs;
}
cr_object * _parse_integer(pt_integer_token * token){
  cr_integer * i = cr_integer_new(token->value);
  pt_token_freeS(token);
  return (cr_object *) i;
}
cr_object * _parse_quote(pt_tokenizer * tz){
  pt_token * token = pt_tokenizer_next(tz);
  cr_object * obj = _parse_object(tz, token);
  if(obj == NULL){
    tz->status = PT_TOKENIZER_FAILED;
    tz->error = "unfinished quote";
    return NULL;
  }else{
    return (cr_object *) cr_quote_new(obj);
  }
}
cr_object * _parse_tuple(pt_tokenizer * tz){
  cr_list * elems = cr_list_newP();
  while(1){
    pt_token * token = pt_tokenizer_next(tz);
    if(token == pt_tuple_end_token){
      cr_tuple * tuple = cr_tuple_from_list(elems);
      cr_list_destroy(elems);
      return (cr_object *) tuple;
    }else{
      cr_object * elem = _parse_object(tz, token);
      cr_list_append(elems, elem);
    }
  }
}
cr_object * _parse_list(pt_tokenizer * tz){
  cr_imlist * list = cr_imlist_empty;
  while(1){
    pt_token * token = pt_tokenizer_next(tz);
    if(token == pt_list_end_token){
      //done
      cr_object * ret = (cr_object *) cr_imlist_reverse(list);
      cr_free(list);
      return ret;
    }else{
      cr_object * obj = _parse_object(tz, token);
      if(obj == NULL){
        tz->status = PT_TOKENIZER_FAILED;
        tz->error = "missing list terminator";
        return NULL;
      }else{
        list = cr_imlist_prependS(list, obj);
      }
    }
  }
}
cr_object * _parse_object(pt_tokenizer * tz, pt_token * token){
  if(token == NULL){
    return NULL;
  }else if(token == pt_list_start_token){
    return _parse_list(tz);
  }else if(token == pt_tuple_start_token){
    return _parse_tuple(tz);
  }else if(token == pt_quote_token){
    return _parse_quote(tz);
  }else if(token->type == pt_integer_tt){
    return _parse_integer((pt_integer_token *) token);
  }else if(token->type == pt_float_tt){
    return _parse_double((pt_float_token *) token);
  }else if(token->type == pt_symbol_tt){
    return _parse_symbol((pt_symbol_token *) token);
  }else if(token->type == pt_string_tt){
    return _parse_string((pt_string_token *) token);
  }else if(token->type == pt_lookup_tt){
    return _parse_lookup((pt_lookup_token *) token);
  }else{
    tz->status = PT_TOKENIZER_FAILED;
    tz->error = "unknown token type";
  }
  return NULL;
}
cr_object * pt_parser_next(pt_tokenizer * tz){
  if(pt_tokenizer_finished(tz)){
    return NULL;
  }else if(pt_tokenizer_failed(tz)){
    return NULL;
  }
  pt_token * token = pt_tokenizer_next(tz);
  return _parse_object(tz, token);
}
cr_object * pt_parser_parse_str(char * str){
  pt_tokenizer * tz = pt_tokenizer_read_string(str);
  cr_object * object = pt_parser_next(tz);
  pt_tokenizer_free(tz);
  return object;
}
