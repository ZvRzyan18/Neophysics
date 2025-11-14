#ifndef NP_WORLD2D_H
#define NP_WORLD2D_H

#include "neophysics/core2d.h"
#include "neophysics/math.h"
#include "neophysics/p2d/body2d.h"
#include "neophysics/p2d/contact_hash2d.h"
#include "neophysics/p2d/container_body2d.h"

#include <string.h>
/*
 actual world2d components
*/
typedef struct {
	NPReal            inv_substeps;
	NPReal2 gravity;
	NPInputCallback2D input_callbacks;
	NPContainerBody2D container_body;
	
	NPContactHash2D   contact_cache;
} NPWorldComponents2D;

/*
 initialize all internal components
 and allocate some reserve memory
*/
NP_COLD
NP_INLINE void __NPWorldComponents2DInit(NPWorldComponents2D *ctx) {
	//make sure it is completly initialized
	memset(ctx, 0, sizeof(NPWorldComponents2D));
}

/*
 deallocate all internal components
 call error callback if failed
*/
NP_COLD
NP_INLINE void __NPWorldComponents2DDestroy(NPWorldComponents2D * ctx) {
	__NPContactHashDestroy(&ctx->contact_cache, &ctx->input_callbacks);
 __NPContainerBodyDestroy(&ctx->container_body, &ctx->input_callbacks);
}

/*
 add new body in world2d simulation
 return its current index id
*/
NP_COLD
NP_INLINE NPUint16 __NPWorldComponents2DAddBody(NPWorldComponents2D * ctx, NPPointer ptr) {
 NPGenericBody2D body;
	const NPUint16 body_type = *(NPUint16*)ptr;
	
	memset(&body, 0x00, sizeof(NPGenericBody2D));
	
	switch(body_type) {
		case NP_POLYGON_2D:
		 __NPPolygonBody2DInit((NPPolygonBody2D*)&body, &ctx->input_callbacks, (NPPolygonBody2DComponents*)ptr);
		break; //NP_POLYGON_2D:
	}
	return __NPContainerBodyAddObject(&ctx->container_body, &ctx->input_callbacks, body);
}

/*
 delete body in world2d simulation
 call error NP_OBJECT_NOT_FOUND if the object does not exist 
*/
NP_COLD
NP_INLINE void __NPWorldComponents2DDestroyBody(NPWorldComponents2D * ctx, NPUint16 id) {
 //make sure that it has null pointer safety
 if(NPUnlikely(
  (ctx->container_body.body_array == NPNULL) || 
  ((ctx->container_body.body_size + ctx->container_body.body_reserve) <= id))
 ) {
  ctx->input_callbacks.error(NP_OBJECT_NOT_FOUND);
  return;
 }

/*
 deallocate a memory and set the object's type to 0xFFFF
 to identify it as empty/unocupied memory.
*/
	const NPUint8 body_type = *(NPUint8*)&ctx->container_body.body_array[id];
 switch(body_type) {
 	case NP_POLYGON_2D:
   //handle possible errors in memory deallocation
   __NPPolygon2DDestroy((NPPolygonBody2D*)&ctx->container_body.body_array[id], &ctx->input_callbacks);
 	break; //NP_POLYGON_2D:
 }
 __NPContainerBodyRemoveObject(&ctx->container_body, &ctx->input_callbacks, id);
}



#endif

