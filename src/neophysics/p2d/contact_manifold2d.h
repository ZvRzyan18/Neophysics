#ifndef NP_CONTACT_MANIFOLD2D_H
#define NP_CONTACT_MANIFOLD2D_H

#include "neophysics/math.h"

typedef struct {
	NPReal2 normal;
	NPReal2 impulse;
	NPReal2 tangent;
	NPReal2 contact_points[2];
	NPUint8 contact_count;
	
	NPReal accumulated_impulse;
	NPReal accumulated_tangent;
} NPContactManifold2D;


NP_HOT
NP_INLINE void __NPContactManifold2DCombine(NPContactManifold2D *out, const NPContactManifold2D *a, const NPContactManifold2D *b) {
 out->normal = b->normal;
 out->impulse = b->impulse;
 out->tangent = b->tangent;
 out->contact_points[0] = b->contact_points[0];
 out->contact_points[1] = b->contact_points[1];
 out->contact_count = b->contact_count;
 out->accumulated_impulse = a->accumulated_impulse;
 out->accumulated_tangent = a->accumulated_tangent;
}

#endif

