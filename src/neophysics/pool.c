#include "neophysics/pool.h"
#include <assert.h>

void NPPool_Init(NPPool *pool, NPAllocator *allocator, const np_u16 stride, const np_size max_size) {
 pool->_free_list = (np_u16*)allocator->np_malloc(max_size * stride + (max_size * sizeof(np_u16)));
 pool->_bytes = &((np_u8*)pool->_free_list)[max_size * sizeof(np_u16)];
 pool->_element_size = max_size;
 pool->_stride = stride;
 pool->_free_size = max_size - 1;
 
 np_s32 i = (np_s16)pool->_free_size;
 np_u32 ii = 0;
 while(i >= 0)
 	pool->_free_list[i--] = ii++;
}


void NPPool_Destroy(NPPool *pool, NPAllocator *allocator) {
 allocator->np_free(pool->_free_list);
}


void* NPPool_Malloc(NPPool *pool) {
	assert(pool->_free_size > -1 && "already reached the limit of pool");
	return (void*)&pool->_bytes[(np_size)pool->_free_list[pool->_free_size--] * (np_size)pool->_stride];
}


void NPPool_Free(NPPool *pool, void* ptr) {
 pool->_free_list[++pool->_free_size] = (np_u16)((((np_size)ptr) - (np_size)pool->_bytes) / (np_size)pool->_stride);
}



