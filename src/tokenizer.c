#include <parsington/tokenizer.h>
#include <parsington/token.h>
#include <stdlib.h>
#include <string.h>
#include <crisp/debug.h>

pt_tokenizer * pt_tokenizer_read_string(char * str){
  pt_tokenizer * tz =
    (pt_tokenizer *) malloc(sizeof(pt_string_tokenizer));
  tz->row = 0;
  tz->col = 0;
  tz->status = PT_TOKENIZER_INIT;
  tz->source = pt_string_source;
  tz->buf[0] = '\0';
  tz->buf_at = 0;
  tz->peek = 0;
  pt_string_tokenizer * stz = (pt_string_tokenizer *) tz;
  stz->str = str;
  stz->index = 0;
  return tz;
}
pt_tokenizer * pt_tokenizer_read_file(FILE * file){
  pt_tokenizer * tz =
    (pt_tokenizer *) malloc(sizeof(pt_file_tokenizer));
  tz->row = 0;
  tz->col = 0;
  tz->buf_at = 0;
  tz->status = PT_TOKENIZER_INIT;
  tz->source = pt_file_source;
  tz->buf[0] = '\0';
  pt_file_tokenizer * ftz = (pt_file_tokenizer *) tz;
  ftz->file = file;
  return  tz;
}
pt_tokenizer * pt_tokenizer_read_path(char * src){
  //ullulul
  printf("readpath not impletented errorerrorerorroerror\n");
  return NULL;
}

#define _buf_add(tz, c) (tz->buf[tz->buf_at++] = c)
#define _buf_last(tz) (tz->buf[tz->buf_at])
#define _buf_reset(tz) (tz->buf_at = 0)
#define _fail(tz, msg) do{tz->status = PT_TOKENIZER_FAILED; tz->error = msg;}while(0)
#define is_symbol_start(c) ((c >= 'a' && c <= 'z') || \
                           (c >= 'A' && c <= 'Z') || \
                           (c == '+') || (c == '*'))
