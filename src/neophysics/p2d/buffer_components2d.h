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
 allocate readonly buffer
*/
NP_COLD
NP_INLINE NPBufferComponents2D* __NPBufferComponents2DAllocate(NPInputCallback2D *callbacks, NPPointer data, NPUint8 vertices_size) {
 NPBufferComponents2D *out = NPNULL;
 
 out = (NPBufferComponents2D*)callbacks->malloc(sizeof(NPBufferComponents2D));
 
 if(NPUnlikely(out == NPNULL)) {
  callbacks->error(NP_ALLOCATION_FAILED);
  return NPNULL;
 }
 memset(out, 0x00, sizeof(NPBufferComponents2D));
 out->vertices = (NPReal2*)callbacks->malloc(sizeof(NPReal2) * vertices_size);
 memcpy(out->vertices, data, sizeof(NPReal2) * vertices_size);
 
 if(NPUnlikely(out->vertices == NPNULL)) {
  callbacks->error(NP_ALLOCATION_FAILED);
  return NPNULL;
 }
 
 out->vertices_size = vertices_size;
 return out;
}

/*
 deallocate readonly buffer
*/
NP_COLD
NP_INLINE void __NPBufferComponents2DDestroy(NPBufferComponents2D *buffer, NPInputCallback2D *callbacks) {
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

