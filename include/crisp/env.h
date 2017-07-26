#ifndef __crisp_env_h__
#define __crisp_env_h__
#include <crisp/map.h>
#include <crisp/object.h>
#include <crisp/symbol.h>
typedef struct _cr_env {
  struct _cr_env * parent;
  cr_map * binds;
} cr_env;

cr_env * cr_env_new(cr_env * parent);
void cr_env_set(cr_env * env, cr_symbol * symbol, cr_object * value);
#define cr_env_setS(e, s, v) cr_env_set(e, s, (cr_object *) v)
cr_object * cr_env_get(cr_env * env, cr_symbol * symbol);
void cr_env_free_binds(cr_env * env);


#endif /* __crisp_env_h__ */
