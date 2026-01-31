#ifndef NP_ALLOCATOR_H
#define NP_ALLOCATOR_H

#include "neophysics/types.h"

typedef struct {
 void* (*np_malloc)(np_size m);
 void (*np_free)(void* m);
} NPAllocator;

#endif

