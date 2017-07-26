#ifndef __crisp_list_h__
#define __crisp_list_h__

#include <crisp/list.h>
#include <stdint.h>
#include <string.h>

typedef uint32_t (*cr_hash)(void*);
typedef int (*cr_cmp)(void*,void*);
typedef void * (*cr_update)(void*);

uint32_t cr_int_hash(int value);
int cr_int_cmp(int v1, int v2);

uint32_t cr_str_hash(char * str);

typedef struct {
  void *key, *value;
} cr_entry;

typedef struct {
  cr_hash hash;
  cr_cmp cmp;
  int entries;
  int capacity;
  cr_list ** buckets;
} cr_map;

cr_map * cr_map_new(cr_hash hash, cr_cmp cmp);
#define cr_map_newS(hash, cmp) cr_map_new((cr_hash) hash, (cr_cmp) cmp)
#define cr_intmap_new() cr_map_new((cr_hash) cr_int_hash, (cr_cmp) cr_int_cmp)
#define cr_strmap_new()\
  cr_map_new((cr_hash) cr_str_hash, (cr_cmp) strcmp)
void cr_map_destroy(cr_map * map);

void * cr_map_set(cr_map * map, void * key, void * value);
#define cr_map_setS(map, key, value)\
  cr_map_set(map, (void *) key, (void *) value)
void * cr_map_get(cr_map * map, void * key);
#define cr_map_getS(map, key) cr_map_get(map, (void *) key)

void * cr_map_unset(cr_map * map, void * key);
#define cr_map_unsetS(map, key) cr_map_unset(map, (void *) key)
void * cr_map_update(cr_map * map, void * key, cr_update update);
#define cr_map_updateS(map, key, update)\
  cr_map_update(map, (void *) key, (cr_update) update)

#endif /* __crisp_list_h__ */
