#include <crisp/lookup.h>

uint32_t cr_lookup_hash(cr_lookup * lu){
  return cr_symbol_hash(lu->module) * 31 + cr_symbol_hash(lu->name);
}
int cr_lookup_cmp(cr_lookup * lu1, cr_lookup * lu2){
  return (lu1->module != lu2->module) || (lu1->name != lu2->name);
}

void cr_lookup_destroy(cr_lookup * lu){
  cr_free(lu->name);
  cr_free(lu->module);
}

cr_prototype cr_lookup_prototype = {
  cr_lookup_type,
  "lookup",
  (cr_hash) cr_lookup_hash,
  (cr_cmp) cr_lookup_cmp
};
cr_lookup * cr_lookup_new(char * module, char * name){
  cr_lookup * lu = (cr_lookup *) cr_mallocS(sizeof(cr_lookup), cr_lookup_destroy);
  lu->prototype = &cr_lookup_prototype;
  cr_symbol * modsym = cr_symbol_new(module);
  cr_symbol * namesym = cr_symbol_new(name);
  lu->name = cr_ref(namesym);
  lu->module = cr_ref(modsym);
  return lu;
}
