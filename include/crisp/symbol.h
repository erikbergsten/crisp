#ifndef __crisp_symbol_h__
#define __crisp_symbol_h__

#include <crisp/object.h>

extern cr_prototype cr_symbol_prototype;

typedef struct {
  cr_prototype * prototype;
  char * name;
  uint32_t hash;
} cr_symbol;


#define cr_null ((cr_object *) cr_symbol_null)
void cr_symbol_clear_all(); //for gc testing purposes
cr_symbol * cr_symbol_new(char * name);

uint32_t cr_symbol_hash(cr_symbol * sym);
int cr_symbol_cmp(cr_symbol * sym1, cr_symbol * sym2);

void cr_symbol_init();

// list of symbols that are used by the language builtins
extern cr_symbol *cr_symbol_null, // "null" is used to represent... you guessed it: null
                 *cr_symbol_true,
                 *cr_symbol_false, // "true" and "false"
                 *cr_symbol_error, //returned by some function in tuples
                 *cr_symbol_rest; // used by functions to look for variadic
                                  // args, symbol: "&"


#endif /* __crisp_symbol_h__ */
