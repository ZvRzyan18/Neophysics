#ifndef NP_CONTACT_MANIFOLD2D_H
#define NP_CONTACT_MANIFOLD2D_H

#include "neophysics/math.h"

typedef struct {
	NPReal2 normal;
	NPReal impulse;
	NPReal friction_impulse;
	NPReal2 contact_points[2];
	NPUint8 contact_count;
} NPContactManifold2D;

#endif

