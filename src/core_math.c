#include <crisp/core/math.h>
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

cr_object * cr_core_math_add(cr_imlist * args, cr_runtime * rt, cr_env * env){
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
cr_object * cr_core_math_mul(cr_imlist * args, cr_runtime * rt, cr_env * env){
  double value = 1;
  int dubs = 0;
  while(args != cr_imlist_empty){
    cr_object * obj = (cr_object *) args->value;
    if(obj->prototype->type == cr_integer_type){
      cr_integer * i = (cr_integer *) obj;
      value *= i->value;
    }else if(obj->prototype->type == cr_double_type){
      dubs = 1;
      cr_double * d = (cr_double *) obj;
      value *= d->value;
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
cr_object * cr_core_math_div(cr_imlist * args, cr_runtime * rt, cr_env * env){
  double value = 0;
  int dubs = 0;
  cr_object * obj = args->value;
  if(obj->prototype->type == cr_double_type){
    cr_double * d = (cr_double *) obj;
    dubs = 1;
    value = d->value;
  }else if(obj->prototype->type == cr_integer_type){
    cr_integer * i = (cr_integer *) obj;
    value = i->value;
  }
  args = args->next;
  while(args != cr_imlist_empty){
    cr_object * obj = (cr_object *) args->value;
    if(obj->prototype->type == cr_integer_type){
      cr_integer * i = (cr_integer *) obj;
      value /= i->value;
    }else if(obj->prototype->type == cr_double_type){
      dubs = 1;
      cr_double * d = (cr_double *) obj;
      value /= d->value;
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
cr_object * cr_core_math_sub(cr_imlist * args, cr_runtime * rt, cr_env * env){
  double value = 0;
  int dubs = 0;
  cr_object * obj = args->value;
  if(obj->prototype->type == cr_double_type){
    cr_double * d = (cr_double *) obj;
    dubs = 1;
    value = d->value;
  }else if(obj->prototype->type == cr_integer_type){
    cr_integer * i = (cr_integer *) obj;
    value = i->value;
  }
  args = args->next;
  while(args != cr_imlist_empty){
    cr_object * obj = (cr_object *) args->value;
    if(obj->prototype->type == cr_integer_type){
      cr_integer * i = (cr_integer *) obj;
      value -= i->value;
    }else if(obj->prototype->type == cr_double_type){
      dubs = 1;
      cr_double * d = (cr_double *) obj;
      value -= d->value;
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


void _add_core_math_fun(cr_env * env, char * name, cr_native_function function){
  cr_debug_info("Adding core math function: %s", name);
  cr_env_setS(env, cr_symbol_new(name), cr_fun_native_new(function));
}


void cr_core_math_register(cr_env * env){
  _add_core_math_fun(env, "+", cr_core_math_add);
  _add_core_math_fun(env, "*", cr_core_math_mul);
  _add_core_math_fun(env, "/", cr_core_math_div);
  _add_core_math_fun(env, "-", cr_core_math_sub);
}
