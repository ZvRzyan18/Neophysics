#include "neophysics/math.h"
#include <stdalign.h>
/*
 branchless design functions and easy to vectorized.
*/
/*-------------------------------------------------------------------*/
/*
 coeffs polynomial with lowest degree
 just acceptable enough for rough estimation
*/
#define __SIN_0 ((np_real)( 0.008028e-00))
#define __SIN_1 ((np_real)(-0.166607e-00))

NP_HOT np_real NP_Sin(np_real x) {
 NP_ALIGNED np_real mx;
 NP_ALIGNED np_real x2_s;
 NP_ALIGNED np_real out_s;
 NP_ALIGNED np_real a;
 NP_ALIGNED np_urb sign_s;
 NP_ALIGNED np_urb flip_s;
 NP_ALIGNED np_urb q;
 
 sign_s = x < 0.0f;
 a =      NP_Abs(x);
 a =      a - (np_real)((np_urb)(a * 0.15915494f)) * 6.28318530f;
 q =      ((np_urb)(a * (np_real)(0.6366197f)));
 flip_s = (q == 3 | q == 1);
 sign_s = sign_s ^ (q == 2 | q == 3);
 mx =     a;
 mx =     mx - (1.57079632f * (np_real)(q));
 mx =     flip_s ? ((np_real)1.57079632f - mx) : mx;
 x2_s =   mx * mx;
 out_s =  __SIN_0 * x2_s + __SIN_1;
 out_s =  out_s * (x2_s * mx) + mx;
 return   sign_s ? -out_s : out_s;
}


NP_HOT np_real NP_Cos(np_real x) {
 NP_ALIGNED np_real mx;
 NP_ALIGNED np_real x2_c;
 NP_ALIGNED np_real out_c;
 NP_ALIGNED np_real a;
 NP_ALIGNED np_urb sign_c;
 NP_ALIGNED np_urb flip_c;
 NP_ALIGNED np_urb q;
 
 a =      NP_Abs(x);
 a =      a - (np_real)((np_urb)(a * 0.15915494f)) * 6.28318530f;
 q =      ((np_urb)(a * (np_real)(0.6366197f)));
 flip_c = (q == 0 | q == 2);
 sign_c = (q == 1 | q == 2);
 mx =     a;
 mx =     mx - (1.57079632f * (np_real)(q));
 mx =     flip_c ? ((np_real)1.57079632f - mx) : mx;
 x2_c = mx * mx;
 out_c =  __SIN_0 * x2_c + __SIN_1;
 out_c =  out_c * (x2_c * mx) + mx;
 return   sign_c ? -out_c : out_c;
}


/*

               point in unit circle
               *
              / |
             /  |
  radius -> /   |
           /    | -> sin
          /     |
         / |    |
 origin *------ *
       angle  | -> cos

  cos(theta) = x-axis of theta angle
  sin(theta) = y-axis of theta angle
*/ 
NP_HOT void NP_SinCos(np_real x, np_real *s, np_real *c) {
 NP_ALIGNED np_real mx;
 NP_ALIGNED np_real mx_s;
 NP_ALIGNED np_real mx_c;
 NP_ALIGNED np_real x2_c;
 NP_ALIGNED np_real x2_s;
 NP_ALIGNED np_real out_s;
 NP_ALIGNED np_real out_c;
 NP_ALIGNED np_real a;
 NP_ALIGNED np_urb sign_s;
 NP_ALIGNED np_urb sign_c;
 NP_ALIGNED np_urb flip_s;
 NP_ALIGNED np_urb flip_c;
 NP_ALIGNED np_urb q;
 
 sign_s = x < 0.0;
 a =      NP_Abs(x);
 a =      a - (np_real)((np_urb)(a * 0.15915494f)) * 6.28318530f;
 q =      ((np_urb)(a * (np_real)(0.6366197f)));
 mx =     a;
 mx =     mx - (1.57079632f * (np_real)(q));

 flip_s = (q == 1 | q == 3);
 flip_c = (q == 0 | q == 2);
 sign_c = (q == 1 | q == 2);

 sign_s = sign_s ^ (q == 2 | q == 3);

 mx_c =   mx;
 mx_s =   mx;

 mx_s =   flip_s ? ((np_real)1.57079632f - mx_s) : mx_s;
 mx_c =   flip_c ? ((np_real)1.57079632f - mx_c) : mx_c;

 x2_s =   mx_s * mx_s;
 x2_c =   mx_c * mx_c;

 out_s =  __SIN_0 * x2_s + __SIN_1;
 out_c =  __SIN_0 * x2_c + __SIN_1;
 
 out_s =  out_s * (x2_s * mx_s) + mx_s;
 out_c =  out_c * (x2_c * mx_c) + mx_c;

 out_s =  sign_s ? -out_s : out_s;
 out_c =  sign_c ? -out_c : out_c;

 *s =     out_s;
 *c =     out_c;
}



