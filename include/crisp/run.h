#ifndef __crisp_run_h__
#define __crisp_run_h__

#include <crisp/list.h>
#include <crisp/map.h>
#include <crisp/env.h>

typedef struct {
  cr_map * modules;
  cr_env * core;
  cr_env * current;
} cr_runtime;

void cr_runtime_init(cr_runtime * rt);
void cr_runtime_finish(cr_runtime * rt);

void cr_run_list(cr_list * statements);

#endif /* __crisp_run_h__ */
