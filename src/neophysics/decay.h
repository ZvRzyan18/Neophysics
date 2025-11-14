#ifndef NP_DECAY_H
#define NP_DECAY_H

#include "neophysics/exponential.h"
#include "neophysics/logarithm.h"

/*
 use for angular and linear damping

 multiplying by the damping factor is not consistent and may
 vary depends on framerate,
 exponential decay is used to reduce the number by a percentage rate
 ((0.0 - 1.0) or a value/range of damping) over a period of time consistently.

*/
NP_HOT 
NP_INLINE NPReal __np_internal_exponential_decay(NPReal x, NPReal k, NPReal t) {
 //1.4426950408 = log2(e)
 //actual formula : x * e ^ kt
 //expressed as : x * 2 ^ (kt * log2(e))
 return x * __np_internal_exp2(k * t * (NPReal)(1.4426950408));
}


NP_HOT 
NP_INLINE NPReal __np_internal_linear_decay(NPReal x, NPReal k, NPReal t) {
 const NPReal linear = x * (k * t);
 const NPReal min_linear = linear < (NPReal)0.0 ? (NPReal)0.0 : linear;
 return min_linear;
}



#endif

