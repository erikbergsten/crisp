#ifndef __crisp_function_h__
#define __crisp_function_h__
#include <crisp/object.h>
#include <crisp/imlist.h>
#include <crisp/env.h>
#include <crisp/run.h>

typedef cr_object * (*cr_native_function)(cr_list * args, cr_runtime * runtime, cr_env * env);
typedef enum {
  cr_function_native,
  cr_function_normal
} cr_function_form;

typedef struct {
  cr_prototype * prototype;
  cr_function_form form;
} cr_fun;

typedef struct {
  cr_prototype * prototype;
  cr_function_form form;
  int macro;
  cr_native_function function;
} cr_fun_native;

typedef struct {
  cr_prototype * prototype;
  cr_function_form form;
  int macro;
  int overloaded;
  cr_env * env;
  cr_imlist * args;
  cr_imlist * body;
} cr_fun_normal;

cr_fun * cr_fun_new(cr_env * env, cr_imlist * args, cr_imlist * body);
#define cr_fun_newS(e, a, b) cr_fun_new(e, (cr_imlist *) a, (cr_imlist *) b)
cr_fun * cr_fun_native_new(cr_native_function function);

cr_fun * cr_macro_new(cr_env * env, cr_imlist * args, cr_imlist * body);
#define cr_macro_newS(e, a, b) cr_macro_new(e, (cr_imlist *) a, (cr_imlist *) b)
cr_fun * cr_macro_native_new(cr_native_function function);


#endif /* __crisp_function_h__ */
