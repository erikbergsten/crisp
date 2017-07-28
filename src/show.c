#include <crisp/show.h>
#include <stdio.h>
#include <crisp/integer.h>
#include <crisp/double.h>
#include <crisp/imlist.h>
#include <crisp/tuple.h>
#include <crisp/function.h>
#include <crisp/quote.h>
#include <crisp/debug.h>

#define sprintfe(buf, format) sprintf(buf + strlen(buf), format)
#define sprintfa(buf, format, extra) sprintf(buf + strlen(buf), format, extra)


void _cr_show(char * dest, cr_object * obj);
void _show_list(char * dest, cr_imlist * list){
  sprintfe(dest, "(");
  while(list != cr_imlist_empty){
    _cr_show(dest, list->value);
    if(list->next != cr_imlist_empty){
      sprintfe(dest, " ");
    }
    list = list->next;
  }
  sprintfe(dest, ")");
}
void _show_tuple(char * dest, cr_tuple * tuple){
  sprintfe(dest, "{");
  for(int i = 0; i < tuple->length; i++){
    _cr_show(dest, tuple->values[i]);
    if(i != tuple->length - 1){
      sprintfe(dest, " ");
    }
  }
  sprintfe(dest, "}");
}
void _show_fun(char * dest, cr_fun * fun){
  if(fun->form == cr_function_native){
    if(fun->macro){
      sprintfe(dest, "[native macro]");
    }else{
      sprintfe(dest, "[native function]");
    }
  }else{
    if(fun->macro){
      sprintfe(dest, "[normal macro]");
    }else{
      sprintfe(dest, "[normal function]");
    }
  }
}
void _show_quote(char * dest, cr_quote * q){
  sprintfe(dest, "'");
  _cr_show(dest, q->value);
}
void _cr_show(char * dest, cr_object * obj){
  switch(obj->prototype->type){
    case cr_integer_type:
      {
        cr_integer * i = (cr_integer *) obj;
        sprintfa(dest, "%i", i->value);
        break;
      }
    case cr_double_type:
      {
        cr_double * d = (cr_double *) obj;
        sprintfa(dest, "%lf", d->value);
        break;
      }
    case cr_symbol_type:
      {
        cr_symbol * sym = (cr_symbol *) obj;
        sprintfa(dest, "%s", sym->name);
        break;
      }
    case cr_function_type:
      {
        _show_fun(dest, (cr_fun *) obj);
        break;
      }
    case cr_quote_type:
      {
        _show_quote(dest, (cr_quote *) obj);
        break;
      }
    case cr_tuple_type:
      {
        _show_tuple(dest, (cr_tuple *) obj);
        break;
      }
    case cr_list_type:
      {
        _show_list(dest, (cr_imlist *) obj);
        break;
      }
    case cr_unknown_type:
      {
        sprintfe(dest, "[unknown type]");
        break;
      }
  }
}
void cr_show(char * dest, cr_object * obj){
  dest[0] = '\0';
  _cr_show(dest, obj);
}
