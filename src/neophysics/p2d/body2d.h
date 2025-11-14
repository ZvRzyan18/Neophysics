#ifndef NP_BODY2D_H
#define NP_BODY2D_H

#include "neophysics/math.h"
#include "neophysics/core2d.h"
#include "neophysics/p2d/buffer_components2d.h"
#include "neophysics/p2d/area2d.h"

/*
 the first 2 byte is an identifier, if its value is 0xFFFF
 means the object is empty
*/

typedef struct {
	NPUint16 type; //first 8 byte = object type, second = movement type
	NPReal2  position;
	NPReal   rotation;
	NPReal   inv_mass;
	NPReal   rotational_inertia;
	NPReal   restitution;
	NPReal2  velocity;
	NPReal   omega;
	NPReal2  force;
	NPReal   torque;
	NPReal2  aabb_min;
	NPReal2  aabb_max;
} NPBody2DComponents;

/*
 polygon object
*/
typedef struct {
	NPUint16 type; //first 8 byte = object type, second = movement type
	NPReal2  position;
	NPReal   rotation;
	NPReal   inv_mass;
	NPReal   rotational_inertia;
	NPReal   restitution;
	NPReal2  velocity;
	NPReal   omega;
	NPReal2  force;
	NPReal   torque;
	NPReal2  aabb_min;
	NPReal2  aabb_max;
	
	NPBufferComponents2D *raw_vertices;
	NPReal2  *vertices;
	NPReal2  *normals;
} NPPolygonBody2D;

/*
 generic type of body2d that could be any body type
*/
typedef union {
	NPUint16           type;
	NPBody2DComponents components;
	NPPolygonBody2D    polygon;
} NPGenericBody2D;



/*
 general body2d functions
*/

/*
 simple integration
*/
NP_HOT
NP_INLINE void __NPGenericBody2DIntegrate(NPGenericBody2D *body, NPReal dt, NPReal2 gravity) {
 NPReal2 vec_dt = __NPAsVector2(dt);
 
 body->components.force = __NPMulAdd2(body->components.force, __NPAsVector2(body->components.inv_mass), gravity);
 body->components.torque *= body->components.rotational_inertia;
 
 body->components.velocity = __NPMulAdd2(body->components.force, vec_dt, body->components.velocity);
 body->components.omega += body->components.torque * dt;
 
 body->components.force = __NPAsVector2((NPReal)0.0);
 body->components.omega = (NPReal)0.0;
 
 body->components.position = __NPMulAdd2(body->components.velocity, vec_dt, body->components.position);
 body->components.rotation += body->components.omega * dt;
}



/*
 update the vertices and aabb transformation in polygon body2d.
 wrap it here to make it easy to use
*/
NP_HOT
NP_INLINE void __NPPolygonBody2DUpdateTransform(NPPolygonBody2D *body) {
	const NPReal2 normalized_direction = __NPSinCos(body->rotation);
	const NPUint8 max_size = body->raw_vertices->vertices_size;
	
	NPReal2 prev_vector = __NPRotate2(body->raw_vertices->vertices[0], normalized_direction);
	body->vertices[0] = __NPAdd2(prev_vector, body->position);

 body->aabb_min.x = __NPMin(body->aabb_min.x, body->vertices[0].x);
 body->aabb_min.y = __NPMin(body->aabb_min.y, body->vertices[0].y);
 body->aabb_max.x = __NPMax(body->aabb_max.x, body->vertices[0].x);
 body->aabb_max.y = __NPMax(body->aabb_max.y, body->vertices[0].y);

	for(NPUint8 i = 0; i < max_size; i++) {
	 
	 const NPUint8 next_index = (i + 1) % max_size;
	 const NPReal2 next_vector = __NPRotate2(body->raw_vertices->vertices[next_index], normalized_direction);
	 
	 body->vertices[next_index] = __NPAdd2(next_vector, body->position);
	 body->normals[i] = __NPNormalize2(__NPPerp2(__NPSub2(next_vector, prev_vector)));
	 
  body->aabb_min.x = __NPMin(body->aabb_min.x, body->vertices[next_index].x);
  body->aabb_min.y = __NPMin(body->aabb_min.y, body->vertices[next_index].y);
  body->aabb_max.x = __NPMax(body->aabb_max.x, body->vertices[next_index].x);
  body->aabb_max.y = __NPMax(body->aabb_max.y, body->vertices[next_index].y);

	 prev_vector = next_vector;
	}

 
}



/*
 cold function/oly called once
 initialize and pre calculate object components
*/
NP_COLD
NP_INLINE void __NPPolygonBody2DInit(NPPolygonBody2D *body, NPInputCallback2D *callbacks, NPPolygonBody2DComponents *components) {
	body->type = (components->body_type << 8) | components->movement_type;
	body->raw_vertices = (NPBufferComponents2D*)components->vertices_buffer;
	
	const NPUint32 bytes_size = body->raw_vertices->vertices_size * sizeof(NPReal2);
	body->vertices = callbacks->malloc(bytes_size << 1);
	body->normals = body->vertices + bytes_size;
	
	body->raw_vertices->refcount++;
	
	if(NPUnlikely(body->vertices == NPNULL)) {
	 callbacks->error(NP_ALLOCATION_FAILED);
	 return;
	}
	
	body->restitution = components->restitution;
	body->position.x = components->position[0];
	body->position.y = components->position[1];
	body->scale.x = components->scale[0];
	body->scale.y = components->scale[1];
	body->rotation = components->rotation;
	body->restitution = components->restitution;

 const	NPReal area = __NPPolygon2DArea(body->raw_vertices->vertices, body->raw_vertices->vertices_size, body->scale);
	body->inv_mass = (NPReal)1.0 / (components->density * area);

	if(NPUnlikely(body->scale.x == (NPReal)0.0 || body->scale.y == (NPReal)0.0))
	 callbacks->error(NP_ZERO_SCALE);
	
	__NPPolygonBody2DUpdateTransform(body);
}



/*
 hot and called frequently in a tight loops
 update function for polygon body2d
 to make it even faster, it should not perform any null checking
*/
NP_HOT
NP_INLINE void __NPPolygonBody2DUpdate(NPPolygonBody2D *body, NPReal dt, NPReal2 gravity) {
	__NPGenericBody2DIntegrate((NPGenericBody2D*)body, dt, gravity);
	__NPPolygonBody2DUpdateTransform(body);
}

/*
 destroy polygon2d 
 and deallocates its memory
*/
NP_COLD
NP_INLINE void __NPPolygon2DDestroy(NPPolygonBody2D *body, NPInputCallback2D *callbacks) {
 if(NPUnlikely(body->vertices == NPNULL)) {
  callbacks->error(NP_DEALLOCATION_FAILED);
  return;
 }
 //decrease reference count
 //to make sure to make sure no use after free
 //memory.
 body->raw_vertices->refcount--;
 callbacks->free(body->vertices);
}

#endif

