#include <minunit.h>
#include <stdio.h>
#include <crisp/map.h>
#include <crisp/debug.h>

int tests_run = 0;
cr_map * map;

static char * test_new(){
  map = cr_strmap_new();
  mu_assert("Expected map to be non-null", map != NULL);
  mu_assert("Expected map to have 0 entries", map->entries == 0);
  mu_assert("Expected key: \"smurf\" to be unbound",
      cr_map_getS(map, "smurf") == NULL);
  return NULL;
}
static char * test_set(){
  cr_map_setS(map, "erik", 12);
  mu_assert("Expected map to have 1 entries", map->entries == 1);
  mu_assert("Expected map to have 8 capacity", map->capacity == 8);
  cr_map_setS(map, "hugo", 1);
  mu_assert("Expected map to have 2 entries", map->entries == 2);
  mu_assert("Expected map to have 8 capacity after 2 inserts", map->capacity == 8);
  cr_map_setS(map, "smurf", 42);
  cr_map_setS(map, "lEET", 1337);
  cr_map_setS(map, "roffe", "ruff");
  cr_map_setS(map, "age", 123);
  mu_assert("Expected map to have 6 entries", map->entries == 6);
  mu_assert("Expected map to have 8 capacity after 6 inserts", map->capacity == 8);
  cr_map_setS(map, "aoeu", "best");
  mu_assert("Expected map to have 7 entries", map->entries == 7);
  mu_assert("Expected map to have 16 capacity after 7 inserts", map->capacity == 16);

  return NULL;
}
static char * test_get(){
  mu_assert("Expected erik to be 12", ((int) cr_map_getS(map, "erik")) == 12);
  mu_assert("Expected roffe to be ruff",
      strcmp((char *) cr_map_getS(map, "roffe"), "ruff") == 0);

  return NULL;
}

static char * test_unset(){
  cr_map_unsetS(map, "erik");
  mu_assert("Expected map to have 6 entries after unset", map->entries == 6);
  mu_assert("Expected erik to be NULL after unset", cr_map_get(map, "erik") == NULL);
  return NULL;
}
int _double(int v){
  return 2 * v;
}
static char * test_update(){
  mu_assert("Expected hugo to be 1 before update",
      ((int) cr_map_get(map, "hugo")) == 1);
  cr_map_updateS(map, "hugo", _double);
  mu_assert("Expected hugo to be 2 after 1 update",
      ((int) cr_map_get(map, "hugo")) == 2);
  cr_map_updateS(map, "hugo", _double);
  mu_assert("Expected hugo to be 4 after 2 updates",
      ((int) cr_map_get(map, "hugo")) == 4);

  return NULL;
}
static char * test_destroy(){
  cr_map_destroy(map);
  return NULL;
}
static char * all_tests(){
  mu_run_test(test_new);
  mu_run_test(test_set);
  mu_run_test(test_get);
  mu_run_test(test_unset);
  mu_run_test(test_update);
  mu_run_test(test_destroy);
  return NULL;
}
int main(){
  cr_debug_init_std();
  char * res = all_tests();
  cr_debug_finish();
  if(res){
    printf("%s\n", res);
    return -1;
  }else{
    printf("MAP TESTS: ALL TESTS PASSED\n");
  }
  printf("%i tests run.\n", tests_run);
  return 0;
}
