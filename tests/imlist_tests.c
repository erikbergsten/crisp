#include <minunit.h>
#include <stdio.h>

#include <crisp/debug.h>
#include <crisp/gc.h>
#include <crisp/imlist.h>
#include <crisp/integer.h>

int tests_run = 0;

cr_imlist *l1, *l2, *l3;

static char * test_prepend(){
  l3 = cr_imlist_prependS(cr_imlist_empty, cr_integer_new(30));
  mu_assert("Expected l3 != NULL", l3 != NULL);
  mu_assert("Expected l3 to have 30 as its head",
      ((cr_integer *)cr_imlist_head(l3))->value == 30);
  mu_assert("Expected l3 to have 0 refs", cr_ref_count(l3) == 0);
  mu_assert("Expected l3 value to have 1 refs", cr_ref_count(l3->value) == 1);
  l2 = cr_imlist_prependS(l3, cr_integer_new(20));
  mu_assert("Expected l3 to have 1 refs", cr_ref_count(l3) == 1);
  l1 = cr_imlist_prependS(l2, cr_integer_new(10));
  return NULL;
}
static char * test_free(){
  cr_free(l1);
  return NULL;
}
static char * str_tests(){
  cr_imlist * word = cr_imlist_from_str("erik is tha bomb");
  cr_imlist_print_str(word);
  cr_free(word);
  printf("%i refs to empty list\n", cr_ref_count(cr_imlist_empty));
  return NULL;
}
static char * all_tests(){
  mu_run_test(test_prepend);
  mu_run_test(test_free);
  mu_run_test(str_tests);
  return NULL;
}

int main(){
  cr_debug_init_std();
  cr_imlist_init();
  char * res = all_tests();
  if(res){
    printf("%s\n", res);
    return -1;
  }else{
    printf("IMLIST TESTS: ALL TESTS PASSED\n");
  }
  printf("%i tests run.\n", tests_run);
  return 0;
}
