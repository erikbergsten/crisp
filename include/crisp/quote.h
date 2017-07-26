#ifndef __crisp_quote_h__
#define __crisp_quote_h__

#include <crisp/object.h>
typedef struct {
  cr_prototype * prototype;
  cr_object * value;
} cr_quote;

cr_quote * cr_quote_new(cr_object * object);


#endif /* __crisp_quote_h__ */
