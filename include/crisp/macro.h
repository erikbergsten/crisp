#ifndef __crisp_macro_h__
#define __crisp_macro_h__

#include <crisp/integer.h>
#include <crisp/show.h>
#include <crisp/env.h>
#include <crisp/quote.h>
#include <crisp/imlist.h>
#include <crisp/function.h>
#include <crisp/debug.h>
typedef struct {
  cr_prototype * prototype;
  cr_imlist * args;
  cr_imlist * body;
  cr_env * env;
} cr_macro;

cr_macro * cr_macro_new(cr_env * env, cr_imlist * args, cr_imlist * body);

#endif /* __crisp_macro_h__ */
