#ifndef NP_QUAT_H
#define NP_QUAT_H

#include "neophysics/real4.h"
#include "neophysics/real3.h"

#ifdef __cplusplus
extern "C" {
#endif

extern const NPReal4 NPQuat_Identity;
NP_HOT NPReal4 NPQuat_Mul(NPReal4 a, NPReal4 b);
NP_HOT NPReal4 NPQuat_FromEulerAngle(const np_real roll, const np_real pitch, const np_real yaw);
NP_HOT NPReal3 NPQuat_ToEulerAngle(NPReal4 q);
NP_HOT NPReal4 NPQuat_FromAxisAngle(const np_real r, const np_real x, const np_real y, const np_real z);
NP_HOT NPReal4 NPQuat_ToAxisAngle(NPReal4 x);
NP_HOT NPReal4 NPQuat_Lerp(NPReal4 a, NPReal4 b, const np_real t);
NP_HOT NPReal4 NPQuat_Slerp(NPReal4 a, NPReal4 b, const np_real t);
NP_HOT NPReal4 NPQuat_Invert(NPReal4 a);
NP_HOT NPReal4 NPQuat_Conjugate(NPReal4 a);

#ifdef __cplusplus
}
#endif

#endif

