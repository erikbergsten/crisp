#include <crisp/symbol.h>
#include <crisp/map.h>
#include <crisp/gc.h>
#include <stdlib.h>
#include <crisp/debug.h>

cr_prototype cr_symbol_prototype = {
  cr_symbol_type,
  "symbol",
  (cr_hash) cr_symbol_hash,
  (cr_cmp) cr_symbol_cmp
};
cr_map * cr_symbols = NULL;
cr_symbol * cr_symbol_null = NULL;
void cr_symbol_destroy(cr_symbol * sym){
  cr_debug_info("Freeing and unsetting symbol: %s", sym->name);
  cr_map_unset(cr_symbols, sym->name);
  free(sym->name);
}
void cr_symbol_init(){
  cr_symbols = cr_strmap_new();
  cr_symbol_null = cr_ref(cr_symbol_new("null"));
}
cr_symbol * cr_symbol_new(char * name){
  cr_symbol * sym = cr_map_get(cr_symbols, name);
  if(sym == NULL){
    sym = (cr_symbol *) cr_mallocS(sizeof(cr_symbol), cr_symbol_destroy);
    sym->prototype = &cr_symbol_prototype;
    sym->name = (char *) malloc(sizeof(char) * (strlen(name) + 1));
    strcpy(sym->name, name);
    sym->hash = cr_str_hash(name);
    cr_map_set(cr_symbols, sym->name, sym);
  }
  return sym;
}
uint32_t cr_symbol_hash(cr_symbol * sym){
  return sym->hash;
}
int cr_symbol_cmp(cr_symbol * sym1, cr_symbol * sym2){
  return sym1 != sym2;
}
void cr_symbol_clear_all(){
  cr_debug_info("Clearing all #%i symbols.",3);
  for(int i = 0; i < cr_symbols->capacity; i++){
    cr_list * bucket = cr_symbols->buckets[i];
    if(bucket){
      for(cr_node * node = bucket->head; node; node = node->next){
        cr_entry * entry = (cr_entry *) node->value;
        cr_symbol * symbol = (cr_symbol *) entry->value;
        free(symbol->name);
        free(cr_ref_meta_addr(symbol));
      }
    }
  }
  cr_map_destroy(cr_symbols);
}
