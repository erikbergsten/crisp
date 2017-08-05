#ifndef __crisp_value_h__
#define __crisp_value_h__

#include <crisp/map.h>


typedef enum {
  cr_integer_type,
  cr_double_type,
  cr_list_type,
  cr_symbol_type,
  cr_lookup_type,
  cr_tuple_type,
  cr_quote_type,
  cr_function_type,
  cr_macro_type,      //not really a type but w/e
  cr_unknown_type
} cr_type;

typedef struct {
  cr_type type;
  const char * name;
  cr_hash hash;
  cr_cmp cmp;
} cr_prototype;

typedef struct {
  cr_prototype * prototype;
} cr_object;

uint32_t cr_object_hash(cr_object * object);
int cr_object_cmp(cr_object * obj1, cr_object * obj2);
#define  cr_object_type(o, t) (o->prototype->type == t)

#endif /* __crisp_value_h__ */
