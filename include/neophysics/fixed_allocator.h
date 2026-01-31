#ifndef NP_FIXED_ALLOCATOR_H
#define NP_FIXED_ALLOCATOR_H

#include "neophysics/allocator.h"

#ifdef __cplusplus
extern "C" {
#endif


typedef struct {
 np_u8 *_bytes;
 np_size _max_size;
 np_size _current_size;
} NPFixedAllocator;

void NPFixedAllocator_Init(NPFixedAllocator *_ctx, NPAllocator *_alloc, np_size max_size);
void NPFixedAllocator_Destroy(NPFixedAllocator *_ctx, NPAllocator *_alloc);
void* NPFixedAllocator_Malloc(NPFixedAllocator *_ctx, np_size bytes_size, np_u8 alignment);
void NPFixedAllocator_Reset(NPFixedAllocator *_ctx);


#ifdef __cplusplus
}
#endif

#endif

