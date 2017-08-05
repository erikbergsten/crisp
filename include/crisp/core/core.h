#ifndef __crisp_core_core_h__
#define __crisp_core_core_h__

#include <crisp/object.h>
#include <crisp/function.h>

void cr_core_init();

void cr_core_register(cr_env * env);

cr_object * cr_core_if(cr_imlist * args, cr_runtime * rt, cr_env * env);

cr_object * cr_core_fun(cr_imlist * args, cr_runtime * runtime, cr_env * env);
cr_object * cr_core_fun_macro(cr_imlist * args, cr_runtime * rt, cr_env * env);

cr_object * cr_core_print(cr_imlist * args, cr_runtime * runtime, cr_env * env);

cr_object * cr_core_def(cr_imlist * args, cr_runtime * rt, cr_env * env);
cr_object * cr_core_def_macro(cr_imlist * args, cr_runtime * rt, cr_env * env);

cr_object * cr_core_in(cr_imlist * args, cr_runtime * rt, cr_env * env);
cr_object * cr_core_module(cr_imlist * args, cr_runtime * rt, cr_env * env);

#endif /* __crisp_core_core_h__ */
