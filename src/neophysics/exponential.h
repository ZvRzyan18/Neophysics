#ifndef NP_EXPONENTIAL_H
#define NP_EXPONENTIAL_H

#include "neophysics/types.h"
#include "neophysics/compiler_hint.h"

//-------------------------------------------------------------------//
//exponential base 2 coeffs
//accurate enough gor at lest 2-3 decimal digits.

#define __EXP2_0 ((NPReal)(1.369766e-2))
#define __EXP2_1 ((NPReal)(5.169035e-2))
#define __EXP2_2 ((NPReal)(2.416384e-1))
#define __EXP2_3 ((NPReal)(6.929661e-1))
#define __EXP2_4 ((NPReal)(1.000003e-0))

/*

 
                exponential growth

                        /
                       |
                      /   
                     |
                    /
                ---*  <- point exactly to one
              __
    _______---      <- negative values are always below one

    exp2(x) = exp(x * ln(2))
*/

//TODO : NO BRANCHES IMPLEMENTATION, IF POSSIBLE
NP_HOT 
NP_INLINE NPReal __np_internal_exp2(NPReal x) {
	NPReal mx, a;
	NPRealBit bits;
	
	bits.f = x;
	
	a = x;
	//double precision
#if defined(NP_USE_FLOAT64)

 if(NPLikely(bits.i & 0x8000000000000000)) { /* x < 0.0f */
  bits.i &= 0x7FFFFFFFFFFFFFFF;
  a = bits.f;
  bits.i = ((NPUint64)(1023U + ((NPUint64)(x))) << 52U);
  a -= (NPUint64)x;

 	mx = __EXP2_0 * a + __EXP2_1;
	 mx = mx * a + __EXP2_2;
  mx = mx * a + __EXP2_3;
	 mx = mx * a + __EXP2_4;
  return 1.0 / (bits.f * mx);
 }
 bits.i = ((NPUint64)(1023U + ((NPUint64)(x))) << 52U);
 a -= (NPUint64)x;

	mx = __EXP2_0 * a + __EXP2_1;
	mx = mx * a + __EXP2_2;
 mx = mx * a + __EXP2_3;
	mx = mx * a + __EXP2_4;

 return bits.f * mx;

 //single precision
#else

 if(NPLikely(bits.i & 0x80000000)) {/* x < 0.0f */
  bits.i &= 0x7FFFFFFF;
  a = bits.f;
  bits.i = ((NPUint32)(127U + ((NPUint32)(x))) << 23U);
  a -= (NPUint32)x;

 	mx = __EXP2_0 * a + __EXP2_1;
 	mx = mx * a + __EXP2_2;
  mx = mx * a + __EXP2_3;
 	mx = mx * a + __EXP2_4;
  return 1.0f / (bits.f * mx);
 }
 bits.i = ((NPUint32)(127U + ((NPUint32)(x))) << 23U);
 a -= (NPUint32)x;

	mx = __EXP2_0 * a + __EXP2_1;
	mx = mx * a + __EXP2_2;
 mx = mx * a + __EXP2_3;
	mx = mx * a + __EXP2_4;

 return bits.f * mx;

#endif
}

#endif

