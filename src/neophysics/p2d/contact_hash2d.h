#ifndef NP_CONTACT_HASH2D_H
#define NP_CONTACT_HASH2D_H

#include "neophysics/core2d.h"
#include "neophysics/compiler_hint.h"
#include "neophysics/p2d/contact_manifold2d.h"

#include <string.h>

/*
 hash table node components, 
 it uses chaining, but stored in an array 
 to minimize cache miss
*/
typedef struct {
	NPUint16            a, b;
	NPUint16            still_contact; //check if its still in contact, if not then remove
	NPUint32            next;
	NPContactManifold2D contact_info; //contact info
} NPContactHashNode2D;

/*
 hash table cache for contacts
*/
typedef struct {
	NPContactHashNode2D *nodes;
	NPUint32            nodes_size;
	NPUint8             nodes_reserve;
} NPContactHash2D;

/*
 PRIVATE USE, DO NOT USE THIS OUTSIDE OF HEADER
 reserve a memory for hash contact
 terminate if failed
*/
NP_HOT
NP_INLINE void __NPContactHashReserve(NPContactHash2D *table, NPInputCallback2D *callbacks, uint8_t max_reserve) {
	if(NPUnlikely(table->nodes == NPNULL)) {
		table->nodes = (NPContactHashNode2D*)callbacks->malloc(sizeof(NPContactHashNode2D) * max_reserve);
		
		if(NPUnlikely(table->nodes == NPNULL)) {
		 callbacks->error(NP_ALLOCATION_FAILED);
		 return;
		}
		
		memset(table->nodes, 0xFF, sizeof(NPContactHashNode2D) * max_reserve);
		table->nodes_reserve = max_reserve;
	} else {
		NPContactHashNode2D *nodes = (NPContactHashNode2D*)callbacks->malloc((table->nodes_size + max_reserve) * sizeof(NPContactHashNode2D));
		
		if(NPUnlikely(nodes == NPNULL)) {
			callbacks->error(NP_ALLOCATION_FAILED);
			return;
		}
		memset(nodes + table->nodes_size, 0xFF, sizeof(NPContactHashNode2D) * max_reserve);
		memcpy(nodes, table->nodes, table->nodes_size * sizeof(NPContactHashNode2D));
		callbacks->free(table->nodes);
		table->nodes = nodes;
		table->nodes_reserve = max_reserve;
		return;
	}
	NPUnreachable();
}

/*
 PRIVATE USE, DO NOT USE THIS OUTSIDE OF HEADER
 find the next empty slot, iterate the elements one by one
 though slow, still offer a fast iteration
*/
NP_HOT
NP_INLINE NPUint32 __NPContactHashFindNextEmptySlot(NPContactHash2D *table) {
	//O(N)
	NPContactHashNode2D *node = table->nodes + 8;
	const NPContactHashNode2D *end_nodes = (table->nodes + table->nodes_size + table->nodes_reserve);
	while((node <= end_nodes) && !(node->a == 0xFFFF && node->b == 0xFFFF))
	 node++;
	return (NPUint32)(node - table->nodes);
}

/*
 insert elements and use (a + b) & 7 to combine the contact pair 
 index into one unique id
*/
NP_HOT
NP_INLINE void __NPContactHashInsertElement(NPContactHash2D *table, NPInputCallback2D *callbacks, NPUint16 a, NPUint16 b, NPContactManifold2D *data) {
	NPContactHashNode2D node;
	node.a = a < b ? a : b;
	node.b = a < b ? b : a;
	node.contact_info = *data;
	node.next = 0xFFFFFFFF;
	node.still_contact = 0;
	
	const uint32_t index = (a + b) & 7;
	
	if(table->nodes_reserve == 0)
 	__NPContactHashReserve(table, callbacks, 16);
		
	NPContactHashNode2D *start_node = &table->nodes[index];
	
	if(NPUnlikely((start_node->a == 0xFFFF) && (start_node->b == 0xFFFF))) {
		*start_node = node;
	} else {
	 NPUint32 prev_index = index;
  NPUint32 next_index = index;
 	do {
	 	prev_index = next_index;
	 	if(NPUnlikely(start_node->next == 0xFFFFFFFF)) {
			 prev_index = next_index;
		 	next_index = __NPContactHashFindNextEmptySlot(table);
		 	table->nodes[next_index] = node;
		  table->nodes[prev_index].next = next_index;
	 	 break;
	 	}
		 next_index = start_node->next;
	 	start_node = &table->nodes[start_node->next];
 	} while(1);

	}
	
	table->nodes_reserve--;
	table->nodes_size++;
}

/*
 delete element from cache, more likely if not in contact
*/
NP_HOT
NP_INLINE void __NPContactHashDeleteElement(NPContactHash2D *table, NPUint16 a, NPUint16 b) {
	const NPUint32 index = (a + b) & 7;
	const NPUint16 ma = a < b ? a : b;
	const NPUint16 mb = a < b ? b : a;
	NPContactHashNode2D start_node = table->nodes[index];
	NPUint32 prev_index = index;
	NPUint32 next_index = index;
	do {
		prev_index = next_index;
		if(NPUnlikely((start_node.a == ma) && (start_node.b == mb))) {
			memset(&table->nodes[next_index], 0xFF, sizeof(NPContactHashNode2D));
		 table->nodes[prev_index].next = 0xFFFFFFFF;
		 break;
		}
		next_index = start_node.next;
		start_node = table->nodes[start_node.next];
	} while(1);
	table->nodes_reserve++;
	table->nodes_size--;
}

/*
 find the index of element base on the given pair of object indices
 return index for fast access
*/
NP_HOT
NP_INLINE NPUint32 __NPContactHashFindIndexOf(NPContactHash2D *table, NPUint16 a, NPUint16 b) {
	const NPUint32 index = (a + b) & 7;
	const NPUint16 ma = a < b ? a : b;
	const NPUint16 mb = a < b ? b : a;
	NPContactHashNode2D *start_node = &table->nodes[index];
	NPUint32 prev_index = index;
	NPUint32 next_index = index;
	do {
		prev_index = next_index;
		if(NPUnlikely(start_node->a == ma && start_node->b == mb))
		 break;
		next_index = start_node->next;
		start_node = &table->nodes[start_node->next];
	} while(1);
	return next_index;
}

/*
 destroy hash table contact
*/
NP_COLD
NP_INLINE void __NPContactHashDestroy(NPContactHash2D *table, NPInputCallback2D *callbacks) {
	if(NPLikely(table->nodes != NPNULL))
	 callbacks->free(table->nodes);
}

#endif

