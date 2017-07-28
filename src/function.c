#include <crisp/function.h>
#include <crisp/gc.h>
#include <crisp/debug.h>

void cr_fun_destroy(cr_fun * fun){
  if(fun->form == cr_function_normal){
    cr_fun_normal * nfun = (cr_fun_normal *) fun;
    cr_free(nfun->env);
    cr_free(nfun->args);
    cr_free(nfun->body);
  }
}


cr_prototype cr_fun_prototype = {
  cr_function_type,
  "function",
  (cr_hash) NULL,
  (cr_cmp) NULL
};

cr_fun * _cr_fun_new(int macro, cr_env * env, cr_imlist * args, cr_imlist * body){
  cr_fun_normal * fun =
    (cr_fun_normal *) cr_mallocS(sizeof(cr_fun_normal), cr_fun_destroy);
  fun->prototype = &cr_fun_prototype;
  fun->macro = macro;
  fun->env = cr_ref(env);
  fun->form = cr_function_normal;
  fun->args = cr_ref(args);
  fun->body = cr_ref(body);
  fun->overloaded = 0;
  return (cr_fun *) fun;
}
cr_fun * cr_macro_new(cr_env * env, cr_imlist * args, cr_imlist * body){
  return _cr_fun_new(1, env, args, body);
}
cr_fun * cr_fun_new(cr_env * env, cr_imlist * args, cr_imlist * body){
  return _cr_fun_new(0, env, args, body);
}
void native_killer(cr_fun_native * fun){
  cr_debug_info("killing native!");
}
cr_fun * _cr_fun_native_new(int macro, cr_native_function function){
  cr_fun_native * fun =
    (cr_fun_native *) cr_mallocS(sizeof(cr_fun_native), native_killer);
  fun->function = function;
  fun->macro = macro;
  fun->form = cr_function_native;
  fun->prototype = &cr_fun_prototype;
  return (cr_fun *) fun;
}
cr_fun * cr_fun_native_new(cr_native_function function){
  return _cr_fun_native_new(0, function);
}
cr_fun * cr_macro_native_new(cr_native_function function){
  return _cr_fun_native_new(1, function);
}
