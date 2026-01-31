#ifndef NP_WORLD3D_H
#define NP_WORLD3D_H

#include "neophysics/types.h"
#include "neophysics/real3.h"
#include "neophysics/pool.h"
#include "neophysics/fixed_allocator.h"
#include "neophysics/core3d/variant3d.h"

#ifdef __cplusplus
extern "C" {
#endif

struct NPWorldConfig3D {
 NPReal3 gravity;
 np_real compare_epsilon;
 np_u8 iteration_steps;
 np_u8 broadphase_type;
};

struct NPWorldLimits3D {
 np_size max_temp_buffer_bytes;
 np_size max_broadphase_allocator_bytes;
 np_u32 max_contact_size;
 np_u16 max_island;
 np_u16 max_objects_per_island;
 np_u16 max_bodies;
 np_u16 max_constraints;
};


struct NPWorld3D {
 NPWorldLimits3D _limits;
 NPWorldConfig3D _config;
 NPAllocator _allocator;
 NPPool _body_pool;
 NPPool _constraint_pool;
 NPPool _island_pool;
 NPPool _contact_hash_node_pool;
 NPFixedAllocator _fixed_allocator;
 
 np_real _inv_iteration_steps;
};


void NPWorld3D_Init(NPWorld3D *ctx, NPWorldLimits3D *limits, NPAllocator *allocator);
void NPWorld3D_Destroy(NPWorld3D *ctx);
np_u16 NPWorld3D_AddBody(NPWorld3D *ctx, void *object_data);
void NWorld3D_RemoveBody(np_u16 id);

#ifdef __cplusplus
}
#endif

#endif

