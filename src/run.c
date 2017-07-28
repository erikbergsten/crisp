#include <crisp/run.h>
#include <crisp/object.h>
#include <crisp/eval.h>
#include <crisp/debug.h>
#include <crisp/env.h>
#include <crisp/core/core.h>
#include <crisp/core/math.h>
#include <crisp/core/list.h>
#include <crisp/show.h>
#include <crisp/gc.h>

void cr_module_clear(cr_map * map){
  for(int i = 0; i < map->capacity; i++){
    cr_list * bucket = map->buckets[i];
    if(bucket){
      for(cr_node * node = bucket->head; node; node = node->next){
        cr_entry * entry = (cr_entry *) node->value;
        cr_free(entry->key);
        cr_free(entry->value);
      }
    }
  }
}
void cr_runtime_finish(cr_runtime * rt){

  for(int i = 0; i < rt->modules->capacity; i++){
    cr_list * bucket = rt->modules->buckets[i];
    if(bucket){
      for(cr_node * node = bucket->head; node; node = node->next){
        cr_entry * entry = (cr_entry *) node->value;
        cr_env_free_binds((cr_env *) entry->value);
        cr_free(entry->key);
        cr_free(entry->value);
      }
    }
  }
  cr_map_destroy(rt->modules);
  cr_free(rt->core);
}

void cr_runtime_init(cr_runtime * rt){
  rt->core = cr_ref(cr_env_new(NULL));
  rt->modules = cr_map_newS(cr_symbol_hash, cr_symbol_cmp);
  cr_map_setS(rt->modules, cr_symbol_new("core"), cr_ref(rt->core));
  rt->current = rt->core;
  cr_core_register(rt->core);
  cr_core_list_register(rt->core);
  cr_core_math_register(rt->core);
}
void cr_run_list(cr_list * statements){
  cr_runtime rt;
  cr_runtime_init(&rt);
  cr_core_init();
  cr_debug_info("Initializing global environment");
  cr_symbol * def = cr_ref(cr_symbol_new("def"));
  int n = 1;
  for(cr_node * node = statements->head; node; node = node->next){
    int skip = 0;
    cr_object * obj = (cr_object *) node->value;
    cr_ref(obj);
    cr_debug_info("Evaluating statement #%i with %i refs", n++, cr_ref_count(obj));
    char buf[256];
    cr_show(buf, obj);
    cr_debug_info("Statement: %s", buf);
    cr_object * val = cr_eval(obj, &rt, rt.current);
    cr_ref(val);
    cr_show(buf, val);
    cr_debug_info("Value: %s witd %i refs", buf, cr_ref_count(val));
    cr_free(obj);
    cr_free(val);
  }
  //cr_debug_info("Clearing global binds");
  //cr_runtime_finish(&rt);
  //cr_force_free(cr_imlist_empty);
}
