#include <minunit.h>
#include <stdio.h>
#include <crisp/debug.h>
#include <crisp/gc.h>
#include <crisp/tokenizer.h>

int tests_run = 0;
static char * test_tokenize(){
  cr_list * tokens = cr_tokenize_file("../test.cr");
  printf("found %i tokens!\n", cr_list_length(tokens));
  return NULL;
}

static char * all_tests(){
  mu_run_test(test_tokenize);

  return NULL;
}

int main(){
  cr_debug_init_std();
  char * res = all_tests();
  if(res){
    printf("%s\n", res);
    return -1;
  }else{
    printf("TOKENIZER TESTS: ALL TESTS PASSED\n");
  }
  printf("%i tests run.\n", tests_run);
  return 0;
}
