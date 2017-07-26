#include <minunit.h>
#include <stdio.h>

#include <crisp/debug.h>
#include <crisp/gc.h>
#include <crisp/symbol.h>

int tests_run = 0;
cr_symbol * symx, *symy, *symx2;
static char * test_new(){
  symx = cr_ref(cr_symbol_new("x"));
  mu_assert("Expected symx != NULL", symx != NULL);
  mu_assert("Expected symx to have 1 refs.", cr_ref_count(symx));
  symy = cr_ref(cr_symbol_new("y"));
  mu_assert("Expected symy != NULL.", symy != NULL);
  mu_assert("Expected symy to have 1 refs.", cr_ref_count(symx));
  symx2 = cr_ref(cr_symbol_new("x"));
  mu_assert("Expected symx2 != NULL", symx2 != NULL);
  mu_assert("Expected symx2 to have 2 refs.", cr_ref_count(symx2));

  mu_assert("Expected symx == symx2", symx == symx2);
  return NULL;
}
static char * test_ref(){
  return NULL;
}
static char * test_free(){
  cr_free(symx);
  mu_assert("Expected symx to have 1 ref", cr_ref_count(symx) == 1);
  cr_free(symy);
  cr_free(symx2);
  return NULL;
}
static char * all_tests(){
  mu_run_test(test_new);
  mu_run_test(test_free);

  return NULL;
}

int main(){
  cr_debug_init_std();
  char * res = all_tests();
  cr_debug_finish();
  cr_symbol_clear_all();
  if(res){
    printf("%s\n", res);
    return -1;
  }else{
    printf("SYMBOL TESTS: ALL TESTS PASSED\n");
  }
  printf("%i tests run.\n", tests_run);
  return 0;
}
