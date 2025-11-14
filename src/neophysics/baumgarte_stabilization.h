#ifndef NP_BAUMGARTE_STABILIZATION
#define NP_BAUMGARTE_STABILIZATION

#include "neophysics/types.h"
#include "neophysics/compiler_hint.h"

/*
 baumgrate stabilization
 
 small correction for object separation
 of two contact bodies.
*/
NP_HOT
NP_INLINE NPReal __np_internal_baumgarte_penetration(NPReal coeffs_beta, NPReal penetration, NPReal inv_delta_time) {
 const NPReal neg_pen = -penetration;
 const NPReal min_pen = neg_pen < (NPReal)0.0 ? neg_pen : (NPReal)0.0;
 const NPReal bias = -coeffs_beta * min_pen * inv_delta_time;
 return bias;
}




#endif

