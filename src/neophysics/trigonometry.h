#ifndef NP_TRIGONOMETRY_H
#define NP_TRIGONOMETRY_H

#include "neophysics/types.h"
#include "neophysics/compiler_hint.h"

//-------------------------------------------------------------------//
//coeffs polynomial with lowest degree
//just acceptable enough for rough estimation
#define __SIN_0 ((NPReal)( 0.008028e-00))
#define __SIN_1 ((NPReal)(-0.166607e-00))

#define __COS_0 ((NPReal)(-0.001312e-00))
#define __COS_1 ((NPReal)( 0.041592e-00))
#define __COS_2 ((NPReal)(-0.499976e-00))
#define __COS_3 ((NPReal)( 0.999998e-00))


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
NP_HOT 
NP_INLINE void __np_internal_sincos(NPReal x, NPUint8 sign, NPReal *s, NPReal *c) {
 NPReal mx, mx_s, mx_c, x2_c, x2_s, out_s, out_c, a;
 NPUint32 sign_s, sign_c, flip_s, flip_c, q;
 
 a = x;
 sign_s      = sign;
 
 q =         ((NPUint32)(a * (NPReal)(0.6366197)));
 mx =        a;
 mx =        mx - (1.57079632 * (NPReal)(q));
 
 //hoping that this is pipeline optimized 
 //execution
 mx_c =      mx;
 mx_s =      mx;
 

 flip_s =    (q == 1 || q == 3);
 flip_c =    (q == 1 || q == 3);
 sign_c =    (q == 1 || q == 2);

 sign_s =    sign_s ^ (q == 2 || q == 3);

 mx_s =      flip_s ? ((NPReal)1.57079632 - mx_s) : mx_s;
 mx_c =      flip_c ? (mx_c - (NPReal)1.57079632) : mx_c;

 x2_s =      mx_s * mx_s;
 x2_c =      mx_c * mx_c;

 out_s = __SIN_0 * x2_s + __SIN_1;
 out_c = __COS_0 * x2_c + __COS_1;
 
 out_s = out_s * (x2_s * mx_s) + mx_s;
 out_c = out_c * x2_c + __COS_2;

 out_s =     sign_s ? -out_s : out_s;
 out_c = out_c * x2_c + __COS_3;

 *s = out_s;
 out_c =     sign_c ? -out_c : out_c;
 *c = out_c;
}



#endif

