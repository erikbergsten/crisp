#include <crisp/eval.h>
#include <stdio.h>
#include <crisp/debug.h>
#include <crisp/integer.h>
#include <crisp/show.h>
#include <crisp/gc.h>
#include <crisp/double.h>
#include <crisp/env.h>
#include <crisp/quote.h>
#include <crisp/imlist.h>
#include <crisp/function.h>
#include <crisp/debug.h>

cr_object * _eval_fun(cr_fun_normal * fun,
                      cr_imlist * arg_values,
                      cr_env * env){
  cr_env * fun_env = cr_ref(cr_env_new(fun->env));
  cr_imlist * arg_names = fun->args;
  char buf[128];
  while(arg_names != cr_imlist_empty){
    cr_symbol * sym = (cr_symbol *) arg_names->value;
    cr_object * val;
    if(arg_values != cr_imlist_empty){
      cr_show(buf, arg_values->value);
      val = cr_eval(arg_values->value, env);
      cr_show(buf, val);
    }else{
      val = (cr_object *) cr_symbol_null;
    }
    cr_env_set(fun_env, sym, val);
    arg_names = arg_names->next;
  }
  cr_show(buf, (cr_object *) fun->body);
  cr_object * object = cr_eval((cr_object *)fun->body, fun_env);
  cr_free(fun_env);
  cr_free(fun);
  return object;
}
cr_object * _eval_list(cr_imlist * list, cr_env * env){
  cr_fun * fun = (cr_fun *) cr_eval(list->value, env);
  cr_ref(fun);
  if(fun->prototype->type != cr_function_type){
    cr_debug_warn("Trying to apply non function as function! Actual type is: %s",
        fun->prototype->name);
    cr_free(fun);
    return NULL;
  }
  if(fun->form == cr_function_normal){
    return _eval_fun((cr_fun_normal *) fun, list->next, env);
  }else{
    cr_fun_native * native = (cr_fun_native *) fun;
    cr_imlist * args = list->next;
    cr_imlist * evald = cr_imlist_empty;
    while(args != cr_imlist_empty){
      cr_object * obj = (cr_object *) args->value;
      evald = cr_imlist_prepend(evald, cr_eval(obj, env));
      args = args->next;
    }
    cr_ref(evald);
    cr_object * ret = native->function(evald, env);
    char buf[128];
    cr_show(buf, ret);
    cr_free(evald);
    cr_free(fun);
    return ret;
  }
  return NULL;
}
cr_object * cr_eval(cr_object * object, cr_env * env){
  switch(object->prototype->type){
    case cr_symbol_type:
      return cr_env_get(env, (cr_symbol *) object);
      break;
    case cr_list_type:
      return _eval_list((cr_imlist *) object, env);
      break;
    case cr_quote_type:
      return ((cr_quote *) object)->value;
      break;
    default:
      return object;
      break;
  }
}
