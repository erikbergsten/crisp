#include <crisp/core.h>
#include <crisp/gc.h>
#include <crisp/function.h>
#include <crisp/debug.h>
#include <crisp/show.h>
#include <crisp/tuple.h>
#include <crisp/integer.h>
#include <crisp/double.h>


cr_object * cr_core_add(cr_imlist * args, cr_env * env){
  double value = 0;
  int dubs = 0;
  while(args != cr_imlist_empty){
    cr_object * obj = (cr_object *) args->value;
    if(obj->prototype->type == cr_integer_type){
      cr_integer * i = (cr_integer *) obj;
      value += i->value;
    }else if(obj->prototype->type == cr_double_type){
      dubs = 1;
      cr_double * d = (cr_double *) obj;
      value += d->value;
    }else{
      return (cr_object *) cr_tuple_error("+ only works with integers and doubles");
    }
    args = args->next;
  }
  if(dubs){
    return (cr_object *) cr_double_new(value);
  }else{
    return (cr_object *) cr_integer_new((int) value);
  }
}

cr_object * cr_core_fun(cr_imlist * args, cr_env * env){
  cr_imlist * body = (cr_imlist *) args->value;
  if(body->prototype->type != cr_list_type){
    cr_debug_warn("Failed to create function!");
    return (cr_object *) cr_tuple_error("function body must be a list");
  }
  args = args->next;
  cr_imlist * arg_list = (cr_imlist *) args->value;
  if(arg_list->prototype->type != cr_list_type){
    cr_debug_warn("Failed to create function!");
    return (cr_object *) cr_tuple_error("function args must be a list");
  }
  return (cr_object *) cr_fun_new(env, arg_list, body);
}

cr_object * cr_core_print(cr_imlist * args, cr_env * env){
  while(args != cr_imlist_empty){
    cr_object * object = args->value;
    char buf[128];
    cr_show(buf, object);
    printf("%s ", buf);
    args = args->next;
  }
  printf("\n");
  return (cr_object *) cr_symbol_null;
}


void _add_fun(cr_env * env, char * name, cr_native_function function){
  cr_debug_info("Adding core function: %s", name);
  cr_env_setS(env, cr_symbol_new(name), cr_fun_native_new(function));
}


void cr_core_init(cr_env * env){
  _add_fun(env, "fun", cr_core_fun);
  _add_fun(env, "print", cr_core_print);
  _add_fun(env, "+", cr_core_add);
}
