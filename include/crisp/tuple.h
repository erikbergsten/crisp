#ifndef __crisp_tuple_h__
#define __crisp_tuple_h__

#include <crisp/object.h>
#include <crisp/symbol.h>
#include <crisp/imlist.h>

typedef struct {
  cr_prototype * prototype;
  int length;
  cr_object ** values;
} cr_tuple;

cr_tuple * cr_tuple_new(int n, cr_object ** values);
cr_tuple * cr_pair_new(cr_object * obj1, cr_object * obj2);

cr_object * cr_tuple_fst(cr_tuple * tuple);
cr_object * cr_tuple_snd(cr_tuple * tuple);

cr_tuple * cr_tuple_error(char * str);

#endif /* __crisp_tuple_h__ */
