#include <minunit.h>
#include <stdio.h>
#include <crisp/debug.h>
#include <crisp/object.h>
#include <crisp/gc.h>
#include <crisp/symbol.h>
#include <crisp/show.h>
#include <crisp/imlist.h>
#include <parsington/tokenizer.h>
#include <parsington/token.h>
#include <parsington/parser.h>

int tests_run = 0;
char * list_str = "(1 2 3)";

static char * test_parser(){
  cr_object * list = pt_parser_parse_str(list_str);
  char buf[128];
  cr_show(buf, list);
  cr_debug_info("read: %s", buf);
  return NULL;
}
static char * test_stdio(){
  pt_tokenizer * tz = pt_tokenizer_read_file(stdin);
  cr_object * object = NULL;
  char buf[128];
  int i = 0;
  do{
    printf("> ");
    object = pt_parser_next(tz);
    cr_show(buf, object);
    printf("#%i => %s\n", i++, buf);
  }while(object != NULL);
  return NULL;
}
static char * all_tests(){
  mu_run_test(test_parser);
  cr_debug_info("STDIO test\n");
  mu_run_test(test_stdio);
  return NULL;
}

int main(){
  FILE * log = fopen("parser_tests.log", "a+");
  if(log == NULL){
    printf("FAILED TO OPEN LOG FILE\n");
    exit(1);
  }
  cr_debug_init(log, log, log);
  cr_imlist_init();
  cr_symbol_init();
  pt_token_init();
  char * res = all_tests();
  fclose(log);
  if(res){
    printf("ERROR: %s\n", res);
    return -1;
  }else{
    printf("PARSER TESTS: ALL TESTS PASSED\n");
  }
  printf("%i tests run.\n", tests_run);
  return 0;
}
