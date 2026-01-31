#ifndef NP_POOL_H
#define NP_POOL_H

#include "neophysics/allocator.h"

#ifdef __cplusplus
extern "C" {
#endif


typedef struct {
 np_u8 *_bytes;
 np_u16 *_free_list;
 np_size _element_size;
 np_s32 _free_size;
 np_u16 _stride;
} NPPool;

void NPPool_Init(NPPool *pool, NPAllocator *allocator, const np_u16 stride, const np_size max_size);
void NPPool_Destroy(NPPool *pool, NPAllocator *allocator);
void* NPPool_Malloc(NPPool *pool);
void NPPool_Free(NPPool *pool, void* ptr);


#ifdef __cplusplus
}
#endif

#endif