NP_HOT np_real NP_Tan(np_real x) {
 NP_ALIGNED	np_real s;
 NP_ALIGNED np_real c;
 NP_SinCos(x, &s, &c);
 return s / c;
}

/*
 arc sine
          • (x, y) = normalized value
         /|
        / |
       /  |
      /   |
     /__  |
    /   | |
   •-------
   ^
  angle
 
  angle = asin(y)
*/

/*
 # arc sine 
 • for x between [0, 0.5] (4 degree polynomial)
 asinPoly(x) = c0 * x + c1 * x² + c2 * x³ + c3 * x⁴....
 asin(x) = asinPoly(x);

 • for x between [0.5, 1.0]
                                     ____________
 asin(x) = half_pi - 2.0 * asinPoly(√ (1 - x) / 2)

 • better performance :
  acos(x) = acos(x)/sqrt(1.0 - x), interval : [0, 1]
  3 degree polynomial
*/

/*
#define __ASIN_0 ((np_real)7.74260188e-02)
#define __ASIN_1 ((np_real)2.61321437e-02)
#define __ASIN_2 ((np_real)7.71270282e-02)
#define __ASIN_3 ((np_real)1.66629259e-01)
#define __ASIN_4 ((np_real)1.57079632e-00)

NP_HOT np_real NP_Asin(np_real x) {

 NP_ALIGNED np_real x2;
 NP_ALIGNED np_real mx;
 NP_ALIGNED np_urb hi;
 NP_ALIGNED np_urb sign;

 sign = x < 0.0f;
 mx = NP_Abs(x);
 hi = x > 0.5f;
 mx = hi ? NP_Sqrt((1.0f - mx) * 0.5f) : mx;
 x2 = mx * mx;
 mx = mx + (mx * x2) * (((__ASIN_0 * x2 + __ASIN_1) * x2 + __ASIN_2) * x2 + __ASIN_3);
 mx = hi ? __ASIN_4 - (mx + mx) : mx;
 return sign ? -mx : mx;
}
*/
#define __ASIN_0 ((np_real) 0.46707077e-01)
#define __ASIN_1 ((np_real)-0.20212058e-00)
#define __ASIN_2 ((np_real) 1.57021169e-00)

NP_HOT np_real NP_Asin(np_real x) {
 NP_ALIGNED np_real mx;
 NP_ALIGNED np_real mx1;
 mx = NP_Abs(x);
 mx1 = __ASIN_0 * mx + __ASIN_1;
 mx1 = mx1 * mx + __ASIN_2;
 mx1 = mx1 * NP_Sqrt(1.0f - mx);
 return 1.57079632f - ((x < 0.0f) ?  3.14159265e-00f - mx1 : mx1);
}

/*
 arc cosine
          • (x, y) = normalized value
         /|
        / |
       /  |
      /   |
     /__  |
    /   | |
   •-------
   ^
  angle
 
  angle = acos(x)
*/
/*
 # arc cosine
 • for x between [0, 0.5] (4 degree polynomial)
 acosPoly(x) = c0 * x + c1 * x² + c2 * x³ + c3 * x⁴....
 acos(x) = acosPoly(x)
 
 • for x between [0.5, 1.0]
                                ____________
 acos(x) = pi - 2.0 * acosPoly(√ (1 - x) / 2)
 
 • better performance :
  acos(x) = acos(x)/sqrt(1.0 - x), interval : [0, 1]
  2 degree polynomial
*/
/*
NP_HOT np_real NP_Acos(np_real x) {
 NP_ALIGNED np_real x2;
 NP_ALIGNED np_real mx;
 NP_ALIGNED np_urb hi;
 NP_ALIGNED np_urb sign;

 sign = x < 0.0f;
 mx = NP_Abs(x);
 hi = mx > 0.5f;
 mx = hi ? NP_Sqrt((1.0f - mx) * 0.5f) : mx;
 x2 = mx * mx;
 mx = mx + (mx * x2) * (((__ASIN_0 * x2 + __ASIN_1) * x2 + __ASIN_2) * x2 + __ASIN_3);
 mx = hi ? (mx + mx) : __ASIN_4 - mx;
 return sign ? 3.14159265f - mx : mx;
}
*/
#define __ACOS_0 ((np_real)-0.15658276e-00)
#define __ACOS_1 ((np_real) 1.56496682e-00)

