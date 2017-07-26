#include <minunit.h>
#include <stdio.h>
#include <crisp/debug.h>
#include <crisp/object.h>
#include <crisp/gc.h>
#include <crisp/show.h>
#include <crisp/tokenizer.h>
#include <crisp/imlist.h>
#include <crisp/parser.h>
int tests_run = 0;

static char * test_parser(){
  cr_list * tokens = cr_tokenize_file("../test.cr");
  cr_list * stms = cr_parse(tokens);
  cr_debug_info("got %i statements", cr_list_length(stms));
  char buf[128];
  int i = 0;
  for(cr_node * node = stms->head; node; node = node->next){
    cr_object * obj = (cr_object *) node->value;
    cr_show(buf, obj);
    cr_debug_info("Statement #%i of type [%s]: %s", ++i, obj->prototype->name,  buf);
  }

  return NULL;
}
static char * all_tests(){
  mu_run_test(test_parser);
  return NULL;
}

int main(){
  cr_imlist_init();
  cr_debug_init_std();
  char * res = all_tests();
  if(res){
    printf("%s\n", res);
    return -1;
  }else{
    printf("PARSER TESTS: ALL TESTS PASSED\n");
  }
  printf("%i tests run.\n", tests_run);
  return 0;
}
