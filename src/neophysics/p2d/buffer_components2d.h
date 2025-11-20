#ifndef NP_BUFFER2D_H
#define NP_BUFFER2D_H

#include "neophysics/core2d.h"
#include "neophysics/math.h"

#include <string.h>


typedef struct {
 NPReal2 *vertices;
 NPUint16 refcount; //use to detect invalid pointers
 NPUint8 vertices_size; //vertices size / per NPReal2 object
} NPBufferComponents2D;


/*
 NOTE : PRIVATE USE ONLY
 re arrange position based on the center of mass of an object
 centralize the object position
 
 make sure after this process, it is ready to use.
*/
NP_COLD
NP_INLINE void __NPBufferComponents2DReArrangePositionCOM(NPReal2 *out_vertices, const NPReal2 *in_vertices, const NPUint8 vertices_size) {
 NPUint8 index = 0;
 NPReal2 center_of_mass;
 NPReal area = 0;
 
 memset(&center_of_mass, 0x00, sizeof(NPReal2));
 
 while(index < vertices_size) {
  const NPUint8 next_index = (index + 1) % vertices_size;
  const NPReal crossed_value = __NPCross2(in_vertices[index], in_vertices[next_index]);
  area += crossed_value;
  center_of_mass = __NPMulAdd2(in_vertices[index], in_vertices[next_index], __NPAsVector2(crossed_value));
  index++;
 }
 area *= (NPReal)0.5;
 
 const NPReal inv_size = (NPReal)1.0 / ((NPReal)6.0) * area;
 center_of_mass = __NPMul2(center_of_mass, __NPAsVector2(inv_size));
 
 index = 0;
 while(index < vertices_size) {
  out_vertices[index] = __NPSub2(in_vertices[index], center_of_mass);
  index++;
 }
}


/*
 allocate readonly buffer
*/
NP_COLD
NP_INLINE NPBufferComponents2D* __NPBufferComponents2DAllocate(const NPInputCallback2D *callbacks, const NPPointer data, const NPUint8 vertices_size) {
 NPBufferComponents2D *out = NPNULL;
 
 out = (NPBufferComponents2D*)callbacks->malloc(sizeof(NPBufferComponents2D));
 
 if(NPUnlikely(out == NPNULL)) {
  callbacks->error(NP_ALLOCATION_FAILED);
  return NPNULL;
 }
 memset(out, 0x00, sizeof(NPBufferComponents2D));
 
 out->vertices = (NPReal2*)callbacks->malloc(sizeof(NPReal2) * vertices_size);
 
 if(NPUnlikely(out->vertices == NPNULL)) {
  callbacks->error(NP_ALLOCATION_FAILED);
  return NPNULL;
 }
 
 __NPBufferComponents2DReArrangePositionCOM(out->vertices, (const NPReal2*)data, vertices_size);

 out->vertices_size = vertices_size;
 return out;
}

/*
 deallocate readonly buffer
*/
NP_COLD
NP_INLINE void __NPBufferComponents2DDestroy(NPBufferComponents2D *buffer, const NPInputCallback2D *callbacks) {
 if(NPUnlikely(buffer == NPNULL)) {
  callbacks->error(NP_DEALLOCATION_FAILED);
  return;
 }
 
 if(NPUnlikely(buffer->vertices == NPNULL)) {
  callbacks->error(NP_DEALLOCATION_FAILED);
  return;
 }
 
 if(NPUnlikely(buffer->refcount > 0)) {
  callbacks->error(NP_INVALID_STALE_POINTER);
  return;
 }

 callbacks->free(buffer->vertices);
 callbacks->free(buffer);
}


#endif

