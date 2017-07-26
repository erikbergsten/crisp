#include <minunit.h>
#include <stdio.h>
#include <crisp/debug.h>
#include <crisp/list.h>

int tests_run = 0;

cr_list * list;
static char * test_create(){
  list = cr_list_new(sizeof(int));
  mu_assert("Expected list to be initiated", list != NULL);
  mu_assert("Expected initial list length to be 0", cr_list_length(list) == 0);
  mu_assert("Expected empty list head to be null", list->head == NULL);

  return NULL;
}

static char * test_append(){
  cr_list_appendS(list, 12);
  mu_assert("Expected int list length to be 1", cr_list_length(list) == 1);
  mu_assert("Expected int list to have head == end", list->head == list->end);
  cr_list_appendS(list, 23);
  mu_assert("Expected int list length to be 2", cr_list_length(list) == 2);
  mu_assert("Expected int head == 12", ((int) cr_list_head(list)) == 12);
  mu_assert("Expected int end == 23", ((int) cr_list_end(list)) == 23);
  cr_list_prependS(list, 2);
  mu_assert("Expected int list length to be 3", cr_list_length(list) == 3);
  mu_assert("Expected int head == 2", ((int) cr_list_head(list)) == 2);
  mu_assert("Expected int end == 23", ((int) cr_list_end(list)) == 23);
  return NULL;
}
static char * test_pop(){
  int x = (int) cr_list_pop(list);
  mu_assert("Expected popped value to be 23", x == 23);
  mu_assert("Expected int list length to be 2 after pop", cr_list_length(list) == 2);
  mu_assert("Expected int end == 12 after pop", ((int) cr_list_end(list)) == 12);
  x = (int) cr_list_shift(list);
  mu_assert("Expected shifted value to be 2", x == 2);
  mu_assert("Expected int list length to be 1 after shift", cr_list_length(list) == 1);

  x = (int) cr_list_shift(list);
  mu_assert("Expected shifted value to be 12", x == 12);
  mu_assert("Expected int list length to be 0 after shift", cr_list_length(list) == 0);

  void * null = cr_list_pop(list);
  mu_assert("Expected popped value to be NULL", null == NULL);

  return NULL;
}

static char * test_destroy(){
  cr_list_destroy(list);
  return NULL;
}

static char * all_tests(){
  mu_run_test(test_create);
  mu_run_test(test_append);
  mu_run_test(test_pop);
  mu_run_test(test_destroy);
  return NULL;
}

int main(){
  cr_debug_init_std();
  char * res = all_tests();
  if(res){
    printf("FAILED: %s\n", res);
    printf("%i tests run.\n", tests_run);
    return -1;
  }else{
    printf("LIST TESTS: ALL TESTS PASSED\n");
    printf("%i tests run.\n", tests_run);
    return 0;
  }
}
