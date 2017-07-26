#include <minunit.h>
#include <stdio.h>

#include <crisp/debug.h>
#include <crisp/gc.h>
#include <crisp/eval.h>
#include <crisp/integer.h>
#include <crisp/double.h>
#include <crisp/function.h>
#include <crisp/symbol.h>
#include <crisp/imlist.h>

cr_object * _add_fun(cr_imlist * list, cr_env * env){
  int dub = 0;
  double value = 0;
  while(list != cr_imlist_empty){
    cr_object * obj = list->value;
    if(obj->prototype->type == cr_integer_type){
      cr_integer * i = (cr_integer *) obj;
      value += i->value;
    }else if(obj->prototype->type == cr_double_type){
      dub = 1;
      cr_double * d = (cr_double *) obj;
      value = d->value;
    }else{
      return (cr_object *) cr_symbol_null;
    }
    list = list->next;
  }
  if(dub){
    return (cr_object *) cr_double_new(value);
  }else{
    return (cr_object *) cr_integer_new((int) value);
  }
}
int tests_run = 0;
static char * test_eval(){
  cr_env * global = cr_ref(cr_env_new(NULL));

  cr_integer * int1 = cr_integer_new(10);
  cr_integer * int2 = cr_integer_new(5);
  cr_double * dub1 = cr_double_new(13.37);

  cr_fun * add = cr_ref(cr_fun_native_new(_add_fun));

  cr_imlist * args1 = cr_imlist_prependS(cr_imlist_empty, int2);
  cr_imlist * args = cr_imlist_prependS(args1, dub1);
  cr_imlist * call = cr_imlist_prependS(args, add);

  cr_object * res = cr_evalS(call, global);
  if(res == NULL){
    cr_debug_info("got null?");
  }else if(res->prototype == NULL){
  }else if(res->prototype->type == cr_integer_type){
    cr_integer * i = (cr_integer *) res;
    cr_debug_info("addition got: %i", i->value);
  }else if(res->prototype->type == cr_double_type){
    cr_double * d = (cr_double *) res;
    cr_debug_info("addition got: %lf", d->value);
  }


  return NULL;
}
static char * all_tests(){
  mu_run_test(test_eval);

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
    printf("EVAL TESTS: ALL TESTS PASSED\n");
  }
  printf("%i tests run.\n", tests_run);
  return 0;
}
