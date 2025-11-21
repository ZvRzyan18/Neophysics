#ifndef NP_CORE2D_H
#define NP_CORE2D_H

#include "neophysics/types.h"

/*
 custom alias
*/
typedef NPPointer NPWorld2D;
typedef NPUint16  NPBody2D;
typedef NPPointer NPReadonlyBuffer2D;


/*
 enum contants
*/
enum NPBodyType2D {
	NP_POLYGON_2D = 1,
};

enum NPMovementType2D {
	NP_DYNAMIC_2D,
	NP_KINEMATIC_2D,
	NP_STATIC_2D,
};

enum NPDampingDecayType2D {
 NP_DAMPING_EXPONENITAL_DECAY_2D,
 NP_DAMPING_LINEAR_DECAY_2D,
};

enum NPBroadphaseType2D {
 NP_BROADPHASE_BRUITE_FORCE_2D,
};


//-------------------------------------------------------------------//

/*
 structures
*/
typedef struct {
	NPPointer (*malloc)(NPUint32 size);
	void (*free)(NPPointer ptr);
	void (*error)(NPInt32 code);
} NPInputCallback2D;

/*
 polygon body 2d
*/
typedef struct {
	NPUint16 body_type;
	NPUint16 movement_type;
	NPReal   scale[2];
	NPReal   position[2];
	NPReal   rotation;
	NPReal   density;
	NPReal   restitution;
	NPReal   linear_damping[2];
	NPReal   angular_damping;
	
	NPReadonlyBuffer2D vertices_buffer;
} NPPolygonBody2DComponents;

//-------------------------------------------------------------------//

/*
 create a NPWorld2D object.
 returns null if failed.
*/
NP_API NPWorld2D npWorld2DCreate(const NPInputCallback2D *callbacks);

/*
 destroy a NPWorld2D object
 call error callback if failed.
*/
NP_API void npWorld2DDestroy(NPWorld2D world);

/*
 update the simulation frame by frame
*/
NP_API void npWorld2DUpdate(NPWorld2D world, const NPReal dt);

/*
 the amount of subdivisions in delta time, making simulations
 a lot more accurate
 if it is more than 100 itbwill throw an input failed 
*/
NP_API void npWorld2DSetMaxSubIterations(NPWorld2D world, const NPUint32 max_num);

//-------------------------------------------------------------------//
/*
 create a NPBody2D object
 terminate if world2d is null
*/
NP_API NPBody2D npWorld2DCreateBody2D(NPWorld2D world, const NPPointer *components);

/*
 destroy a NPBody2D object
 if the body id is 0xFFFF, it will throw an input failed 
 terminate if world2d is null
*/
NP_API void npWorld2DDestroyBody2D(NPWorld2D world, NPBody2D body);

//-------------------------------------------------------------------//
/*
 create and allocate a new buffer
 call allocated failed if unsucessful
 create a copy of memory that can be used for shared objects
*/
NP_API NPReadonlyBuffer2D npReadonlyBuffer2DAllocate(const NPInputCallback2D *callbacks, const NPPointer data, const NPUint8 vertices_size);

/*
 destroy buffer 
 call deallocation failed if unsucessful
 make sure no other object uses the buffer or else
 it will call error with invalid stale pointer
*/
NP_API void npReadonlyBuffer2DDestroy(NPReadonlyBuffer2D buffer, const NPInputCallback2D *callbacks);

#endif

