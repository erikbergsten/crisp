#ifndef __crisp_core_list_h__
#define __crisp_core_list_h__




void cr_core_list_register(cr_env * env);

cr_object * cr_core_list_conj(cr_imlist * args, cr_runtime * runtime, cr_env * env);
cr_object * cr_core_list_head(cr_imlist * args, cr_runtime * rt, cr_env * env);
cr_object * cr_core_list_tail(cr_imlist * args, cr_runtime * rt, cr_env * env);

#endif /* __crisp_core_list_h__ */
