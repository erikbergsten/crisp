#include <crisp/list.h>
#include <stdlib.h>
#include <stdio.h>

cr_node * cr_node_new(cr_node * prev, cr_node * next, size_t elem_size, void * value){
  size_t size = sizeof(void *) - elem_size;
  cr_node * node = (cr_node *) malloc(sizeof(cr_node));
  node->prev = prev;
  node->next = next;
  node->value = value;
  return node;
}

cr_list * cr_list_new(size_t elem_size){
  cr_list * list = (cr_list *) malloc(sizeof(cr_list));
  list->head = NULL;
  list->end = NULL;
  list->elem_size = elem_size;
  return list;
}

int cr_list_length(cr_list * list){
  cr_node * node = list->head;

  int i = 0;
  while(node){
    node = node->next;
    i++;
  }
  return i;
}

void cr_list_append(cr_list * list, void * elem){
  cr_node * end = cr_node_new(list->end, NULL, list->elem_size, elem);
  if(list->end){
    list->end->next = end;
  }else{
    //list was empty so teh new element is both head and end
    list->head = end;
    end->prev = NULL;
  }
  list->end = end;
}
void cr_list_prepend(cr_list * list, void * elem){
  cr_node * head = cr_node_new(NULL, list->head, list->elem_size, elem);
  if(list->head){
    list->head->prev = head;
  }else{
    list->end = head;
  }
  list->head = head;
}
void * cr_list_pop(cr_list * list){
  if(list->end){
    cr_node * end = list->end;
    void * value = end->value;

    if(list->head == end){
      //was singleton - now empty!
      list->head = NULL;
      list->end = NULL;
    }else{
      end->prev->next = NULL;
      list->end = end->prev;
    }
    free(end);
    return value;
  }else{
    return NULL;
  }
}
void * cr_list_shift(cr_list * list){
  if(list->head){
    cr_node * head = list->head;
    void * value = head->value;

    if(list->end == head){
      //was singleton - now empty!
      list->end = NULL;
      list->head = NULL;
    }else{
      head->next->prev = NULL;
      list->head = head->next;
    }
    free(head);
    return value;
  }else{
    return NULL;
  }
}
void cr_list_remove(cr_list * list, cr_node * node){
  if(list->head == node){
    cr_list_shift(list);
  }else if(list->end == node){
    cr_list_pop(list);
  }else{
    node->prev->next = node->next;
    node->next->prev = node->prev;
    free(node);
  }
}
void cr_list_destroy(cr_list * list){
  cr_node * node, * next;
  next = list->head;
  while((node = next) != NULL){
    next = node->next;
    free(node);
  }
  free(list);
}
