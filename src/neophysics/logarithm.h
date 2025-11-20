#ifndef NP_LOGARITHM_H
#define NP_LOGARITHM_H

#include "neophysics/types.h"
#include "neophysics/compiler_hint.h"

//-------------------------------------------------------------------//
//log base 2 coeffs
//accurate enough gor at lest 2-3 decimal digits.

#define __LOG2_0 ((NPReal)(-8.161580e-2))
#define __LOG2_1 ((NPReal)( 6.451423e-1))
#define __LOG2_2 ((NPReal)(-2.120675e-0))
#define __LOG2_3 ((NPReal)( 4.070090e-0))
#define __LOG2_4 ((NPReal)(-2.512854e-0))


/*

 
                                        ______
                                    __ -
  logarithmic growth ->         __ -
                            _ -
                           /
                          -
                         /
    udefined negatives  |
      -------|-------|--}* -> point exactly to one
                       /
                       | -> below 1.0 are negatives
                       /
   
-> gradually goes to zero with negative sign


  log2(x) = ln(x) / ln(2)
 
*/
//TODO : NO BRANCHES IMPLEMENTATION, IF POSSIBLE
NP_HOT 
NP_INLINE NPReal __np_internal_log2(NPReal x) {
 NPRealBit mantissa, bits;
 NPReal ma, mx, a;
 
 bits.f = x;
 
 //double precision
#if defined(NP_USE_FLOAT64)

 if(NPLikely((bits.i >> 52U) < 1023U)) { /* x < 1.0 */
 //below 1.0 are just reciprocal and negative log2(x) = -log2(1/x)
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
 //normal calculation
 mantissa.i = 4607182418800017408U | (bits.i & 0x000FFFFFFFFFFFFF);
 ma = mantissa.f;

 mx = __LOG2_0 * ma + __LOG2_1;
	mx = mx * ma + __LOG2_2;
 mx = mx * ma + __LOG2_3;
	mx = mx * ma + __LOG2_4;

 return ((bits.i >> 52U)-1023U) + mx;

 //single precision
#else

 if(NPLikely((bits.i >> 23U) < 127U)) { /* x < 1.0 */
 //below 1.0 are just reciprocal and negative log2(x) = -log2(1/x)
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
 //normal calculation
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

