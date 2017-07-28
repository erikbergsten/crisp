#ifndef __crisp_symbol_h__
#define __crisp_symbol_h__

#include <crisp/object.h>

extern cr_prototype cr_symbol_prototype;

typedef struct {
  cr_prototype * prototype;
  char * name;
  uint32_t hash;
} cr_symbol;

extern cr_symbol * cr_symbol_null;
#define cr_null ((cr_object *) cr_symbol_null)
void cr_symbol_init();
void cr_symbol_clear_all(); //for gc testing purposes
cr_symbol * cr_symbol_new(char * name);

uint32_t cr_symbol_hash(cr_symbol * sym);
int cr_symbol_cmp(cr_symbol * sym1, cr_symbol * sym2);



#endif /* __crisp_symbol_h__ */
