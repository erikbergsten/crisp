#include <crisp/gc.h>
#include <crisp/map.h>
#include <crisp/integer.h>

cr_prototype cr_integer_prototype = {
  cr_integer_type,
  "integer",
  (cr_hash) cr_integer_hash,
  (cr_cmp) cr_integer_cmp
};

cr_integer * cr_integer_new(int value){
  cr_integer * integer = (cr_integer *) cr_mallocS(sizeof(cr_integer), NULL);
  integer->prototype = &cr_integer_prototype;
  integer->value = value;
  return integer;
}

uint32_t cr_integer_hash(cr_integer * integer){
  return cr_int_hash(integer->value);
}

int cr_integer_cmp(cr_integer * int1, cr_integer * int2){
  return cr_int_cmp(int1->value, int2->value);
}

