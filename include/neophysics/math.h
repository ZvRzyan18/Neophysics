#ifndef NP_MATH_H
#define NP_MATH_H

#include "neophysics/types.h"

#ifdef __cplusplus
extern "C" {
#endif


#ifdef NP_USE_F64
#define NP_SIGNIFICAND  52
#define NP_BIAS         1023
#define NP_SIGN_BIT     0x8000000000000000
#define NP_INV_SIGN_BIT 0x7FFFFFFFFFFFFFFF
#define NP_NUM_BITS_M1  63
#else
#define NP_SIGNIFICAND  23
#define NP_BIAS         127
#define NP_SIGN_BIT     0x80000000
#define NP_INV_SIGN_BIT 0x7FFFFFFF
#define NP_NUM_BITS_M1  31
#endif

typedef union {
	np_real f;
 np_urb i;
 np_srb s;
} NPRealBit;


/*
 manually using float representation of inf, nan
*/
NP_HOT static inline np_real NP_Nan(void) {
 NPRealBit bits;
#ifdef NP_USE_F64
 bits.i = 0x7FF0000000000001;
#else
 bits.i = 0x7F800001;
#endif
 return bits.f;
}



NP_HOT static inline np_real NP_Infinity(void) {
 NPRealBit bits;
#ifdef NP_USE_F64
 bits.i = 0x7FF0000000000000;
#else
 bits.i = 0x7F800000;
#endif
 return bits.f;
}


/*
 common math functions
*/

NP_HOT static inline np_real NP_Abs(np_real x) {
 NPRealBit bits;
 bits.f = x;
 bits.i &= NP_INV_SIGN_BIT;
 return bits.f;
}


NP_HOT static inline np_real NP_Min(np_real a, np_real b) {
 return a < b ? a : b;
}

NP_HOT static inline np_real NP_Max(np_real a, np_real b) {
 return a > b ? a : b;
}

NP_HOT static inline np_real NP_Clamp(np_real x, np_real _min, np_real _max) {
 return NP_Max(NP_Min(x, _max), _min);
}

NP_HOT static inline np_real NP_WrapAngle(np_real x) {
 np_real mx;
 np_urb sign;
 sign = x < 0.0f;
 mx = NP_Abs(x);
 mx = mx - (np_real)((np_urb)(mx * 0.15915494f)) * 6.28318530f;
 return sign ? -mx : mx;
}


NP_HOT np_real NP_Sin(np_real x);
NP_HOT np_real NP_Cos(np_real x);
NP_HOT void NP_SinCos(np_real x, np_real *s, np_real *c);
NP_HOT np_real NP_Tan(np_real x);
NP_HOT np_real NP_Asin(np_real x);
NP_HOT np_real NP_Acos(np_real x);
NP_HOT np_real NP_Atan(np_real x);
NP_HOT np_real NP_Atan2(np_real y, np_real x);

NP_HOT np_real NP_Log2(np_real x);
NP_HOT np_real NP_Exp2(np_real x);
NP_HOT np_real NP_Sqrt(np_real x);
NP_HOT np_real NP_InvSqrt(np_real x);

#ifdef __cplusplus
}
#endif

#endif


