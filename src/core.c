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

cr_object * cr_core_if(cr_imlist * args, cr_runtime * rt, cr_env * env){
  cr_object * condition = args->value;
  char buf[128];
  cr_show(buf, condition);
  cr_debug_info("evaluating if: %s", buf);
  args = args->next;
  cr_object * opt_if = args->value;
  args = args->next;
  cr_object * opt_else = args->value;
  if((cr_symbol *) condition == cr_true){
    return (cr_object *) cr_eval(opt_if, rt, env);
  }else{
    return (cr_object *) cr_eval(opt_else, rt, env);
  }
}
cr_object * cr_core_if_macro(cr_imlist * args, cr_runtime * rt, cr_env * env){
  cr_object * condition = args->value;
  args = args->next;
  cr_object * opt_if = args->value;
  args = args->next;
  cr_object * opt_else = args->value;
  cr_imlist * ret1 = cr_imlist_prependS(cr_imlist_empty, cr_quote_newS(opt_else));
  cr_imlist * ret2 = cr_imlist_prependS(ret1, cr_quote_newS(opt_if));
  cr_imlist * ret3 = cr_imlist_prependS(ret2, condition);
  cr_imlist * ret4 = cr_imlist_prependS(ret3, cr_symbol_new("if_"));
  return (cr_object *) ret4;
}


/* --- DEF FUNCTION AND MACRO --- */
cr_object * cr_core_def(cr_imlist * args, cr_runtime * rt, cr_env * env){
  if(rt->current == NULL){
    cr_debug_fail("No module specified for DEF!");
    return NULL;
  }
  cr_symbol * name = (cr_symbol *) args->value;
  if(name->prototype->type != cr_symbol_type){
    cr_debug_fail("Failed def! non-symbol name");
  }
  args = args->next;
  cr_object * value = args->value;
  if(args->next != cr_imlist_empty){
    cr_debug_fail("Failed def! too many arguments");
  }
  cr_env_set(rt->current, name, value);
  return cr_null;
}
cr_object * cr_core_def_macro(cr_imlist * args, cr_runtime * rt, cr_env * env){
  cr_symbol * name = (cr_symbol *) args->value;
  if(name->prototype->type != cr_symbol_type){
    cr_debug_fail("Failed def! non-symbol name");
  }
  args = args->next;
  cr_object * value = args->value;
  if(args->next != cr_imlist_empty){
    cr_debug_fail("Failed def! too many arguments");
  }
  cr_imlist * ret1 = cr_imlist_prependS(cr_imlist_empty, value);
  cr_imlist * ret2 = cr_imlist_prependS(ret1, cr_quote_newS(name));
  cr_imlist * ret3 = cr_imlist_prependS(ret2, cr_symbol_new("def_"));
  //list is now: (def_ 'name value)
  return (cr_object *) ret3;
}

/* --- FUNCTION CREATION FUNCTION AND MACRO --- */
cr_object * cr_core_fun(cr_imlist * args, cr_runtime * rt, cr_env * env){
  cr_imlist * arg_list = (cr_imlist *) args->value;
  if(arg_list->prototype->type != cr_list_type){
    cr_debug_warn("Failed to create function!");
    return (cr_object *) cr_tuple_error("function args must be a list");
  }
  args = args->next;
  cr_imlist * body = (cr_imlist *) args->value;
  if(body->prototype->type != cr_list_type){
    cr_debug_warn("Failed to create function!");
    return (cr_object *) cr_tuple_error("function body must be a list");
  }
  return (cr_object *) cr_fun_new(env, arg_list, body);
}
cr_object * cr_core_fun_macro(cr_imlist * args, cr_runtime * rt, cr_env * env){
  cr_imlist * arg_list = (cr_imlist *) args->value;
  args = args->next;
  cr_object * body = args->value;
  cr_imlist * ret1 = cr_imlist_prependS(cr_imlist_empty, cr_quote_newS(body));
  cr_imlist * ret2 = cr_imlist_prependS(ret1, cr_quote_newS(arg_list));
  cr_imlist * ret3 = cr_imlist_prependS(ret2, cr_symbol_new("fun_"));
  return (cr_object *) ret3;
}
/*
cr_object * cr_core_defmacro(cr_imlist * args, cr_runtime * rt, cr_env * env){
  cr_imlist * arg_list = (cr_imlist *) args->value;
  args = args->next;
  cr_imlist * body = (cr_imlist *) args->value;
  return (cr_object *) cr_macro_new(env, arg_list, body);
}*/
cr_object * cr_core_defmacro_macro(cr_imlist * args, cr_runtime * rt, cr_env * env){
  cr_imlist * arg_list = (cr_imlist *) args->value;
  args = args->next;
  cr_object * body = args->value;
  cr_imlist * ret1 = cr_imlist_prependS(cr_imlist_empty, cr_quote_newS(body));
  cr_imlist * ret2 = cr_imlist_prependS(ret1, cr_quote_newS(arg_list));
  cr_imlist * ret3 = cr_imlist_prependS(ret2, cr_symbol_new("macro_"));
  return (cr_object *) ret3;
}


