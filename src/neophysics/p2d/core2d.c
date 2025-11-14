#include "neophysics/core2d.h"
#include "neophysics/p2d/world_components2d.h"
#include "neophysics/p2d/buffer_components2d.h"

#include <string.h>

/*
 create a NPWorld2D object.
 returns null if failed.
*/
NP_API NPWorld2D npWorld2DCreate(NPInputCallback2D *callbacks) {
	if(NPUnlikely(callbacks == NPNULL))
	 return NPNULL;
	
	if(NPUnlikely( (callbacks->malloc == NPNULL) || (callbacks->free == NPNULL) || (callbacks->error == NPNULL) ))
	 return NPNULL;
	
	NPWorldComponents2D *world_components = callbacks->malloc(sizeof(NPWorldComponents2D));
 __NPWorldComponents2DInit(world_components);
	world_components->input_callbacks = *callbacks;
	return (NPWorld2D)world_components;
}

/*
 destroy a NPWorld2D object
 call error callback if failed.
*/
NP_API void npWorld2DDestroy(NPWorld2D world) {
	if(NPUnlikely(world == NPNULL))
	 return;
	
	NPWorldComponents2D *world_components = (NPWorldComponents2D*)world;
	NPInputCallback2D callback = world_components->input_callbacks;
	__NPWorldComponents2DDestroy(world_components);
	callback.free(world_components);
}

/*
 the amount of subdivisions in delta time, making simulations
 a lot more accurate
 if it is more than 100 itbwill throw an input failed 
*/
NP_API void npWorld2DSetMaxSubIterations(NPWorld2D world, NPUint32 max_num) {
	NPWorldComponents2D *world_components = (NPWorldComponents2D*)world;

 //handle null pointer and too large input
	if(NPUnlikely(world == NPNULL))
	 return;
	if(NPUnlikely(max_num > 100)) {
		world_components->input_callbacks.error(NP_INPUT_FAILED);
		return;
	}
 world_components->inv_substeps = ((NPReal)1.0) / ((NPReal)max_num);
}

/*
 create a NPBody2D object
 terminate if world2d is null
*/
NP_API NPBody2D npWorld2DCreateBody2D(NPWorld2D world, NPPointer *components) {
	//handle null pointer
	if(NPUnlikely(world == NPNULL))
	 return 0xFFFF;
	return __NPWorldComponents2DAddBody((NPWorldComponents2D*)world, components);
}

/*
 destroy a NPBody2D object
 if the body id is 0xFFFF, it will throw an input failed 
 terminate if world2d is null
*/
NP_API void npWorld2DDestroyBody2D(NPWorld2D world, NPBody2D body) {
	NPWorldComponents2D *world_components = (NPWorldComponents2D*)world;
 //handle null pointer
 if(NPUnlikely(world == NPNULL))
	 return;
	if(NPUnlikely(body == 0xFFFF)) {
		world_components->input_callbacks.error(NP_INPUT_FAILED);
  return;
	}
	__NPWorldComponents2DDestroyBody((NPWorldComponents2D*)world, body);
}


/*
 create and allocate a new buffer
 call allocated failed if unsucessful
*/
NP_API NPReadonlyBuffer2D npReadonlyBuffer2DAllocate(NPInputCallback2D *callbacks, NPPointer data, NPUint8 vertices_size) {
 //callback validation
	if(NPUnlikely( (callbacks->malloc == NPNULL) || (callbacks->free == NPNULL) || (callbacks->error == NPNULL) ))
	 return NPNULL;
 return (NPReadonlyBuffer2D)__NPBufferComponents2DAllocate(callbacks, data, vertices_size);
}

/*
 destroy buffer 
 call deallocation failed if unsucessful
*/
NP_API void npReadonlyBuffer2DDestroy(NPReadonlyBuffer2D buffer, NPInputCallback2D *callbacks) {
 //callback validation
	if(NPUnlikely( (callbacks->malloc == NPNULL) || (callbacks->free == NPNULL) || (callbacks->error == NPNULL) ))
	 return;
 __NPBufferComponents2DDestroy((NPBufferComponents2D*)buffer, callbacks);
}


