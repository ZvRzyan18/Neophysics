#ifndef NP_CONTAINER_BODY2D_H
#define NP_CONTAINER_BODY2D_H

#include "neophysics/core2d.h"
#include "neophysics/p2d/body2d.h"

#include <string.h>

typedef struct {
	NPGenericBody2D *body_array;
	NPUint16        body_size; //10k+ objects is more than enough
	NPUint8         body_reserve; //8 byte is more than enough for reserve
} NPContainerBody2D;


/*
 PRIVATE USE, DO NOT USE THIS OUTSIDE OF HEADER
 set a reserve value to a body container
*/
NP_COLD
NP_INLINE void __NPContainerBodyReserve(NPContainerBody2D *container, NPInputCallback2D *callbacks, NPUint8 reserve) {
	if(NPUnlikely(container->body_array == NPNULL)) {
		NPGenericBody2D *allocated_array = callbacks->malloc(sizeof(NPGenericBody2D) * reserve);
		
		if(NPUnlikely(allocated_array == NPNULL)) {
			callbacks->error(NP_ALLOCATION_FAILED);
			return;
		}
		
		container->body_array = allocated_array;
		container->body_reserve = reserve;
	} else {
		NPGenericBody2D *allocated_array = callbacks->malloc(sizeof(NPGenericBody2D) * reserve);

		if(NPUnlikely(allocated_array == NPNULL)) {
			callbacks->error(NP_ALLOCATION_FAILED);
			return;
		}
		
		memset(allocated_array + container->body_size, 0xFF, sizeof(NPGenericBody2D) * reserve);
		memcpy(allocated_array, container->body_array, container->body_size * sizeof(NPGenericBody2D));
		callbacks->free(container->body_array);
		container->body_array = allocated_array;
		container->body_reserve = reserve;
		return;
	}
	NPUnreachable();
}

/*
 PRIVATE USE, DO NOT USE THIS OUTSIDE OF HEADER
 find the next empty slot
*/
NP_INLINE NPUint16 __NPContainerBodyFindNextEmptySlot(NPContainerBody2D *container) {
	NPUint16 index = 0;
	const NPUint16 max_iteration = container->body_size + container->body_reserve;
	while((index < max_iteration) && (container->body_array[index].type != 0xFFFF))
	 index++;
	return 0xFFFF;
}

/*
 add an object to a body container
 return the current index where object is stored
*/
NP_COLD
NP_INLINE NPUint16 __NPContainerBodyAddObject(NPContainerBody2D *container, NPInputCallback2D *callbacks, NPGenericBody2D obj) {
 if(NPUnlikely(container->body_reserve == 0))
  __NPContainerBodyReserve(container, callbacks, 16);
 
 NPUint16 empty_slot = __NPContainerBodyFindNextEmptySlot(container);
 container->body_array[empty_slot] = obj;
	return empty_slot;
}


/*
 remove an object to a body container
*/
NP_COLD
NP_INLINE void __NPContainerBodyRemoveObject(NPContainerBody2D *container, NPInputCallback2D *callbacks, NPUint16 id) {
	if(NPUnlikely(id >= (container->body_size + container->body_reserve))) {
	 callbacks->error(NP_OBJECT_NOT_FOUND);
	 return;
	}
	
	if(NPUnlikely(container->body_size == 0)) {
	 callbacks->error(NP_OBJECT_NOT_FOUND);
	 return;
	}
	
	container->body_array[id].type = 0xFFFF;
	container->body_size--;
	container->body_reserve++;
}

/*
 destroy the container
*/
NP_COLD
NP_INLINE void __NPContainerBodyDestroy(NPContainerBody2D *container, NPInputCallback2D *callbacks) {
	if(NPLikely(container->body_array != NPNULL)) 
	 callbacks->free(container->body_array);
}



#endif


