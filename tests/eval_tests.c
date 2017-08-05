#include <minunit.h>
#include <parsington/parser.h>
#include <stdio.h>
#include <crisp/debug.h>
#include <crisp/gc.h>
#include <crisp/eval.h>
#include <crisp/integer.h>
#include <crisp/double.h>
#include <crisp/function.h>
#include <crisp/symbol.h>
#include <crisp/show.h>
#include <crisp/imlist.h>

int tests_run = 0;

static char * test_list(){
  cr_runtime rt;
  cr_runtime_init(&rt);
  cr_object * int1 =  cr_ref(pt_parser_parse_str("12"));
  char buf1[128], buf2[128];
  cr_object * int1e = cr_eval(int1, &rt, rt.core);
  cr_show(buf1, int1);
  cr_show(buf2, int1e);
  cr_debug_info("before %s", buf1);
  cr_debug_info("after %s", buf2);
  cr_free(int1);

  cr_object * list1 = cr_ref(pt_parser_parse_str("(core/+ 1 2)"));
  cr_object * list1e = cr_eval(list1, &rt, rt.core);
  cr_show(buf1, list1);
  cr_show(buf2, list1e);
  cr_debug_info("before %s", buf1);
  cr_debug_info("after %s", buf2);

  return NULL;
}

static char * all_tests(){
  mu_run_test(test_list);
  return NULL;
}

int main(){
  cr_imlist_init();
  cr_symbol_init();
  pt_token_init();
  cr_debug_init_std();
  cr_force_free(cr_imlist_empty);
  char * res = all_tests();
  cr_symbol_clear_all();
  pt_token_finalize();
  cr_debug_finish();
  if(res){
    printf("%s\n", res);
    return -1;
  }else{
    printf("EVAL TESTS: ALL TESTS PASSED\n");
  }
  printf("%i tests run.\n", tests_run);
  return 0;
}
