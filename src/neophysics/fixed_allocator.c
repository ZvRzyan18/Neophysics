#include "neophysics/fixed_allocator.h"
#include <assert.h>

void NPFixedAllocator_Init(NPFixedAllocator *_ctx, NPAllocator *_alloc, np_size max_size) {
 _ctx->_bytes = (np_u8*)_alloc->np_malloc(max_size);
 _ctx->_max_size = max_size;
 _ctx->_current_size = 0;
}


void NPFixedAllocator_Destroy(NPFixedAllocator *_ctx, NPAllocator *_alloc) {
 _alloc->np_free(_ctx->_bytes);
}


void* NPFixedAllocator_Malloc(NPFixedAllocator *_ctx, np_size bytes_size, np_u8 alignment) {
 void *p1, **p2, *new_ptr;
 new_ptr = (void*)&_ctx->_bytes[_ctx->_current_size];
 np_size offset = alignment - 1 + sizeof(void*);
 _ctx->_current_size += offset + bytes_size;

 assert(_ctx->_current_size < _ctx->_max_size);
 
 p1 = new_ptr;
 p2 = (void**)(((np_size)p1 + offset) & ~(alignment - 1));
 p2[-1] = p1;
 return p2;
}


void NPFixedAllocator_Reset(NPFixedAllocator *_ctx) {
 _ctx->_current_size = 0;
}


