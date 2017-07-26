#ifndef __crisp_double_h__
#define __crisp_double_h__

#include <crisp/object.h>

extern cr_prototype cr_double_prototype;
typedef struct {
  cr_prototype * prototype;
  double value;
} cr_double;

cr_double * cr_double_new(double value);
int cr_double_cmp(cr_double * dub1, cr_double * dub2);


#endif /* __crisp_double_h__ */