#define is_symbol(c) (is_symbol_start(c) || (c >= '0' && c <= '9'))
#define is_whitespace(c) ((c == ' ') || (c == '\n') || (c == '\t'))
#define is_terminal(c) (c == ')' || c == '}')
#define is_digit(c) (c >= '0' && c <= '9')
void _pop(pt_tokenizer * tz){
  if(tz->source == pt_file_source){
    pt_file_tokenizer * ftz = (pt_file_tokenizer *) tz;
    tz->peek = fgetc(ftz->file);
  }else if(tz->source == pt_string_source){
    pt_string_tokenizer * stz = (pt_string_tokenizer *) tz;
    tz->peek = stz->str[stz->index++];
  }
}
#define _peek(tz) (tz->peek)
pt_token * _read_integer(pt_tokenizer * tz, char c){
  if(is_whitespace(c) || is_terminal(c)){
    _buf_add(tz, '\0');
    pt_token * token = pt_integer_token_new(atoi(tz->buf));
    if(is_whitespace(c)){

      //STOPPED POPPING BECAUSE IT PROBABLY WASNT NECESSARY BUT MAYBE IT WAS
      //THERE FOR SOME REASON SO CHECK THIS OUT IF YOU FIND A STRANGE PARSING
      //BUG

      //_pop(tz);
    }
    _buf_reset(tz);
    tz->mode = pt_default_mode;
    return token;
  }else if(is_digit(c)){
    _buf_add(tz, c);
    _pop(tz);
  }else if(c == '.'){
    tz->mode = pt_float_mode;
    _buf_add(tz, c);
    _pop(tz);
  }else{
    _fail(tz, "invalid integer parse");
  }
  return NULL;
}
pt_token * _read_float(pt_tokenizer * tz, char c){
  if(is_whitespace(c) || is_terminal(c)){
    if(_buf_last(tz) == '.'){
      _fail(tz, "invalid float parse: \".\" terminated float is not allowed");
      return NULL;
    }
    _buf_add(tz, '\0');
    double v = atof(tz->buf);
    pt_token * token = pt_float_token_new(v);
    _buf_reset(tz);
    tz->mode = pt_default_mode;
    return token;
  }else if(is_digit(c)){
    _buf_add(tz, c);
    _pop(tz);
  }else{
    _fail(tz, "invalid float parse");
  }
  return NULL;
}
pt_token * _read_char(pt_tokenizer * tz, char c){
  pt_token * token = pt_integer_token_new((size_t) c);
  tz->mode = pt_default_mode;
  _pop(tz);
  return token;
}
pt_token * _read_symbol(pt_tokenizer * tz, char c){
  if(is_whitespace(c) || is_terminal(c)){
    //done!!
    _buf_add(tz, '\0');
    pt_token * token = pt_symbol_token_new(tz->buf);
    _buf_reset(tz);
    tz->mode = pt_default_mode;
    return token;
  }else if(c == '/'){
    tz->mode = pt_lookup_mode;
    _buf_add(tz, c);
    _pop(tz);
  }else if(is_symbol(c)){
    _buf_add(tz, c);
    _pop(tz);
  }else{
    _fail(tz, "invalid symbol parse");
  }
  return NULL;
}
pt_token * _read_string(pt_tokenizer * tz, char c){
  //add escaping!!
  if(c == '"'){
    //done
    _buf_add(tz, '\0');
    pt_token * token = pt_string_token_new(tz->buf);
    _buf_reset(tz);
    tz->mode = pt_default_mode;
    _pop(tz);
    return token;
  }else{
    _buf_add(tz, c);
    _pop(tz);
  }
  return NULL;
}
pt_token * _read_lookup(pt_tokenizer * tz, char c){
  if(is_whitespace(c) || is_terminal(c)){
    //done!!
    _buf_add(tz, '\0');
    int module_length = (int) (strchr(tz->buf, '/') - tz->buf);
    tz->buf[module_length] = '\0';
    char * name = tz->buf + module_length + 1;
    pt_token * token = pt_lookup_token_new(tz->buf, name);
    _buf_reset(tz);
    tz->mode = pt_default_mode;
    if(is_whitespace(c)){
      _pop(tz);
    }
    return token;
  }else if(is_symbol(c)){
    _buf_add(tz, c);
    _pop(tz);
  }else{
    _fail(tz, "invalid lookup parse");
  }
  return NULL;
}
pt_token * _read_default(pt_tokenizer * tz, char c){
  if(is_whitespace(c)){
    _pop(tz);
    return NULL;
  }else if(c == '('){
    _pop(tz);
    return pt_list_start_token;
  }else if(c == ')'){
    _pop(tz);
    return pt_list_end_token;
  }else if(c == '{'){
    _pop(tz);
    return pt_tuple_start_token;
  }else if(c == '}'){
    _pop(tz);
    return pt_tuple_end_token;
  }else if(c == '\''){
    _pop(tz);
    return pt_quote_token;
  }else if(is_digit(c)){
    tz->mode = pt_integer_mode;
    _buf_add(tz, c);
    _pop(tz);
  }else if(c == '"'){
    tz->mode = pt_string_mode;
    _pop(tz);
  }else if(is_symbol_start(c)){
    tz->mode = pt_symbol_mode;
    _buf_add(tz, c);
    _pop(tz);
  }else if(c == '$'){
    tz->mode = pt_char_mode;
    _pop(tz);
  }
  return NULL;
}
void _update_counter(pt_tokenizer * tz, char c){
  if(c == '\n'){
    tz->row += 1;
    tz->col = 0;
  }else{
    tz->col += 1;
  }
}
pt_token * pt_tokenizer_next(pt_tokenizer * tz){
  if(tz->status == PT_TOKENIZER_INIT){
    _pop(tz);
    tz->status = PT_TOKENIZER_RUNNING;
    tz->mode = pt_default_mode;
  }else if(tz->status == PT_TOKENIZER_SUCCESS){
    return NULL;
  }
  char c;
  pt_token * token = NULL;
  c = _peek(tz);
  while(token == NULL &&           //we found a token and can move on
        pt_tokenizer_running(tz)){ //the tokenizer has failed or finished
    if(c == EOF || c == '\0'){
      tz->status = PT_TOKENIZER_SUCCESS;
      c = ' ';
    }
    _update_counter(tz, c);
    switch(tz->mode){
      case pt_default_mode:
        token = _read_default(tz, c);
        break;
      case pt_integer_mode:
        token = _read_integer(tz, c);
        break;
      case pt_float_mode:
        token = _read_float(tz, c);
        break;
      case pt_symbol_mode:
        token = _read_symbol(tz, c);
        break;
      case pt_string_mode:
        token = _read_string(tz, c);
        break;
      case pt_char_mode:
        token = _read_char(tz, c);
        break;
      case pt_lookup_mode:
        token = _read_lookup(tz, c);
        break;
    }
    c = _peek(tz);
  }
  if(pt_tokenizer_failed(tz)){
    return NULL;
  }else if(token != NULL){
    return token;
  }else if(tz->mode == pt_default_mode){
    //if we are in default mode we have only read whitespace in this parse and
    //reached termination, i.e there was nothing left to parse
    return NULL;
  }else{
    tz->error = "Unexpected EOF";
    tz->status = PT_TOKENIZER_FAILED;
    return NULL;
  }
}

void pt_tokenizer_free(pt_tokenizer * tz){
  if(tz->source == pt_file_source){
    pt_file_tokenizer * ftz = (pt_file_tokenizer *) tz;
    fclose(ftz->file);
  }else if(tz->source == pt_string_source){
    pt_string_tokenizer * stz = (pt_string_tokenizer *) stz;
    //free string? hmmm skip it for now.... keheheh...
  }
  free(tz);
}
