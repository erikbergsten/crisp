#include <crisp/object.h>
#include <crisp/debug.h>


uint32_t cr_object_hash(cr_object * object){
#ifdef CR_DEBUG
  if(object->prototype == NULL){
    cr_debug_warn("Trying to hash object without prototype\n");
  }
#endif
  if(object->prototype->hash){
    return object->prototype->hash(object);
  }
  cr_debug_warn("Trying to hash object of type %s which has no hash function.",
                 object->prototype->name);
  return 0;
}
int cr_object_cmp(cr_object * obj1, cr_object * obj2){
#ifdef CR_DEBUG
  if(obj1->prototype == NULL){
    cr_debug_warn("Trying to compare object without prototype\n");
  }
  if(obj2->prototype == NULL){
    cr_debug_warn("Trying to compare object without prototype\n");
  }
#endif
  if(obj1->prototype == obj2->prototype){
    return obj1->prototype->cmp(obj1, obj2);
  }else{
    return 1;
  }
}

