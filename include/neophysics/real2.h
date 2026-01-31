#ifndef NP_REAL2_H
#define NP_REAL2_H

#include "neophysics/math.h"

typedef struct {
	np_real x, y;
} NPReal2;


static inline NPReal2 NP_Stv2(np_real a) {
 NPReal2 out;
 out.x = a;
 out.y = a;
 return out;
}


static inline NPReal2 NP_Add2(NPReal2 a, NPReal2 b) {
#ifdef NP_HAS_NEON

#ifdef NP_USE_F64
 float64x2_t out = vaddq_f64(*(float64x2_t*)&a, *(float64x2_t*)&b);
 return *(NPReal2*)&out;
#else
 float32x2_t out = vadd_f32(*(float32x2_t*)&a, *(float32x2_t*)&b);
 return *(NPReal2*)&out;
#endif

#else
 NPReal2 out;
 out.x = a.x + b.x;
 out.y = a.y + b.y;
 return out;
#endif
}


static inline NPReal2 NP_Sub2(NPReal2 a, NPReal2 b) {
#ifdef NP_HAS_NEON

#ifdef NP_USE_F64
 float64x2_t out = vsubq_f64(*(float64x2_t*)&a, *(float64x2_t*)&b);
 return *(NPReal2*)&out;
#else
 float32x2_t out = vsub_f32(*(float32x2_t*)&a, *(float32x2_t*)&b);
 return *(NPReal2*)&out;
#endif

#else
 NPReal2 out;
 out.x = a.x - b.x;
 out.y = a.y - b.y;
 return out;
#endif
}


static inline NPReal2 NP_Mul2(NPReal2 a, NPReal2 b) {
#ifdef NP_HAS_NEON

#ifdef NP_USE_F64
 float64x2_t out = vmulq_f64(*(float64x2_t*)&a, *(float64x2_t*)&b);
 return *(NPReal2*)&out;
#else
 float32x2_t out = vmul_f32(*(float32x2_t*)&a, *(float32x2_t*)&b);
 return *(NPReal2*)&out;
#endif

#else
 NPReal2 out;
 out.x = a.x * b.x;
 out.y = a.y * b.y;
 return out;
#endif
}


static inline NPReal2 NP_Div2(NPReal2 a, NPReal2 b) {
#ifdef NP_HAS_NEON

#ifdef NP_USE_F64
 float64x2_t out = vdivq_f64(*(float64x2_t*)&a, *(float64x2_t*)&b);
 return *(NPReal2*)&out;
#else
 float32x2_t out = vdiv_f32(*(float32x2_t*)&a, *(float32x2_t*)&b);
 return *(NPReal2*)&out;
#endif

#else
 NPReal2 out;
 out.x = a.x / b.x;
 out.y = a.y / b.y;
 return out;
#endif
}


static inline NPReal2 NP_MulAc2(NPReal2 a, NPReal2 b, NPReal2 c) {
#ifdef NP_HAS_NEON

#ifdef NP_USE_F64
 float64x2_t out = vfmaq_f64(*(float64x2_t*)&c, *(float64x2_t*)&a, *(float64x2_t*)&b);
 return *(NPReal2*)&out;
#else
 float32x2_t out = vfma_f32(*(float32x2_t*)&c, *(float32x2_t*)&a, *(float32x2_t*)&b);
 return *(NPReal2*)&out;
#endif

#else
 NPReal2 out;
 out.x = a.x * b.x + c.x;
 out.y = a.y * b.y + c.y;
 return out;
#endif
}


static inline NPReal2 NP_Neg2(NPReal2 a) {
#ifdef NP_HAS_NEON

#ifdef NP_USE_F64
 float64x2_t out = vnegq_f64(*(float64x2_t*)&a);
 return *(NPReal2*)&out;
#else
 float32x2_t out = vneg_f32(*(float32x2_t*)&a);
 return *(NPReal2*)&out;
#endif

#else
 NPReal2 out;
 out.x = -a.x;
 out.y = -a.y;
 return out;
#endif
}


static inline np_real NP_Dot2(NPReal2 a, NPReal2 b) {
 return a.x * b.x + a.y * b.y;
}


static inline NPReal2 NP_Normalize2(NPReal2 a) {
 const np_real il = NP_InvSqrt(NP_Dot2(a, a));
 return NP_Mul2(a, NP_Stv2(il));
}

#endif
