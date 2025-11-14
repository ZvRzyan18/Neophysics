#ifndef NP_MATH_H
#define NP_MATH_H

#include "neophysics/types.h"
#include "neophysics/core2d.h"
#include "neophysics/compiler_hint.h"
#include "neophysics/trigonometry.h"
#include "neophysics/exponential.h"
#include "neophysics/logarithm.h"
#include "neophysics/decay.h"
#include <math.h>

typedef struct {
	NPReal x, y;
} NPReal2;


/*
 scalar math functions
*/

NP_HOT 
NP_INLINE NPReal __NPAbs(NPReal a) {
 return a < (NPReal)0.0 ? -a : a;
}


NP_HOT 
NP_INLINE NPReal __NPMin(NPReal a, NPReal b) {
 return a < b ? a : b;
}


NP_HOT 
NP_INLINE NPReal __NPMax(NPReal a, NPReal b) {
 return a < b ? b : a;
}


NP_HOT
NP_INLINE NPReal __NPClamp(NPReal x, NPReal __min_value, NPReal __max_value) {
 return __NPMin(__NPMax(x, __min_value), __max_value);
}

/*
 used for linear and angular damping, to consistenly 
 reduce a value per percenrage consistently.
*/
NP_HOT 
NP_INLINE NPReal __NPDecay(NPUint8 type, NPReal x, NPReal decay_const, NPReal time) {
 switch(type) {
  case NP_DAMPING_EXPONENITAL_DECAY_2D:
  //exponential decay based on exponential base 2
   return __np_internal_exponential_decay(x, decay_const, time);
  break;
  case NP_DAMPING_LINEAR_DECAY_2D:
   return __np_internal_linear_decay(x, decay_const, time);
  break;
 }
 return (NPReal)0.0;
}


NP_HOT 
NP_INLINE 
NPReal __NPSqrt(NPReal x) {
	//TODO : use bit hacks
	return sqrtf(x);
}


NP_HOT 
NP_INLINE 
NPReal __NPInvSqrt(NPReal x) {
	//TODO : use bit hacks
	return (NPReal)1.0 / sqrtf(x);
}

/*
 sincos
 
 compute sine and cosine at the same time
 with single range reduction and single remainder 
 operation.
*/
NP_HOT 
NP_INLINE 
NPReal2 __NPSinCos(NPReal x) {
	NPReal2 out;
	
	//TODO : use bit checking instead.
	//infinity/nan
	if(NPUnlikely( (((NPReal)1.0 / (NPReal)0.0) ) == x || x != x )) {
	 out.x = x;
	 out.y = x;
	 return out;
	}
	
	//doing some adjustment before actual calculation
	//handle signs and large value
	const NPUint8 is_negative = x < ((NPReal)0.0);
	const NPReal absolute_value = is_negative ? -x : x;
	const NPUint32 whole_range = (NPUint32)(absolute_value * ((NPReal)6.2831853));
	const NPReal range_x = absolute_value - ((NPReal)whole_range);
	//rough approximation of sin and cos
	__np_internal_sincos(range_x, is_negative, &out.y, &out.x);
	return out;
}


NP_HOT
NP_INLINE
NPReal __NPExp2(NPReal x) {
	//TODO : use bit checking instead.
	//infinity/nan
	if(NPUnlikely( (((NPReal)1.0 / (NPReal)0.0) ) == x || x != x ))
  return x;
 
 //return infinity
 //handle overflow, and then round them to infinity
 if(NPUnlikely(x > (NPReal)127.0))
  return (NPReal)1.0 / (NPReal)0.0;
 
 //approximation of exponential base 2
	return __np_internal_exp2(x);
}


NP_HOT
NP_INLINE
NPReal __NPLog2f(NPReal x) {
	//TODO : use bit checking instead.
	//infinity/nan
	if(NPUnlikely( (((NPReal)1.0 / (NPReal)0.0) ) == x || x != x ))
  return x;
  
 //return nan if x is negative (undefined)
 if(NPUnlikely(x < (NPReal)0.0))
  return (NPReal)0.0 / (NPReal)0.0;
 
 //approximation of logarithm base 2
	return __np_internal_log2(x);
}

/*
 vector functions
*/

/*
 perpendicular vector,
 return the vector, that forms a 90 degree from 
 an input vector
*/
NP_HOT
NP_INLINE
NPReal2 __NPPerp2(NPReal2 v) {
	NPReal2 out;
	out.x = -v.y;
	out.y = v.x;
	return out;
}


NP_HOT
NP_INLINE
NPReal2 __NPAbs2(NPReal2 v) {
	NPReal2 out;
	out.x = __NPAbs(v.x);
	out.y = __NPAbs(v.y);
	return out;
}


NP_HOT
NP_INLINE
NPReal __NPLengthSquared2(NPReal2 v) {
	return v.x * v.x + v.y * v.y;
}

NP_HOT
NP_INLINE
NPReal2 __NPNormalize2(NPReal2 v) {
	const NPReal inv_length = __NPInvSqrt(__NPLengthSquared2(v));
	NPReal2 out;
	out.x = v.x * inv_length;
	out.y = v.y * inv_length;
	return out;
}

NP_HOT
NP_INLINE
NPReal2 __NPRotate2(NPReal2 v, NPReal2 normalized) {
	NPReal2 out;
	out.x = v.x * normalized.x - v.y * normalized.y;
	out.y = v.x * normalized.y + v.y * normalized.x;
	return out;
}


NP_HOT
NP_INLINE
NPReal2 __NPAsVector2(NPReal x) {
	NPReal2 out;
	out.x = x;
	out.y = x;
	return out;
}


NP_HOT
NP_INLINE
NPReal2 __NPAdd2(NPReal2 a, NPReal2 b) {
	NPReal2 out;
	out.x = a.x + b.x;
	out.y = a.y + b.y;
	return out;
}


NP_HOT
NP_INLINE
NPReal2 __NPSub2(NPReal2 a, NPReal2 b) {
	NPReal2 out;
	out.x = a.x - b.x;
	out.y = a.y - b.y;
	return out;
}


NP_HOT
NP_INLINE
NPReal2 __NPMul2(NPReal2 a, NPReal2 b) {
	NPReal2 out;
	out.x = a.x * b.x;
	out.y = a.y * b.y;
	return out;
}

/*
 this can be implemented as multiply accumulate 
 or fused multiply addition if specific instruction is
 available.
*/
NP_HOT
NP_INLINE
NPReal2 __NPMulAdd2(NPReal2 a, NPReal2 b, NPReal2 c) {
	NPReal2 out;
	out.x = a.x * b.x + c.x;
	out.y = a.y * b.y + c.y;
	return out;
}


#endif


