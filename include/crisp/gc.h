#ifndef __crisp_gc_h__
#define __crisp_gc_h__

#include <stdlib.h>
typedef void (*cr_destroy)(void*);

void * cr_malloc(size_t size, cr_destroy destroy);
#define cr_mallocS(size, destroy) cr_malloc(size, (cr_destroy) destroy)
void * cr_ref(void * ptr);
#define cr_refS(ptr) cr_ref((void *) ptr)
void cr_free(void * ptr);
#define cr_freeS(ptr) cr_free((void *) ptr)
void * cr_ref_meta_addr(void * ptr);
void cr_force_free(void * ptr);
int cr_ref_count(void * ptr);
#define cr_ref_countS(ptr) cr_ref_count((void *) ptr)

#endif /* __crisp_gc_h__ */
