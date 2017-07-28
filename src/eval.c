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
                      cr_runtime * runtime,
                      cr_imlist * arg_values,
                      cr_env * env){
  cr_env * fun_env = cr_ref(cr_env_new(fun->env));
  cr_imlist * arg_names = fun->args;
  char buf[128];
  while(arg_names != cr_imlist_empty){
    cr_symbol * sym = (cr_symbol *) arg_names->value;
    cr_object * val;
    if(arg_values != cr_imlist_empty){
      if(fun->macro){
        val = arg_values->value;
        cr_show(buf, val);
        cr_debug_info("setting %s to %s in macro call!", sym->name, buf);
      }else{
        cr_show(buf, arg_values->value);
        val = cr_eval(arg_values->value, runtime, env);
        cr_show(buf, val);
        cr_debug_info("setting %s to %s in function call!", sym->name, buf);
      }
    }else{
      val = (cr_object *) cr_symbol_null;
    }
    cr_env_set(fun_env, sym, val);
    arg_names = arg_names->next;
    arg_values = arg_values->next;
  }
  cr_show(buf, (cr_object *) fun->body);
  cr_debug_info("evaluating function body: %s", buf);
  cr_object * object = cr_eval((cr_object *)fun->body, runtime, fun_env);
  cr_show(buf, (cr_object *) object);
  cr_debug_info("function returned: %s", buf);
  cr_free(fun_env);
  if(fun->macro){
    cr_debug_info("EVALUATING MACRO HERE BBOYS");
    cr_free(fun);
    cr_show(buf, object);
    cr_debug_info("evaluating returned object: %s", buf);
    return cr_eval(object, runtime, env);
  }else{
    cr_debug_info("EVALUATING NON MACRO HERE BBOYS");
    cr_free(fun);
    return object;
  }
}
cr_object * _eval_list(cr_imlist * list, cr_runtime * runtime, cr_env * env){
  char buf[128];
  cr_show(buf, list);
  cr_debug_info("evaluating list %s", buf);
  cr_show(buf, list->value);
  cr_debug_info("evaluating function %s", buf);
  cr_fun * fun = (cr_fun *) cr_eval(list->value, runtime, env);
  cr_showS(buf, fun);
  cr_debug_info("got fun: %s", buf);
  cr_ref(fun);
  if(fun->prototype->type != cr_function_type){
    cr_debug_warn("Trying to apply non function as function! Actual type is: %s",
        fun->prototype->name);
    cr_free(fun);
    return NULL;
  }
  if(fun->form == cr_function_normal){
    return _eval_fun((cr_fun_normal *) fun, runtime, list->next, env);
  }else{
    cr_fun_native * native = (cr_fun_native *) fun;
    cr_imlist * args = list->next;
    cr_object * ret;
    if(fun->macro){
      ret = native->function(args, runtime, env);
    }else{
      cr_imlist * evald = NULL, *tmp = NULL;
      while(args != cr_imlist_empty){
        cr_object * obj = (cr_object *) args->value;
        cr_object * e_obj = cr_eval(obj, runtime, env);
        cr_imlist * im = cr_imlist_prepend(cr_imlist_empty, e_obj);
        if(tmp){
          tmp->next = cr_ref(im);
        }else{
          evald = im;
        }
        tmp = im;
        args = args->next;
      }
      cr_ref(evald);
      ret = native->function(evald, runtime, env);
      cr_free(evald);
    }
    char buf[128];
    cr_show(buf, ret);
    if(fun->macro){
      cr_free(fun);
      cr_debug_info("native macro returned: %s", buf);
      cr_object * obj = cr_eval(ret, runtime, env);
      cr_free(ret);
      return obj;
    }else{
      cr_debug_info("native returned: %s", buf);
      cr_free(fun);
      return ret;
    }
  }
  return NULL;
}
cr_object * cr_eval(cr_object * object, cr_runtime * runtime, cr_env * env){
  switch(object->prototype->type){
    case cr_symbol_type:
      {
      cr_symbol * sym = (cr_symbol *) object;
      cr_debug_info("evaluating symbol: %s", sym->name);
      cr_object * val = cr_env_get(env, (cr_symbol *) object);
      if(val){
        char buf[128];
        cr_show(buf, val);
        cr_debug_info("got: %s", buf);
      }else{
        printf("got NULL");
      }
      return val;
      break;
      }
    case cr_list_type:
      return _eval_list((cr_imlist *) object, runtime, env);
      break;
    case cr_quote_type:
      return ((cr_quote *) object)->value;
      break;
    default:
      return object;
      break;
  }
}
