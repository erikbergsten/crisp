#include <minunit.h>
#include <stdio.h>

#include <crisp/debug.h>
#include <crisp/gc.h>

int tests_run = 0;

char * myString;

void funny_destructor(char * str){
  printf("freeing string: %s\n", str);
}

static char * test_malloc(){
  myString = cr_mallocS(sizeof(char) * 5, NULL);
  strcpy(myString, "erik");
  mu_assert("Expected myString to be non null.", myString != NULL);
  mu_assert("Expected myString to have 0 refs.", cr_ref_count(myString));
  return NULL;
}
static char * test_ref(){
  cr_ref(myString);
  mu_assert("Expected myString to have 1 refs.", cr_ref_count(myString));
  cr_ref(myString);
  mu_assert("Expected myString to have 2 refs.", cr_ref_count(myString));

  return NULL;
}
static char * test_free(){
  cr_free(myString);
  mu_assert("Expected myString to have 1 refs.", cr_ref_count(myString));
  cr_free(myString);
}
static char * all_tests(){
  mu_run_test(test_malloc);
  mu_run_test(test_ref);
  mu_run_test(test_free);

  return NULL;
}

int main(){
  cr_debug_init_std();
  char * res = all_tests();
  if(res){
    printf("%s\n", res);
    return -1;
  }else{
    printf("<something> TESTS: ALL TESTS PASSED\n");
  }
  printf("%i tests run.\n", tests_run);
  return 0;
}
