#include <crisp/imlist.h>
#include <crisp/gc.h>
#include <crisp/integer.h>
#include <stdio.h>

cr_imlist * cr_imlist_empty = NULL;
int cr_imlist_cmp(cr_imlist * list1, cr_imlist * list2){
  if((list1 == cr_imlist_empty) || (list2 == cr_imlist_empty)){
    return list1 != list2;
  }
  while(list1 != cr_imlist_empty && list2 != cr_imlist_empty){
    if(cr_object_cmp(list1->value, list2->value) != 0){
      return 1;
    }
    list1 = list1->next;
    list2 = list2->next;
  }
  //if either of the lists are non null after the loop it must be longer
  return list1 != cr_imlist_empty || list2 != cr_imlist_empty;
}
uint32_t cr_imlist_hash(cr_imlist * list){
  if(list == cr_imlist_empty){
    return 0;
  }
  uint32_t hash = 0;
  while(list != cr_imlist_empty){
    hash = hash*31 + cr_object_hash(list->value);
    list = list->next;
  }
  return hash;
}
cr_prototype cr_imlist_prototype = {
  cr_list_type,
  "list",
  (cr_hash) cr_imlist_hash,
  (cr_cmp) cr_imlist_cmp
};

void cr_imlist_init(){
  cr_imlist_empty = (cr_imlist *) cr_ref(cr_malloc(sizeof(cr_imlist), NULL));
  cr_imlist_empty->prototype = &cr_imlist_prototype;
}
void cr_imlist_destroy(cr_imlist * list){
  cr_free(list->value);
  if(list->next != cr_imlist_empty){
    cr_free(list->next);
  }
}

cr_imlist * cr_imlist_prepend(cr_imlist * list, cr_object * value){
  cr_imlist * new = (cr_imlist *) cr_mallocS(sizeof(cr_imlist), cr_imlist_destroy);
  new->prototype = &cr_imlist_prototype;
  new->next =  cr_ref(list);
  new->value = cr_ref(value);
  return new;
}

cr_imlist * cr_imlist_tail(cr_imlist * list){
  return list->next;
}
cr_object * cr_imlist_head(cr_imlist * list){
  return list->value;
}
cr_imlist * cr_imlist_from_str(char * str){
  cr_imlist * list = cr_imlist_empty;
  for(int i = strlen(str) - 1; i >= 0; i--){
    list = cr_imlist_prependS(list, cr_integer_new((int) str[i]));
  }
  return list;
}
void cr_imlist_print_str(cr_imlist * list){
  while(list != cr_imlist_empty){
    cr_integer * i = (cr_integer *) list->value;
    if(! (cr_object_type(i, cr_integer_type) && i->value < 128)){
      printf("?");
    }else{
      printf("%c", (char) i->value);
    }
    list = list->next;
  }
  printf("\n");
}
cr_imlist * cr_imlist_reverse(cr_imlist * list){
  //slow and steady reverse function!
  cr_imlist * rev = cr_imlist_empty;
  while(list != cr_imlist_empty){
    rev = cr_imlist_prepend(rev, list->value);
    list = list->next;
  }
  return rev;
}
