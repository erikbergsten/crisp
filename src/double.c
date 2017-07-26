#include <crisp/gc.h>
#include <crisp/double.h>
#include <math.h>

cr_prototype cr_double_prototype = {
  cr_double_type,
  "double",
  (cr_hash) NULL,
  (cr_cmp) cr_double_cmp
};

cr_double * cr_double_new(double value){
  cr_double * dubbs = (cr_double *) cr_mallocS(sizeof(cr_double), NULL);
  dubbs->prototype = &cr_double_prototype;
  dubbs->value = value;
  return dubbs;
}
int cr_double_cmp(cr_double * dub1, cr_double * dub2){
  double diff = dub1->value - dub2->value;
  if(diff < 0){
    return floor(diff);
  }else{
    return ceil(diff);
  }
}

