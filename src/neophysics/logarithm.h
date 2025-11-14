#ifndef NP_LOGARITHM_H
#define NP_LOGARITHM_H

#include "neophysics/types.h"
#include "neophysics/compiler_hint.h"

#define __LOG2_0 ((NPReal)(-8.161580e-2))
#define __LOG2_1 ((NPReal)( 6.451423e-1))
#define __LOG2_2 ((NPReal)(-2.120675e-0))
#define __LOG2_3 ((NPReal)( 4.070090e-0))
#define __LOG2_4 ((NPReal)(-2.512854e-0))



NP_HOT 
NP_INLINE NPReal __np_internal_log2(NPReal x) {
 NPRealBit mantissa, bits;
 NPReal ma, mx, a;
 
 bits.f = x;
 
#if defined(NP_USE_FLOAT64)

 if(NPLikely((bits.i >> 52U) < 1023U)) { /* x < 1.0 */
  a = 1.0 / x;
  bits.f = a;
  mantissa.i = 4607182418800017408U | (bits.i & 0x000FFFFFFFFFFFFF);
  ma = mantissa.f;

  mx = __LOG2_0 * ma + __LOG2_1;
 	mx = mx * ma + __LOG2_2;
  mx = mx * ma + __LOG2_3;
 	mx = mx * ma + __LOG2_4;
 
  return -(((bits.i >> 52U)-1023U) + mx);
 }
 mantissa.i = 4607182418800017408U | (bits.i & 0x000FFFFFFFFFFFFF);
 ma = mantissa.f;

 mx = __LOG2_0 * ma + __LOG2_1;
	mx = mx * ma + __LOG2_2;
 mx = mx * ma + __LOG2_3;
	mx = mx * ma + __LOG2_4;

 return ((bits.i >> 52U)-1023U) + mx;

#else

 if(NPLikely((bits.i >> 23U) < 127U)) { /* x < 1.0 */
  a = 1.0f / x;
  bits.f = a;
  mantissa.i = 1065353216U | (bits.i & 0x007FFFFF);
  ma = mantissa.f;

  mx = __LOG2_0 * ma + __LOG2_1;
 	mx = mx * ma + __LOG2_2;
  mx = mx * ma + __LOG2_3;
	 mx = mx * ma + __LOG2_4;

  return -(((bits.i >> 23U)-127U) + mx);
 }
 mantissa.i = 1065353216U | (bits.i & 0x007FFFFF);
 ma = mantissa.f;

 mx = __LOG2_0 * ma + __LOG2_1;
	mx = mx * ma + __LOG2_2;
 mx = mx * ma + __LOG2_3;
	mx = mx * ma + __LOG2_4;
 
 return ((bits.i >> 23U)-127U) + mx;

#endif
}

#endif

