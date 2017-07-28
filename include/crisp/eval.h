#ifndef __crisp_eval_h__
#define __crisp_eval_h__

#include <crisp/object.h>
#include <crisp/env.h>
#include <crisp/run.h>

cr_object * cr_eval(cr_object * object, cr_runtime * runtime, cr_env * env);
#define cr_evalS(o, r, e) cr_eval((cr_object *) o, r, e)

#endif /* __crisp_eval_h__ */
