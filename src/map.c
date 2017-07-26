#include <crisp/map.h>
#include <crisp/debug.h>
#include <stdlib.h>


uint32_t cr_int_hash(int value){
  uint32_t hash = (uint32_t) value;
  hash = ((hash >> 16) ^ hash) * 0x45d9f3b;
  hash = ((hash >> 16) ^ hash) * 0x45d9f3b;
  hash = (hash >> 16) ^ hash;
  return hash;

}
int cr_int_cmp(int v1, int v2){
  return v1 - v2;
}

uint32_t cr_str_hash(char * str){
  uint32_t hash = 5381;
  int c;
  while ((c = *str++))
    hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
  
  return hash;
}
cr_map * cr_map_new(cr_hash hash, cr_cmp cmp){
  cr_map * map = (cr_map *) malloc(sizeof(cr_map));
  map->hash = hash;
  map->cmp = cmp;
  map->capacity = 8;
  map->entries = 0;
  map->buckets = calloc(map->capacity, sizeof(cr_list *));
  return map;
}
void cr_map_destroy(cr_map * map){
  for(int i = 0; i < map->capacity; i++){
    cr_list * bucket = map->buckets[i];
    if(bucket){
      for(cr_node * node = bucket->head; node; node = node->next){
        free(node->value);
      }
      cr_list_destroy(bucket);
    }
  }
  free(map->buckets);
  free(map);
}
cr_entry * _entry_new(void * key, void * value){
  cr_entry * entry = (cr_entry *) malloc(sizeof(cr_entry));
  entry->key = key;
  entry->value = value;
  return entry;
}

#define _mod(h, c) (h & (c - 1))
#define _over_capacity(map) (map->entries > map->capacity * 0.75f)

void _grow(cr_map * map){
  int new_capacity = map->capacity * 2;
  cr_list ** new_buckets = calloc(new_capacity, sizeof(cr_list *));
  for(int i = 0; i < map->capacity; i++){
    cr_list * bucket = map->buckets[i];
    if(bucket){
      for(cr_node * node = bucket->head; node; node = node->next){
        cr_entry * entry = (cr_entry *) node->value;
        uint32_t hash = map->hash(entry->key);
        int index = _mod(hash, new_capacity);
        if(new_buckets[index] == NULL){
          new_buckets[index] = cr_list_newP();
        }
        cr_list_append(new_buckets[index], entry);
      }
      cr_list_destroy(bucket);
    }
  }
  free(map->buckets);
  map->buckets = new_buckets;
  map->capacity = new_capacity;
}
void * cr_map_set(cr_map * map, void * key, void * value){
  uint32_t hash = map->hash(key);
  int index = _mod(hash, map->capacity);
  cr_list * bucket = map->buckets[index];
  if(bucket == NULL){
    bucket = cr_list_newP();
    cr_list_appendS(bucket, _entry_new(key, value));
    map->buckets[index] = bucket;
  }else{
    for(cr_node * node = bucket->head; node; node = node->next){
      cr_entry * entry = (cr_entry *) node->value;
      if(map->cmp(key, entry->key) == 0){
        //already in list
        void * old = entry->value;
        entry->value = value;
        return old;
      }
    }
    //key wasnt in bucket
    cr_list_appendS(bucket, _entry_new(key, value));
  }
  //if we got this far we added a new element somehow
  map->entries++;
  if(_over_capacity(map)){
    _grow(map);
  }
  return NULL;
}
void * cr_map_get(cr_map * map, void * key){
  uint32_t hash = map->hash(key);
  int index = _mod(hash, map->capacity);
  cr_list * bucket = map->buckets[index];
  if(bucket == NULL){
    return NULL;
  }else{
    for(cr_node * node = bucket->head; node; node = node->next){
      cr_entry * entry = (cr_entry *) node->value;
      if(map->cmp(key, entry->key) == 0){
        return entry->value;
      }
    }
    return NULL;
  }

}
void * cr_map_unset(cr_map * map, void * key){
  uint32_t hash = map->hash(key);
  int index = _mod(hash, map->capacity);
  cr_list * bucket = map->buckets[index];
  if(bucket == NULL){
    cr_debug_warn("Unsetting already unset key");
    return NULL;
  }else{
    for(cr_node * node = bucket->head; node; node = node->next){
      cr_entry * entry = (cr_entry *) node->value;
      if(map->cmp(key, entry->key) == 0){
        void * value = entry->value;
        cr_list_remove(bucket, node);
        free(entry);
        map->entries--;
        return value;
      }
    }
    return NULL;
  }
}
void * cr_map_update(cr_map * map, void * key, cr_update update){
  uint32_t hash = map->hash(key);
  int index = _mod(hash, map->capacity);
  cr_list * bucket = map->buckets[index];
  if(bucket == NULL){
    return NULL;
  }else{
    for(cr_node * node = bucket->head; node; node = node->next){
      cr_entry * entry = (cr_entry *) node->value;
      if(map->cmp(key, entry->key) == 0){
        void * old = entry->value;
        entry->value = update(old);
        return old;
      }
    }
    return NULL;
  }
}