/* --- SHITTY OLD PRINT FUNCTION -- */
cr_object * cr_core_print(cr_imlist * args, cr_runtime * rt, cr_env * env){
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

/* --- MODULE LOOKUP FUNCTION (in) AND MACRO --- */
cr_object * cr_core_in(cr_imlist * args, cr_runtime * rt, cr_env * env){
  cr_symbol * modname = (cr_symbol *) args->value;
  if(modname->prototype->type != cr_symbol_type){
    cr_debug_fail("Failed lookup (in) declaration! module name needs to be a symbol");
  }
  args = args->next;
  cr_symbol * symbol = (cr_symbol *) args->value;
  if(symbol->prototype->type != cr_symbol_type){
    cr_debug_fail("Failed lookup (in) declaration! need to look for symbol");
  }
  cr_env * module = cr_map_get(rt->modules, modname);
  if(module == NULL){
    cr_debug_fail("failed lookup! no such module %s", modname->name);
  }
  return cr_env_get(module, symbol);
}
cr_object * cr_core_in_macro(cr_imlist * args, cr_runtime * rt, cr_env * env){
  cr_symbol * module = (cr_symbol *) args->value;
  args = args->next;
  cr_object * symbol = args->value;
  cr_imlist * ret1 = cr_imlist_prependS(cr_imlist_empty, cr_quote_newS(module));
  cr_imlist * ret2 = cr_imlist_prependS(ret1, cr_quote_newS(symbol));
  cr_imlist * ret3 = cr_imlist_prependS(ret2, cr_symbol_new("in_"));
  return (cr_object *) ret3;
}

/* --- MODULE DECL FUNCTION AND MACRO --- */
cr_object * cr_core_module(cr_imlist * args, cr_runtime * rt, cr_env * env){
  cr_debug_info("having some fun!");
  cr_symbol * name = (cr_symbol *) args->value;
  cr_debug_info("Setting module: %s", name->name);
  if(name->prototype->type != cr_symbol_type){
    cr_debug_fail("Failed module declaration! non-symbol name");
  }
  if(cr_map_get(rt->modules, name) != NULL){
    cr_debug_fail("Duplicate module declaration, name: %s", name->name);
  }
  cr_env * module = cr_env_new(rt->core);
  cr_map_set(rt->modules, cr_ref(name), cr_ref(module));
  rt->current = module;
  return (cr_object *) cr_symbol_null;
}
cr_object * cr_core_module_macro(cr_imlist * args, cr_runtime * rt, cr_env * env){
  cr_symbol * name = (cr_symbol *) args->value;
  cr_imlist * ret1 = cr_imlist_prependS(cr_imlist_empty, cr_quote_newS(name));
  cr_imlist * ret2 = cr_imlist_prependS(ret1, cr_symbol_new("module_"));

  return (cr_object *) ret2;
}
cr_object * cr_core_eq(cr_imlist * args, cr_runtime * rt, cr_env * env){
  cr_object * prev = NULL;
  while(args != cr_imlist_empty){
    cr_object * obj = args->value;
    if(prev){
      if(cr_object_cmp(obj, prev) != 0){
        return (cr_object *) cr_false;
      }
    }
    prev = obj;
    args = args->next;
  }
  return (cr_object *) cr_true;
}
cr_object * cr_core_do(cr_imlist * args, cr_runtime * rt, cr_env * env){
  cr_object * object;
  while(args != cr_imlist_empty){
    object = cr_eval(args->value, rt, env);
    args = args->next;
  }
  return object;
}


void _add_fun(cr_env * env, char * name, cr_native_function function){
  cr_debug_info("Adding core function: %s", name);
  cr_env_setS(env, cr_symbol_new(name), cr_fun_native_new(function));
}

/*
void _add_macro(cr_env * env, char * name, cr_native_function function){
  cr_debug_info("Adding core macro: %s", name);
  cr_env_setS(env, cr_symbol_new(name), cr_macro_native_new(function));
}
*/
void cr_core_init(){
}
void cr_core_register(cr_env * env){
  _add_fun(env, "module_", cr_core_module);
  //_add_macro(env, "module", cr_core_module_macro);

  _add_fun(env, "in_", cr_core_in);
  //_add_macro(env, "in", cr_core_in_macro);

  _add_fun(env, "def_", cr_core_def);
  //_add_macro(env, "def", cr_core_def_macro);

  _add_fun(env, "fun_", cr_core_fun);
  //_add_macro(env, "fun", cr_core_fun_macro);

//  _add_fun(env, "macro_", cr_core_defmacro);
  //_add_macro(env, "macro", cr_core_defmacro_macro);

  _add_fun(env, "print", cr_core_print);
  _add_fun(env, "if_", cr_core_if);
  _add_fun(env, "do_", cr_core_do);
  //_add_macro(env, "if", cr_core_if_macro);
  _add_fun(env, "=", cr_core_eq);
}
