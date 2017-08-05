#include <crisp/reader.h>
#include <crisp/symbol.h>
#include <crisp/imlist.h>





void cr_reader_init(){

}

void cr_read(cr_object * statement, cr_runtime * runtime){
  //first we have to look for special "meta" statements by looking at the
  //head in each list. the builtin functions def and module are handled here
  if(cr_object_type(statement, cr_list_type)){
    cr_imlist * list = (cr_imlist *) statement;
    if(list == cr_imlist_empty){
      return cr_eval(list);
    }
    cr_object * head = list->value;
  }
}
