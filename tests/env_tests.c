#include <minunit.h>
#include <stdio.h>

#include <crisp/debug.h>
#include <crisp/gc.h>
#include <crisp/env.h>
#include <crisp/symbol.h>
#include <crisp/integer.h>

int tests_run = 0;

cr_env * global, *myenv;
cr_symbol *x, *y;
cr_integer *int1, *int2;

static char * test_new(){
  global = cr_env_new(NULL);
  myenv = cr_ref(cr_env_new(global));
  x = cr_ref(cr_symbol_new("x"));
  y = cr_ref(cr_symbol_new("y"));
  int1 = cr_ref(cr_integer_new(5));
  int2 = cr_ref(cr_integer_new(10));
  return NULL;
}
static char * test_set(){
  cr_env_setS(global, x, int1);
  cr_env_setS(global, y, int2);
  cr_env_setS(myenv, x, int2);
  return NULL;
}
static char * test_get(){
  mu_assert("Expected global x = 5",
      ((cr_integer *) cr_env_get(global, x))->value == 5);
  mu_assert("Expected global y = 10",
      ((cr_integer *) cr_env_get(global, y))->value == 10);
  mu_assert("Expected local x = 10",
      ((cr_integer *) cr_env_get(myenv, x))->value == 10);
  mu_assert("Expected local y = 10",
      ((cr_integer *) cr_env_get(myenv, y))->value == 10);

  return NULL;
}
static char * test_free(){
  mu_assert("X should have 3 ref only", cr_ref_count(x) == 3);
  cr_free(myenv);
  mu_assert("X should have 1 ref only", cr_ref_count(x) == 1);
  cr_free(x);
  cr_free(y);

  cr_free(int1);
  cr_free(int2);

  return NULL;
}
static char * all_tests(){
  mu_run_test(test_new);
  mu_run_test(test_set);
  mu_run_test(test_get);
  mu_run_test(test_free);

  return NULL;
}

int main(){
  cr_debug_init_std();
  char * res = all_tests();
  cr_symbol_clear_all();
  if(res){
    printf("%s\n", res);
    return -1;
  }else{
    printf("ENV TESTS: ALL TESTS PASSED\n");
  }
  printf("%i tests run.\n", tests_run);
  return 0;
}
