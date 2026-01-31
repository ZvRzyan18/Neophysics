#include "neophysics/core3d/world3d.h"
#include "neophysics/core3d/body3d.h"
#include "neophysics/core3d/island3d.h"

#include <string.h>


void NPWorld3D_Init(NPWorld3D *ctx, NPWorldLimits3D *limits, NPAllocator *allocator) {
 memset(ctx, 0x0, sizeof(NPWorld3D));
 ctx->_limits = *limits;
 ctx->_allocator = *allocator;
 NPPool_Init(&ctx->_body_pool, &ctx->_allocator, sizeof(NPBody3D), ctx->_limits.max_bodies);
 NPPool_Init(&ctx->_island_pool, &ctx->_allocator, sizeof(NPIsland3D), ctx->_limits.max_island);
}


void NPWorld3D_Destroy(NPWorld3D *ctx) {
 NPPool_Destroy(&ctx->_body_pool, &ctx->_allocator);
 NPPool_Destroy(&ctx->_island_pool, &ctx->_allocator);
}


np_u16 NPWorld3D_AddBody(NPWorld3D *ctx, void *object_data) {
 (void)ctx;
 (void)object_data;
 return 0;
}


void NWorld3D_RemoveBody(np_u16 id) {
 (void)id;
}



