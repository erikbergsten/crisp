#ifndef __crisp_core_math_h__
#define __crisp_core_math_h__

#include <crisp/env.h>
#include <crisp/imlist.h>
#include <crisp/run.h>
void cr_core_math_register(cr_env * env);

cr_object * cr_core_math_add(cr_imlist * args, cr_runtime * rt, cr_env * env);
cr_object * cr_core_math_mul(cr_imlist * args, cr_runtime * rt, cr_env * env);
cr_object * cr_core_math_div(cr_imlist * args, cr_runtime * rt, cr_env * env);
cr_object * cr_core_math_sub(cr_imlist * args, cr_runtime * rt, cr_env * env);


#endif /* __crisp_core_math_h__ */
