#ifndef NP_ISLAND3D_H
#define NP_ISLAND3D_H

#include "neophysics/types.h"
#include "neophysics/real3.h"
#include "neophysics/pool.h"
#include "neophysics/fixed_allocator.h"
#include "neophysics/core3d/variant3d.h"

#ifdef __cplusplus
extern "C" {
#endif



struct NPIsland3D {
 NPFixedAllocator _broadphase_allocator;
 np_u16 _body_begin;
 np_u16 _body_end;
 np_u16 *_id_array;
 NPBody3D *_bodies;
 NPReal3 _aabb_min;
 NPReal3 _aabb_max;
 np_u16 _id_size;
};

void NPIsland3D_Init(NPIsland3D *island, NPWorld3D *ctx);
void NPIsland3D_Merge(NPIsland3D *island, NPWorld3D *ctx, NPIsland3D *other_island);
void NPIsland3D_Update(NPIsland3D *island, NPWorld3D *ctx, np_real dt);
void NPIsland3D_Destroy(NPIsland3D *island, NPWorld3D *ctx);



#ifdef __cplusplus
}
#endif

#endif