NP_HOT np_real NP_Acos(np_real x) {
 NP_ALIGNED np_real mx;
 NP_ALIGNED np_real mx1;
 mx = NP_Abs(x);
 mx1 = __ACOS_0 * mx + __ACOS_1;
 mx1 = mx1 * NP_Sqrt(1.0f - mx);
 return (x < 0.0f) ? 3.14159265e-00f - mx1 : mx1;
}


/*
 arc tangent
          • (x, y) normalized value
         /|
        / |
       /  |
      /   |
     /__  |
    /   | |
   •-------
     ^
    angle
 
  angle = atan(y/x)
*/


/*
 # arc tangent
                     ________
 atan(x) = asin(x / √1 + x*x ) 

 • alternative (only works on positive x):
                     ________
 atan(x) = acos(1 / √1 + x*x )
*/

/*
 implementation
 
 for |x| <= 1.0
 
 atanPoly(x) = c0 * x + c1 * x² + c2 * x³....
 
 for |x| >= 1.0
 
 atan(x) pi_half - atanPoly(1.0 / x)
*/

#define __ATAN_0 ((np_real)-1.88450026e-02)
#define __ATAN_1 ((np_real) 7.00033564e-02)
#define __ATAN_2 ((np_real)-1.31229245e-01)
#define __ATAN_3 ((np_real) 1.98759123e-01)
#define __ATAN_4 ((np_real)-3.33311700e-01)
#define __ATAN_5 ((np_real) 1.57079632e-00)
#define __ATAN_6 ((np_real) 3.14159265e-00)

NP_HOT np_real NP_Atan(np_real x) {
 NP_ALIGNED np_real x2;
 NP_ALIGNED np_real mx;
 NP_ALIGNED np_urb hi;
 NP_ALIGNED np_urb sign;
 NP_ALIGNED NPRealBit bits;
 
 bits.f = x;
 sign = bits.i & NP_SIGN_BIT;
 bits.i = bits.i & ~sign;
 hi = (bits.i >> NP_SIGNIFICAND) >= NP_BIAS;
 bits.f = hi ? 1.0f / bits.f : bits.f;
 mx = bits.f;
 x2 = mx * mx;
 mx = mx + (mx * x2) * ((((__ATAN_0 * x2 + __ATAN_1) * x2 + __ATAN_2) * x2 + __ATAN_3) * x2 + __ATAN_4);
 bits.f = hi ? __ATAN_5 - mx : mx;
 bits.i = (bits.i & NP_INV_SIGN_BIT) | sign;
 return bits.f;
}


NP_HOT np_real NP_Atan2(np_real y, np_real x) {
 NP_ALIGNED np_real ratio;
 NP_ALIGNED np_real mx;
 NP_ALIGNED np_real x2;
 NP_ALIGNED np_urb both_lo;
 NP_ALIGNED np_urb y_hi_x_lo;
 NP_ALIGNED np_urb hi;
 NP_ALIGNED np_urb sign;
 NP_ALIGNED NPRealBit bits;
 
 both_lo = (y < 0.0f && x < 0.0f);
 y_hi_x_lo = (y > 0.0f && x < 0.0f);
 ratio = y / x;
 
 bits.f = ratio;
 sign = bits.i & NP_SIGN_BIT;
 bits.i = bits.i & ~sign;
 hi = (bits.i >> NP_SIGNIFICAND) >= NP_BIAS;
 bits.f = hi ? 1.0f / bits.f : bits.f;
 mx = bits.f;
 x2 = mx * mx;
 mx = mx + (mx * x2) * ((((__ATAN_0 * x2 + __ATAN_1) * x2 + __ATAN_2) * x2 + __ATAN_3) * x2 + __ATAN_4);
 bits.f = hi ? __ATAN_5 - mx : mx;
 bits.i = (bits.i & NP_INV_SIGN_BIT) | sign;
 mx = bits.f;
 
 mx = both_lo ? mx - __ATAN_6 : mx;
 return !both_lo && y_hi_x_lo ? mx + __ATAN_6 : mx;
}



