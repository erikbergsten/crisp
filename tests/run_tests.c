#include <minunit.h>
#include <stdio.h>
#include <crisp/debug.h>
#include <crisp/gc.h>
#include <crisp/symbol.h>
#include <crisp/tokenizer.h>
#include <crisp/parser.h>
#include <crisp/imlist.h>
#include <crisp/run.h>

int tests_run = 0;


static char * test_run(){
  cr_debug_info("testing RUN");
  cr_list * tokens = cr_tokenize_file("../test.cr");
  cr_debug_info("parsed #%i tokens", cr_list_length(tokens));
  cr_list * statements = cr_parse(tokens);
  cr_list_destroy(tokens);
  cr_debug_info("parsed #%i statements", cr_list_length(statements));
  cr_run_list(statements);
  cr_list_destroy(statements);
  return NULL;
}
static char * all_tests(){
  mu_run_test(test_run);
  cr_debug_info("calling clearall");
  cr_symbol_clear_all();
  cr_debug_info("closing stdout");
  fclose(stdout);
  return NULL;
}

int main(){
  cr_debug_init_std();
  cr_symbol_init();
  cr_imlist_init();
  char * res = all_tests();
  if(res){
    printf("%s\n", res);
    return -1;
  }else{
    printf("RUN TESTS: ALL TESTS PASSED\n");
  }
  printf("%i tests run.\n", tests_run);
  return 0;
}
