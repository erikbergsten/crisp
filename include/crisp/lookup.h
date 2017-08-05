#ifndef __crisp_lookup_h__
#define __crisp_lookup_h__

#include <crisp/object.h>
#include <crisp/symbol.h>
#include <crisp/gc.h>

typedef struct {
  cr_prototype * prototype;
  cr_symbol * module, * name;
} cr_lookup;

cr_lookup * cr_lookup_new(char * module, char * name);

#endif /* __crisp_lookup_h__ */
