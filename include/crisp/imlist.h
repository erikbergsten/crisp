#ifndef __crisp_imlist_h__
#define __crisp_imlist_h__

#include <crisp/object.h>

extern cr_prototype cr_imlist_prototype;

typedef struct _cr_imlist{
  cr_prototype * prototype;
  struct _cr_imlist * next;
  cr_object * value;
} cr_imlist;



extern cr_imlist * cr_imlist_empty;

void cr_imlist_init();

cr_imlist * cr_imlist_prepend(cr_imlist * list, cr_object * value);
#define cr_imlist_prependS(l, v) cr_imlist_prepend(l, (cr_object *) v)

cr_imlist * cr_imlist_tail(cr_imlist * list);
cr_object * cr_imlist_head(cr_imlist * list);

cr_imlist * cr_imlist_from_str(char * str);
void cr_imlist_print_str(cr_imlist * imlist);

#endif /* __crisp_imlist_h__ */
