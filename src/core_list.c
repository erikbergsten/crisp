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
#include <crisp/core/list.h>

cr_object * cr_core_list_conj(cr_imlist * args, cr_runtime * rt, cr_env * env){
  cr_object * next = args->value;
  args = args->next;
  cr_imlist * list = (cr_imlist *) args->value;
  // type check list
  return (cr_object *) cr_imlist_prepend(list, next);

}
cr_object * cr_core_list_head(cr_imlist * args, cr_runtime * rt, cr_env * env){
  cr_imlist * list = (cr_imlist *) args->value;
  if(list == cr_imlist_empty){
    return cr_null;
  }
  return (cr_object *) list->value;
}
cr_object * cr_core_list_tail(cr_imlist * args, cr_runtime * rt, cr_env * env){
  cr_imlist * list = (cr_imlist *) args->value;
  if(list == cr_imlist_empty){
    return cr_null;
  }
  return (cr_object *) list->next;
}

void _add_core_list_fun(cr_env * env, char * name, cr_native_function function){
  cr_debug_info("Adding core list function: %s", name);
  cr_env_setS(env, cr_symbol_new(name), cr_fun_native_new(function));
}


void cr_core_list_register(cr_env * env){
  _add_core_list_fun(env, "conj", cr_core_list_conj);
  _add_core_list_fun(env, "head", cr_core_list_head);
  _add_core_list_fun(env, "tail", cr_core_list_tail);
}
