#include <parsington/token.h>
#include <minunit.h>
#include <stdio.h>
#include <string.h>

int tests_run = 0;
pt_token * int_token, *sym_token, *lu_token, *ls_token;

static char * test_new(){
  int_token = pt_integer_token_new(1337);
  mu_assert("Expected int token to be non-null", int_token != NULL);
  mu_assert("Expected int token to have value 1337",
      ((pt_integer_token *)int_token)->value == 1337);
  mu_assert("Expected int_token to have type pt_integer_tt",
      int_token->type == pt_integer_tt);

  sym_token = pt_symbol_token_new("hello");
  mu_assert("Expected symbol token to be non-null", sym_token != NULL);
  mu_assert("Expected symbol token to have value \"hello\"",
      strcmp(((pt_symbol_token *)sym_token)->value, "hello") == 0);
  mu_assert("Expected sym_token to have type pt_symbol_tt",
      sym_token->type == pt_symbol_tt);

  lu_token = pt_lookup_token_new("foo", "bar");
  mu_assert("Expected lookup token to be non-null", lu_token != NULL);
  mu_assert("Expected lookup token to have module \"foo\"",
      strcmp(((pt_lookup_token *)lu_token)->module, "foo") == 0);
  mu_assert("Expected lookup token to have name \"bar\"",
      strcmp(((pt_lookup_token *)lu_token)->name, "bar") == 0);
  mu_assert("Expected lu_token to have type pt_lookup_tt",
      lu_token->type == pt_lookup_tt);

  ls_token = pt_list_start_token;
  mu_assert("Expected ls_token to be non-null", ls_token != NULL);
  mu_assert("Expected ls_token to have type pt_list_start_tt",
      ls_token->type == pt_list_start_tt);

  return NULL;
}
static char * test_free(){
  pt_token_free(int_token);
  pt_token_free(sym_token);
  pt_token_free(lu_token);
  pt_token_free(ls_token);

  pt_token_finalize();
  return NULL;
}

static char * all_tests(){
  mu_run_test(test_new);
  mu_run_test(test_free);

  return NULL;
}
int main(){
  pt_token_init();
  char * res = all_tests();
  if(res){
    printf("TOKEN TESTS FAILED: %s\n", res);
    return -1;
  }else{
    printf("TOKEN TESTS: ALL TESTS PASSED\n");
  }
  printf("%i tests run.\n", tests_run);
  return 0;
}
