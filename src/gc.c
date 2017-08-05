#include <crisp/gc.h>


typedef struct {
  cr_destroy destroy;
  int refs;
} _ptr_meta;

#define add_bytes(ptr, bytes) ((void *) (((char *) ptr) + bytes))

void * cr_malloc(size_t size, cr_destroy destroy){
  _ptr_meta * meta = malloc(sizeof(_ptr_meta) + size);
  meta->destroy = destroy;
  meta->refs = 0;
  return add_bytes(meta, sizeof(_ptr_meta));
}

void * cr_ref(void * ptr){
  _ptr_meta * meta = add_bytes(ptr, -sizeof(_ptr_meta));
  meta->refs++;
  return ptr;
}

void cr_free(void * ptr){
  _ptr_meta * meta = add_bytes(ptr, -sizeof(_ptr_meta));
  if(--meta->refs < 1){
    if(meta->destroy) meta->destroy(ptr);
    free(meta);
  }
}
void cr_force_free(void * ptr){
  _ptr_meta * meta = add_bytes(ptr, -sizeof(_ptr_meta));
  meta->refs = 1;
  cr_free(ptr);
}
void * cr_ref_meta_addr(void * ptr){
  return add_bytes(ptr, -sizeof(_ptr_meta));
}
int cr_ref_count(void * ptr){
  _ptr_meta * meta = add_bytes(ptr, -sizeof(_ptr_meta));
  return meta->refs;
}
