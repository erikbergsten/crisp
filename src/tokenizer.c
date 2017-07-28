#include <crisp/tokenizer.h>
#include <stdio.h>
#include <crisp/list.h>
#include <string.h>
#include <stdlib.h>

typedef enum{
  file_reader,
  string_reader
} reader_type;

typedef struct {
  reader_type type;
  void * data;
  int index;
} reader_source;

char get_char(reader_source * rs){
  if(rs->type == file_reader){
    FILE * file = (FILE *) rs->data;
    return fgetc(file);
  }else if(rs->type == string_reader){
    char * str = (char *) rs->data;
    char c = str[rs->index++];
    if(c == '\0'){
      return EOF;
    }else{
      return c;
    }
  }else{
    return EOF;
  }
}

// token initializers 

cr_int_token * int_new(int value){
  cr_int_token * token = (cr_int_token *) malloc(sizeof(cr_int_token));
  token->value = value;
  token->type = cr_int_token_type;
  return token;
}
cr_text_token * str_new(char * value){
  cr_text_token * token = (cr_text_token *) malloc(sizeof(cr_text_token));
  token->str = value;
  token->type = cr_string_token_type;
  return token;
}
cr_float_token * float_new(double value){
  cr_float_token * token = (cr_float_token *) malloc(sizeof(cr_float_token));
  token->value = value;
  token->type = cr_float_token_type;
  return token;
}
cr_token start = (cr_token) {cr_list_start};
cr_token end = (cr_token) {cr_list_end};
cr_token tstart = (cr_token) {cr_tuple_start};
cr_token tend = (cr_token) {cr_tuple_end};
cr_token quote = (cr_token) {cr_quote_token_type};

//reader  defs
typedef enum {
  int_mode,
  float_mode,
  string_mode,
  symbol_mode,
  default_mode,
  char_mode
} reader_mode;

typedef struct {
  char buf[128];
  int index;
  int escape;
  reader_source rs;
  cr_list * tokens;
  int line, col;
  reader_mode mode;
} reader;

reader reader_new(){
  reader r;
  r.index = 0;
  r.tokens = cr_list_newP();
  r.line = 1;
  r.col = 0;
  r.escape = 0;
  r.mode = default_mode;
  return r;
}
reader file_reader_new(char * path){
  reader r = reader_new();
  reader_source rs;
  rs.type = file_reader;
  rs.data = (void *) fopen(path, "r");
  r.rs = rs;
  return r;
}
reader string_reader_new(char * str){
  reader r = reader_new();
  reader_source rs;
  rs.type = string_reader;
  rs.data = (void *) str;
  rs.index = 0;
  r.rs = rs;
  return r;
}
void update_counter(reader * r, char c){
  if(c == '\n'){
    r->line++;
    r->col = 0;
  }else{
    r->col++;
  }
}
int is_whitespace(char c){
  return (c == ' ') || (c == '\t') || (c == '\n');
}
int is_digit(char c){
  return c >= '0' && c <= '9';
}
int is_symbol_start(char c){
  return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') ||
         (c == '+') || (c == '*') || (c == '-') || (c == '_') ||
         (c == '/') || (c == '?') || (c == '!') || (c == '=');
}
int is_symbol(char c){
  return is_symbol_start(c) || (c >= '0' && c <= '9');
}
int is_terminal(char c){
  return is_whitespace(c) || c == ')' || c == '}';
}

#define reader_reset(r) do{r->mode = default_mode; r->index = 0;}while(0)
#define reader_append(r, c) do{r->buf[r->index++] = c;}while(0)
#define reader_last(r) (r->buf[r->index-1])

