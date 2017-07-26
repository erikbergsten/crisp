#ifndef __crisp_eval_h__
#define __crisp_eval_h__

#include <crisp/object.h>
#include <crisp/env.h>


cr_object * cr_eval(cr_object * object, cr_env * env);
#define cr_evalS(o, e) cr_eval((cr_object *) o, e)

#endif /* __crisp_eval_h__ */
