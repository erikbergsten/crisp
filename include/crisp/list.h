#ifndef __cr_list_h__
#define __cr_list_h__

#include <stddef.h>

typedef struct _cr_node {
  struct _cr_node *next, *prev;
  void * value;
} cr_node;


typedef struct {
  cr_node *head, *end;
  size_t elem_size;
} cr_list;

cr_list * cr_list_new(size_t elem_size);
void cr_list_destroy(cr_list * list);
#define cr_list_newP() cr_list_new(sizeof(void *))
int cr_list_length(cr_list * list);

void cr_list_remove(cr_list * list, cr_node * node);

void cr_list_append(cr_list * list, void * elem);
#define cr_list_appendS(list, elem) cr_list_append(list, (void *) elem)
void cr_list_prepend(cr_list * list, void * elem);
#define cr_list_prependS(list, elem) cr_list_prepend(list, (void *) elem)
void * cr_list_pop(cr_list * list);
void * cr_list_shift(cr_list * list);

#define cr_list_head(list) (list->head ? list->head->value : NULL)
#define cr_list_end(list) (list->end ? list->end->value : NULL)


#endif /* __cr_list_h__ */
