#include <crisp/core/core.h>
#include <crisp/gc.h>
#include <crisp/function.h>
#include <crisp/quote.h>
#include <crisp/run.h>
#include <crisp/eval.h>
#include <crisp/debug.h>
#include <crisp/show.h>
#include <crisp/tuple.h>
#include <crisp/integer.h>
#include <crisp/double.h>

cr_object * cr_core_if(cr_list * args, cr_runtime * rt, cr_env * env){
  cr_node * arg = args->head;
  cr_object * condition = arg->value;
  char buf[128];
  cr_show(buf, condition);
  cr_object * evaled_condition = cr_eval(condition, rt, env);
  arg = arg->next;
  cr_object * opt_if = arg->value;
  arg = arg->next;
  cr_object * opt_else = arg->value;
  if((cr_symbol *) evaled_condition == cr_true){
    return (cr_object *) cr_eval(opt_if, rt, env);
  }else{
    return (cr_object *) cr_eval(opt_else, rt, env);
  }
}

/* --- SHITTY OLD PRINT FUNCTION -- */
cr_object * cr_core_print(cr_list * args, cr_runtime * rt, cr_env * env){
  cr_object * object = args->head->value;
  char buf[128];
  cr_show(buf, object);
  printf("%s ", buf);
  printf("\n");
  return (cr_object *) cr_symbol_null;
}

cr_object * cr_core_eq(cr_list * args, cr_runtime * rt, cr_env * env){
  cr_object * prev = NULL;
  for(cr_node * node = args->head; node; node = node->next){
    cr_object * obj = node->value;
    if(prev){
      if(cr_object_cmp(obj, prev) != 0){
        return (cr_object *) cr_false;
      }
    }
    prev = obj;
  }
  return (cr_object *) cr_true;
}
cr_object * cr_core_do(cr_imlist * args, cr_runtime * rt, cr_env * env){
  /*
  cr_object * object;
  while(args != cr_imlist_empty){
    object = cr_eval(args->value, rt, env);
    args = args->next;
  }
  return object;
  */
}

void _add_fun(cr_env * env, char * name, cr_native_function function){
  cr_debug_info("Adding core function: %s", name);
  cr_env_setS(env, cr_symbol_new(name), cr_fun_native_new(function));
}
void _add_macro(cr_env * env, char * name, cr_native_function function){
  cr_debug_info("Adding core macro: %s", name);
  cr_env_setS(env, cr_symbol_new(name), cr_macro_native_new(function));
}

void cr_core_init(){
}
void cr_core_register(cr_env * env){
  _add_fun(env, "print", cr_core_print);
  _add_fun(env, "iffun", cr_core_if);
  _add_macro(env, "if", cr_core_if);
  _add_fun(env, "dofun", cr_core_do);
  _add_fun(env, "=", cr_core_eq);
}