char * read_symbol(reader * r, char c){
  if(is_terminal(c)){
    reader_append(r, '\0');
    char * str = malloc(sizeof(char) * r->index);
    strcpy(str, r->buf);
    cr_text_token * t = str_new(str);
    t->type = cr_symbol_token_type;
    cr_list_appendS(r->tokens, t);
    if(c == ')'){
      cr_list_appendS(r->tokens, &end);
    }else if(c == '}'){
      cr_list_appendS(r->tokens, &tend);
    }
    reader_reset(r);
  }else if(is_symbol(c)){
    reader_append(r, c);
  }else{
    reader_append(r, '\0');
    return "invalid symbol parse";
  }
  return NULL;
}
char * read_string(reader * r, char c){
  if(r->escape){
    reader_append(r, c);
    r->escape = 0;
  }else{
    if(c == '"'){
      reader_append(r, '\0');
      char * str = malloc(sizeof(char) * r->index);
      strcpy(str, r->buf);
      cr_text_token * t = str_new(str);
      cr_list_appendS(r->tokens, t);
      reader_reset(r);
    }else if(c == '\\'){
      r->escape = 1;
    }else{
      reader_append(r, c);
    }
  }
  return NULL;
}
char * read_float(reader * r, char c){
  if(is_whitespace(c) || c == ')'){
    if(reader_last(r) == '.'){
      return "float cant be . terminated";
    }
    reader_append(r, '\0');
    double v = atof(r->buf);
    cr_float_token * t = float_new(v);
    cr_list_appendS(r->tokens, t);
    if(c == ')'){
      cr_list_appendS(r->tokens, &end);
    }else if(c == '}'){
      cr_list_appendS(r->tokens, &tend);
    }
    reader_reset(r);
  }else if(is_digit(c)){
    reader_append(r, c);
  }else{
    reader_append(r, '\0');
    return "invalid float parse";
  }
  return NULL;
}
char * read_int(reader * r, char c){
  if(is_terminal(c)){
    reader_append(r, '\0');
    int v = atoi(r->buf);
    cr_int_token * t = int_new(v);
    cr_list_appendS(r->tokens, t);
    if(c == ')'){
      cr_list_appendS(r->tokens, &end);
    }else if(c == '}'){
      cr_list_appendS(r->tokens, &tend);
    }
    reader_reset(r);
  }else if(is_digit(c)){
    reader_append(r, c);
  }else if(c == '.'){
    reader_append(r, c);
    r->mode = float_mode;
  }else{
    reader_append(r, '\0');
    return "invalid integer parse";
  }
  return NULL;
}
char * read_char(reader * r, char c){
  if(r->escape){
    //lul xD
    //no escaping enabled lululu
  }else{
    if(c == '\\'){
      r->escape = 1;
    }else{
      int v = (int) c;
      cr_int_token * t = int_new(v);
      cr_list_appendS(r->tokens, t);
      reader_reset(r);
    }
  }
  return NULL;
}
char * read_default(reader * r, char c){
  if(is_whitespace(c)){
  }else if(c == '\''){
    cr_list_appendS(r->tokens, &quote);
  }else if(c == '('){
    cr_list_appendS(r->tokens, &start);
  }else if(c == ')'){
    cr_list_appendS(r->tokens, &end);
  }else if(is_digit(c)){
    r->mode = int_mode;
    reader_append(r, c);
  }else if(c == '"'){
    r->mode = string_mode;
  }else if(is_symbol_start(c)){
    r->mode = symbol_mode;
    reader_append(r, c);
  }else if(c == '$'){
    r->mode = char_mode;
  }else if(c == '{'){
    cr_list_appendS(r->tokens, &tstart);
  }else if(c == '}'){
    cr_list_appendS(r->tokens, &tend);
  }else{
    return "unrecognized symbol";
  }
  return NULL;
}

char * read(reader * r, char c){
  update_counter(r, c);
  switch(r->mode){
    case default_mode:
      return read_default(r, c);
      break;
    case int_mode:
      return read_int(r, c);
      break;
    case char_mode:
      return read_char(r, c);
      break;
    case float_mode:
      return read_float(r, c);
      break;
    case string_mode:
      return read_string(r, c);
      break;
    case symbol_mode:
      return read_symbol(r, c);
      break;
  }
  return NULL;
}
cr_list * cr_tokenize(reader r){
  char c;
  while((c = get_char(&r.rs)) != EOF){
    char * error = read(&r, c);
    if(error){
      printf("error on line %i col %i: %s\n", r.line, r.col, error);
      printf("bad char: %c, buffer contains: %s\n", c, r.buf);
    }
  }
  return r.tokens;
}
cr_list * cr_tokenize_file(char * path){
  reader r = file_reader_new(path);
  cr_list * tokens = cr_tokenize(r);
  fclose(r.rs.data);
  return tokens;
}
cr_list * cr_tokenize_string(char * str){
  reader r = string_reader_new(str);
  return cr_tokenize(r);
}

