#include <crisp/env.h>
#include <crisp/gc.h>
#include <crisp/list.h>

void cr_env_free_binds(cr_env * env){
  cr_map * map = env->binds;
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
  cr_map_destroy(map);
  env->binds = NULL;
}

void cr_env_destroy(cr_env * env){
  if(env->binds){
    cr_env_free_binds(env);
  }
  if(env->parent) cr_free(env->parent);
}

cr_env * cr_env_new(cr_env * parent){
  cr_env * env = cr_mallocS(sizeof(cr_env), cr_env_destroy);
  env->parent = parent;
  if(parent != NULL){
    cr_ref(parent);
  }
  env->binds = cr_map_newS(cr_symbol_hash, cr_symbol_cmp);
  return env;
}
void cr_env_set(cr_env * env, cr_symbol * symbol, cr_object * value){
  cr_map_set(env->binds, cr_ref(symbol), cr_ref(value));
}
cr_object * cr_env_get(cr_env * env, cr_symbol * symbol){
  cr_object * obj = NULL;
  while(obj == NULL && env != NULL){
    obj = cr_map_get(env->binds, symbol);
    env = env->parent;
  }
  if(obj != NULL){
    return obj;
  }else{
    return (cr_object *) cr_symbol_null;
  }
}
