#ifndef NP_BRADPHASE2D_H
#define NP_BRADPHASE2D_H

#include "neophysics/core2d.h"
#include "neophysics/math.h"
#include "neophysics/p2d/contact_hash2d.h"
#include "neophysics/p2d/container_body2d.h"

#include <string.h>


//--------------------------------------------------//
/*
 NP_BROADPHASE_BRUITE_FORCE_2D
*/
typedef struct {
 NPUint8 type;
} NPBroadphaseBruiteForce2D;


/*
 add object...
 just place it here
*/
NP_HOT
NP_INLINE void __NPBroadphaseBruiteForce2DAddObject(NPBroadphaseBruiteForce2D *obj, const NPInputCallback2D *callbacks, NPUint16 id) {
 //do nothing
}


/*
 remove object...
 just place it here
*/
NP_HOT
NP_INLINE void __NPBroadphaseBruiteForce2DRemoveObject(NPBroadphaseBruiteForce2D *obj, const NPInputCallback2D *callbacks, NPUint16 id) {
 //do nothing
}


/*
 update the object contact hash
*/
NP_HOT
NP_INLINE void __NPBroadphaseBruiteForce2DUpdate(NPBroadphaseBruiteForce2D *obj, NPContactHash2D *hash, const NPInputCallback2D *callbacks, NPContainerBody2D *container_body) {
 for(NPUint16 i = 0; i < container_body->body_size; i++) {
  for(NPUint16 j = 0; j < container_body->body_size; i++) {
   
   //skip same objects
   if(i == j) 
    continue;
    
   if(container_body->body_array[i].type != 0xFFFF && container_body->body_array[j].type != 0xFFFF) {
    //both valid objects, narrowphase here
    const NPUint16 a = i;
    const NPUint16 b = j;
    const NPUint32 index = __NPContactHashFindIndexOf(hash, a, b);
    if(index == 0xFFFFFFFF)
     __NPContactHashInsertElement(hash, callbacks, a, b, NPNULL);
   }
   
  }
 }
}


/*
 destroy all allocated object
 just place it here
*/
NP_COLD
NP_INLINE void __NPBroadphaseBruiteForce2DDestroy(NPBroadphaseBruiteForce2D *obj, const NPInputCallback2D *callbacks) {
 //do nothing
}



//--------------------------------------------------//
/*
 broadphase generic here
*/
typedef union {
 NPUint8 type;
 NPBroadphaseBruiteForce2D bruite_force;
} NPBroadphase2D;


/*
 add object...
*/
NP_HOT
NP_INLINE void __NPBroadphase2DAddObject(NPBroadphase2D *obj, const NPInputCallback2D *callbacks, NPUint16 id) {
 switch(obj->type) {
  case NP_BROADPHASE_BRUITE_FORCE_2D:
   __NPBroadphaseBruiteForce2DAddObject(&obj->bruite_force, callbacks, id);
  break;
 }
}


/*
 remove object...
*/
NP_HOT
NP_INLINE void __NPBroadphase2DRemoveObject(NPBroadphase2D *obj, const NPInputCallback2D *callbacks, NPUint16 id) {
 switch(obj->type) {
  case NP_BROADPHASE_BRUITE_FORCE_2D:
   __NPBroadphaseBruiteForce2DRemoveObject(&obj->bruite_force, callbacks, id);
  break;
 }
}


/*
 update the object contact hash
*/
NP_HOT
NP_INLINE void __NPBroadphase2DUpdate(NPBroadphase2D *obj, NPContactHash2D *hash, const NPInputCallback2D *callbacks, NPContainerBody2D *container_body) {
 switch(obj->type) {
  case NP_BROADPHASE_BRUITE_FORCE_2D:
   __NPBroadphaseBruiteForce2DUpdate(&obj->bruite_force, hash, callbacks, container_body);
  break;
 }
}


/*
 destroy all allocated object
*/
NP_COLD
NP_INLINE void __NPBroadphase2DDestroy(NPBroadphase2D *obj, const NPInputCallback2D *callbacks) {
 switch(obj->type) {
  case NP_BROADPHASE_BRUITE_FORCE_2D:
   __NPBroadphaseBruiteForce2DDestroy(&obj->bruite_force, callbacks);
  break;
 }
}



#endif

