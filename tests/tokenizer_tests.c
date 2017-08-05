#include <parsington/token.h>
#include <parsington/tokenizer.h>
#include <minunit.h>
#include <stdio.h>
#include <string.h>
#include <crisp/debug.h>


int tests_run = 0;

char * int_str = "12";
char * float_str = "12.24   ";
char * symbol_str = "     foo ";
char * string_str = "\"foo\" ";
char * tuple_str = "     {     error \"foo bar baz\"} ";
char * lookup_str = "  foo/bar  ";

FILE * _as_file(char * str){
  FILE * f = tmpfile();
  fprintf(f, "%s", str);
  rewind(f);
  return f;
}

static char * _test_integer(pt_tokenizer * tz, size_t value){
  pt_token * int_tok = pt_tokenizer_next(tz);
  mu_assert("Expected int_token to be non null",
      int_tok != NULL);
  mu_assert("Expected int_tok to have type pt_integer_tt",
      int_tok->type == pt_integer_tt);
  mu_assert("Expected int_tok to have correct value",
      ((pt_integer_token *) int_tok)->value == value);
  return NULL;
}
static char * test_lookup(){
  pt_tokenizer * tz = pt_tokenizer_read_string(lookup_str);
  pt_token * lu_token = pt_tokenizer_next(tz);
  mu_assert("Expecten lu_token to be non-null", lu_token != NULL);
  mu_assert("Expected lu_token to have type pt_lookup_tt",
      lu_token->type == pt_lookup_tt);
  pt_lookup_token * lu_tok = (pt_lookup_token *) lu_token;
  mu_assert("Expected lu_tok module to be foo",
   strcmp(lu_tok->module, "foo") == 0);
  mu_assert("Expected lu_tok name to be bar",
      strcmp(lu_tok->name, "bar") == 0);
  pt_token * null_tok = pt_tokenizer_next(tz);
  mu_assert("Expected null_tok to be null", null_tok == NULL);
  mu_assert("Expected tokenizer to be finished",
      pt_tokenizer_finished(tz));
  pt_tokenizer_free(tz);
  return NULL;
}
static char * test_int(){
  pt_tokenizer * tz = pt_tokenizer_read_string(int_str);
  const char * subtest = NULL;
  subtest = _test_integer(tz, 12);
  if(subtest) return subtest;
  pt_token * null_tok = pt_tokenizer_next(tz);
  mu_assert("Expected null_tok to be null", null_tok == NULL);
  mu_assert("Expected tokenizer to be finished",
      pt_tokenizer_finished(tz));
  pt_tokenizer_free(tz);
  return NULL;
}
static char * test_float(){
  pt_tokenizer * tz = pt_tokenizer_read_string(float_str);
  pt_token * float_tok = pt_tokenizer_next(tz);
  mu_assert("Expected float_token to be non null",
      float_tok != NULL);
  mu_assert("Expected float_tok to have type pt_float_tt",
      float_tok->type == pt_float_tt);
  mu_assert("Expected float_tok to have value 12.24",
      ((pt_float_token *) float_tok)->value == 12.24);
  pt_token * null_tok = pt_tokenizer_next(tz);
  mu_assert("Expected null_tok to be null", null_tok == NULL);
  mu_assert("Expected tokenizer to be finished",
      pt_tokenizer_finished(tz));
  pt_tokenizer_free(tz);
  return NULL;
}
static char * _test_symbol(pt_tokenizer * tz, char * str){
  pt_token * sym_tok = pt_tokenizer_next(tz);
  mu_assert("Expected sym_token to be non null",
      sym_tok != NULL);
  mu_assert("Expected sym_tok to have type pt_symbol_tt",
      sym_tok->type == pt_symbol_tt);
  mu_assert("Symbol token has unexpected value",
      strcmp(((pt_symbol_token *) sym_tok)->value, str) == 0);
  return NULL;
}
static char * test_symbol(){
  pt_tokenizer * tz = pt_tokenizer_read_string(symbol_str);
  char * subtest = NULL;
  subtest = _test_symbol(tz, "foo");
  if(subtest) return subtest;
  pt_token * null_tok = pt_tokenizer_next(tz);
  mu_assert("Expected null_tok to be null", null_tok == NULL);
  mu_assert("Expected tokenizer to be finished",
      pt_tokenizer_finished(tz));
  pt_tokenizer_free(tz);
  return NULL;
}
static char * _test_string(pt_tokenizer * tz, char * str){
  pt_token * string_tok = pt_tokenizer_next(tz);
  mu_assert("Expected string_token to be non null",
      string_tok != NULL);
  mu_assert("Expected string_tok to have type pt_string_tt",
      string_tok->type == pt_string_tt);
  mu_assert("Expected string_tok value is wrong!",
      strcmp(((pt_string_token *) string_tok)->value, str) == 0);
  return NULL;
}
static char * test_string(){
  pt_tokenizer * tz = pt_tokenizer_read_string(string_str);
  char * subtest = NULL;
  subtest = _test_string(tz, "foo");
  if(subtest) return subtest;
  pt_token * null_tok = pt_tokenizer_next(tz);
  mu_assert("Expected null_tok to be null", null_tok == NULL);
  mu_assert("Expected tokenizer to be finished",
      pt_tokenizer_finished(tz));
  pt_tokenizer_free(tz);
  return NULL;
}
static char * _test_constant(pt_tokenizer * tz, pt_token * constant){
  pt_token * token = pt_tokenizer_next(tz);
  mu_assert("Expected constant to be non null",
      token != NULL);
  mu_assert("Expected constant to have constant type",
      token->type == constant->type);
  mu_assert("Expected constant to be constant",
      token == constant);
  return NULL;
}
static char * test_tuple(){
  pt_tokenizer * tz = pt_tokenizer_read_string(tuple_str);
  char * subtest = NULL;
  subtest = _test_constant(tz, pt_tuple_start_token);
  if(subtest) return subtest;
  subtest = _test_symbol(tz, "error");
  if(subtest) return subtest;
  subtest = _test_string(tz, "foo bar baz");
  if(subtest) return subtest;
  subtest = _test_constant(tz, pt_tuple_end_token);
  if(subtest) return subtest;
  pt_token * null_tok = pt_tokenizer_next(tz);
  mu_assert("Expected null_tok to be null", null_tok == NULL);
  mu_assert("Expected tokenizer to be finished",
      pt_tokenizer_finished(tz));
  pt_tokenizer_free(tz);
  return NULL;
}
char * quote_str = "'('foo '     bar) ";
static char * test_quote(){
  pt_tokenizer * tz = pt_tokenizer_read_string(quote_str);
  char * subtest = NULL;
  subtest = _test_constant(tz, pt_quote_token);
  if(subtest) return subtest;
  subtest = _test_constant(tz, pt_list_start_token);
  if(subtest) return subtest;
  subtest = _test_constant(tz, pt_quote_token);
  if(subtest) return subtest;
  subtest = _test_symbol(tz, "foo");
  if(subtest) return subtest;
  subtest = _test_constant(tz, pt_quote_token);
  if(subtest) return subtest;
  subtest = _test_symbol(tz, "bar");
  if(subtest) return subtest;
  subtest = _test_constant(tz, pt_list_end_token);
  if(subtest) return subtest;
  pt_token * null_tok = pt_tokenizer_next(tz);
  mu_assert("Expected null_tok to be null", null_tok == NULL);
  mu_assert("Expected tokenizer to be finished",
      pt_tokenizer_finished(tz));
  pt_tokenizer_free(tz);
  return NULL;
}
char * list_str = "(1 2 \n3) ";
static char * test_list(){
  pt_tokenizer * tz = pt_tokenizer_read_string(list_str);
  char * subtest = NULL;
  subtest = _test_constant(tz, pt_list_start_token);
  if(subtest) return subtest;
  subtest = _test_integer(tz, 1);
  if(subtest) return subtest;
  subtest = _test_integer(tz, 2);
  if(subtest) return subtest;
  subtest = _test_integer(tz, 3);
  if(subtest) return subtest;
  subtest = _test_constant(tz, pt_list_end_token);
  if(subtest) return subtest;
  pt_token * null_tok = pt_tokenizer_next(tz);
  mu_assert("Expected null_tok to be null", null_tok == NULL);
  mu_assert("Expected tokenizer to be finished",
      pt_tokenizer_finished(tz));
  pt_tokenizer_free(tz);
  return NULL;
}
char * file_str = "hehe huhu 123";
static char * test_eq(){
  FILE * file = _as_file(file_str);
  pt_tokenizer * stz = pt_tokenizer_read_string(file_str);
  pt_tokenizer * ftz = pt_tokenizer_read_file(file);
  pt_symbol_token * s_token = (pt_symbol_token *) pt_tokenizer_next(stz);
  pt_symbol_token * f_token = (pt_symbol_token *) pt_tokenizer_next(ftz);
  mu_assert("expected sym tokens have the same type",
      s_token->type == f_token->type);
  mu_assert("expected sym tokens to have the same value",
      strcmp(s_token->value, f_token->value) == 0);
  s_token = (pt_symbol_token *) pt_tokenizer_next(stz);
  f_token = (pt_symbol_token *) pt_tokenizer_next(ftz);
  mu_assert("expected sym tokens have the same type",
      s_token->type == f_token->type);
  mu_assert("expected sym tokens to have the same value",
      strcmp(s_token->value, f_token->value) == 0);

  pt_integer_token * s_token2 = (pt_integer_token *) pt_tokenizer_next(stz);
  pt_integer_token * f_token2 = (pt_integer_token *) pt_tokenizer_next(ftz);
  mu_assert("expected int tokens have the same type",
      s_token2->type == f_token2->type);
  mu_assert("expected int tokens to have the same value",
      s_token2->value == f_token2->value);

  return NULL;
}
static char * error_str = "12abc";
static char * test_error(){
  pt_tokenizer * tz = pt_tokenizer_read_string(error_str);
  pt_token * token = pt_tokenizer_next(tz);
  mu_assert("Expected error token to be null",
      token == NULL);
  mu_assert("Expected tz to be failed",
      pt_tokenizer_failed(tz));
  mu_assert("Expected error to be >invalid integer parse<",
      strcmp(tz->error, "invalid integer parse") == 0);
  return NULL;
}

static char * all_tests(){
  mu_run_test(test_int);
  mu_run_test(test_float);
  mu_run_test(test_symbol);
  mu_run_test(test_string);
  mu_run_test(test_tuple);
  mu_run_test(test_list);
  mu_run_test(test_quote);
  mu_run_test(test_lookup);
  mu_run_test(test_eq);
  mu_run_test(test_error);
  return NULL;
}

int main(){
  cr_debug_init_std();
  char * name = "TOKENIZER"; // change this to match the tested file
  pt_token_init();
  char * res = all_tests();
  pt_token_finalize();
  if(res){
    printf("%s TESTS FAILED: %s\n", name, res);
    return -1;
  }else{
    printf("%s TESTS: ALL TESTS PASSED\n", name);
  }
  printf("%i tests run.\n", tests_run);
  return 0;
}
