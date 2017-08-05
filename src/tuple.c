#include <crisp/tuple.h>
#include <crisp/gc.h>

int cr_tuple_cmp(cr_tuple * t1, cr_tuple * t2){
  if(t1->length != t2->length){
    return 1;
  }else{
    for(int i = 0; i < t1->length; i++){
      if(cr_object_cmp(t1->values[i], t2->values[i]) != 0){
        return 1;
      }
    }
    return 0;
  }
}
uint32_t cr_tuple_hash(cr_tuple * tuple){
  uint32_t hash = 0;
  for(int i = 0; i < tuple->length; i++){
    hash = hash * 31 + cr_object_hash(tuple->values[i]);
  }
  return hash;
}
cr_prototype cr_tuple_prototype = {
  cr_tuple_type,
  "tuple",
  (cr_hash) cr_tuple_hash,
  (cr_cmp) cr_tuple_cmp
};

void cr_tuple_destroy(cr_tuple * tuple){
  for(int i = 0; i < tuple->length; i++){
    cr_free(tuple->values[i]);
  }
}
cr_tuple * cr_tuple_new(int n, cr_object ** values){
  cr_tuple * tuple = (cr_tuple *) cr_mallocS(sizeof(cr_tuple), cr_tuple_destroy);
  tuple->prototype = &cr_tuple_prototype;
  tuple->length = n;
  tuple->values = malloc(sizeof(cr_object *) * n);
  for(int i = 0; i < n; i++){
    tuple->values[i] = cr_ref(values[i]);
  }
  return tuple;
}
cr_tuple * cr_tuple_from_list(cr_list * values){
  cr_tuple * tuple = (cr_tuple *) cr_mallocS(sizeof(cr_tuple), cr_tuple_destroy);
  tuple->prototype = &cr_tuple_prototype;
  tuple->length = cr_list_length(values);
  tuple->values = malloc(sizeof(cr_object *) * tuple->length);
  int i = 0;
  for(cr_node * node = values->head; node; node = node->next){
    cr_object * object = (cr_object *) node->value;
    tuple->values[i++] = cr_ref(object);
  }
  return tuple;
}
cr_tuple * cr_pair_new(cr_object * obj1, cr_object * obj2){
  cr_tuple * tuple = (cr_tuple *) cr_mallocS(sizeof(cr_tuple), cr_tuple_destroy);
  tuple->prototype = &cr_tuple_prototype;
  tuple->length = 2;
  tuple->values = malloc(sizeof(cr_object *) * 2);
  tuple->values[0] = cr_ref(obj1);
  tuple->values[1] = cr_ref(obj2);
  return tuple;
}

cr_object * cr_tuple_fst(cr_tuple * tuple){
  return tuple->values[0];
}
cr_object * cr_tuple_snd(cr_tuple * tuple){
  return tuple->values[1];
}
cr_tuple * cr_tuple_error(char * str){
  cr_imlist * message = cr_imlist_from_str(str);
  cr_symbol * sym = cr_symbol_new("error");
  return cr_pair_new((cr_object *) sym, (cr_object *) message);
}