/*-------------------------------------------------------------------*/
/*
 log base 2 coeffs
 accurate enough for at lest 2-3 decimal digits.
*/
#define __LOG2_0 ((np_real)(-8.161580e-02))
#define __LOG2_1 ((np_real)( 6.451423e-01))
#define __LOG2_2 ((np_real)(-2.120675e-00))
#define __LOG2_3 ((np_real)( 4.070090e-00))
#define __LOG2_4 ((np_real)(-2.512854e-00))


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
NP_HOT np_real NP_Log2(np_real x) {
 NP_ALIGNED NPRealBit mantissa;
 NP_ALIGNED NPRealBit bits;
 NP_ALIGNED np_real ma;
 NP_ALIGNED np_real mx;
 NP_ALIGNED np_urb sign;
 
 sign = x < 1.0f;
 bits.f = sign ? (1.0f / x) : x;
#ifdef NP_USE_F64
 mantissa.i = 4607182418800017408U | (bits.i & 0x000FFFFFFFFFFFFF);
#else
 mantissa.i = 1065353216U | (bits.i & 0x007FFFFF);
#endif
 ma = mantissa.f;
 mx = __LOG2_0 * ma + __LOG2_1;
 mx = mx * ma + __LOG2_2;
 mx = mx * ma + __LOG2_3;
	mx = mx * ma + __LOG2_4;
 mx = (np_real)((np_srb)(bits.i >> NP_SIGNIFICAND) - NP_BIAS) + mx;
 return sign ? -mx : mx;
}



/*-------------------------------------------------------------------*/
/*
 exponential base 2 coeffs
 accurate enough for at lest 2-3 decimal digits.
*/
#define __EXP2_0 ((np_real)(0.96857114e-02))
#define __EXP2_1 ((np_real)(0.55922025e-01))
#define __EXP2_2 ((np_real)(0.24021865e-00))
#define __EXP2_3 ((np_real)(0.69312103e-00))
#define __EXP2_4 ((np_real)(1.00000015e-00))


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

NP_HOT np_real NP_Exp2(np_real x) {
	NP_ALIGNED np_real mx;
	NP_ALIGNED np_real a;
 NP_ALIGNED	NPRealBit bits;
	NP_ALIGNED np_srb whole;
	
	a = x;
 whole = (np_srb)(a - 0.5f);
 a = a - ((np_real)whole);
 bits.i = (np_urb)((np_srb)(NP_BIAS + whole) << NP_SIGNIFICAND);
 mx = __EXP2_0 * a + __EXP2_1;
	mx = mx * a + __EXP2_2;
 mx = mx * a + __EXP2_3;
 mx = mx * a + __EXP2_4;
 return bits.f * mx;
}


/*
 Quake III
 fast inv sqrt algorithm
*/
#ifdef NP_USE_F64
#define NP_INV_SQRT_MAGIC_NUMBER 0x5fe6eb50c7b537a9
#else
#define NP_INV_SQRT_MAGIC_NUMBER 0x5F378171
#endif


NP_HOT np_real NP_InvSqrt(np_real x) {
 np_real f, mx, x_half;
 NPRealBit bits, x_bits;
 
 x_bits.f = x;
 bits.i =   NP_INV_SQRT_MAGIC_NUMBER - (x_bits.i >> 1);
 mx =       bits.f;
 x_half =   x * 0.5f;
 f =        mx;
 f =        (f * (1.5f - (x_half * f * f)));
 return f;
}


/*
 sqrt function 
 fpu sqrt, this should be very fast.
*/
NP_HOT np_real NP_Sqrt(np_real x) {
#if defined(__aarch64__)

#if defined(NP_USE_F64)
 __asm__ volatile(
  "fsqrt %d0, %d0 \n"
  : "=w"(x)
  : "w"(x)
 );
 return x;
#else
 __asm__ volatile(
  "fsqrt %s0, %s0 \n"
  : "=w"(x)
  : "w"(x)
 );
 return x;
#endif

#elif defined(__x86_64__)
/*sse2 instructions*/
#if defined(NP_USE_F64)
 __asm__ volatile (
  "sqrtsd %[in], %[out]"
  : [out] "=x" (x)
  : [in] "x" (x)
  : "xmm0"
 );
 return x;
#else
 __asm__ volatile (
  "movss %[in], %%xmm0 \n"
  "sqrtss %%xmm0, %%xmm0 \n"
  "movss %%xmm0, %[out]"
  : [out] "=m" (x)
  : [in] "m" (x)
  : "xmm0"
 );
 return x;
#endif

#else
 np_real f, mx, x_half;
 NPRealBit bits, x_bits;
 
 x_bits.f = x;
 bits.i =   NP_INV_SQRT_MAGIC_NUMBER - (x_bits.i >> 1);
 mx =       bits.f;
 x_half =   x * 0.5f;
 f =        mx;
 f =        (f * (1.5f - (x_half * f * f)));
 return x * f;
#endif
}




