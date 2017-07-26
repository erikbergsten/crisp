#include <crisp/quote.h>
#include <crisp/gc.h>

void cr_quote_destroy(cr_quote * quote){
  cr_free(quote->value);
}

cr_prototype cr_quote_prototype = {
  cr_quote_type,
  "quote",
  (cr_hash) NULL,
  (cr_cmp) NULL
};


cr_quote * cr_quote_new(cr_object * object){
  cr_quote * quote = (cr_quote *) cr_mallocS(sizeof(cr_quote), cr_quote_destroy);
  quote->value = cr_ref(object);
  quote->prototype = &cr_quote_prototype;
  return quote;
}
