#ifndef __crisp_core_h__
#define __crisp_core_h__

#include <crisp/object.h>
#include <crisp/imlist.h>
#include <crisp/env.h>

// creates a function!
cr_object * cr_core_fun(cr_imlist * args, cr_env * env);

// converts a value to an imlist (string)
//cr_object * cr_core_show(cr_imlist * args, cr_env * env);

// converts a value to an imlist and prints it
cr_object * cr_core_print(cr_imlist * args, cr_env * env);

// prints an imlist that is a string
//cr_object * cr_core_print_str(cr_imlist * args, cr_env * env);


void cr_core_init(cr_env * env);


//

#endif /* __crisp_core_h__ */
