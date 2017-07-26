#ifndef __crisp_integer_h__
#define __crisp_integer_h__

#include <crisp/object.h>

extern cr_prototype cr_integer_prototype;

typedef struct {
  cr_prototype * prototype;
  int value;
} cr_integer;

cr_integer * cr_integer_new(int value);
uint32_t cr_integer_hash(cr_integer * integer);
int cr_integer_cmp(cr_integer * int1, cr_integer * int2);


#endif /* __crisp_integer_h__ */
