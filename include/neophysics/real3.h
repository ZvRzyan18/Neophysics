#ifndef NP_REAL3_H
#define NP_REAL3_H

#include "neophysics/math.h"

typedef struct {
	np_real x, y, z, _pad;
} NPReal3;


static inline NPReal3 NP_Stv3(np_real a) {
 NPReal3 out;
 out.x = a;
 out.y = a;
 out.z = a;
 return out;
}


static inline NPReal3 NP_Add3(NPReal3 a, NPReal3 b) {
#ifdef NP_HAS_NEON

#ifdef NP_USE_F64
 NPReal3 outv;
 float64x2_t out = vaddq_f64(*(float64x2_t*)&a, *(float64x2_t*)&b);
 float64x2_t out1 = vaddq_f64(*(float64x2_t*)&a.z, *(float64x2_t*)&b.z);
 vst1q_f64((np_real*)&outv, out);
 vst1q_f64((np_real*)&outv.z, out1);
 return outv;
#else
 float32x4_t out = vaddq_f32(*(float32x4_t*)&a, *(float32x4_t*)&b);
 return *(NPReal3*)&out;
#endif

#else
 NPReal3 out;
 out.x = a.x + b.x;
 out.y = a.y + b.y;
 out.z = a.z + b.z;
 return out;
#endif
}


static inline NPReal3 NP_Sub3(NPReal3 a, NPReal3 b) {
#ifdef NP_HAS_NEON

#ifdef NP_USE_F64
 NPReal3 outv;
 float64x2_t out = vsubq_f64(*(float64x2_t*)&a, *(float64x2_t*)&b);
 float64x2_t out1 = vsubq_f64(*(float64x2_t*)&a.z, *(float64x2_t*)&b.z);
 vst1q_f64((np_real*)&outv, out);
 vst1q_f64((np_real*)&outv.z, out1);
 return outv;
#else
 float32x4_t out = vsubq_f32(*(float32x4_t*)&a, *(float32x4_t*)&b);
 return *(NPReal3*)&out;
#endif

#else
 NPReal3 out;
 out.x = a.x - b.x;
 out.y = a.y - b.y;
 out.z = a.z - b.z;
 return out;
#endif
}


static inline NPReal3 NP_Mul3(NPReal3 a, NPReal3 b) {
#ifdef NP_HAS_NEON

#ifdef NP_USE_F64
 NPReal3 outv;
 float64x2_t out = vmulq_f64(*(float64x2_t*)&a, *(float64x2_t*)&b);
 float64x2_t out1 = vmulq_f64(*(float64x2_t*)&a.z, *(float64x2_t*)&b.z);
 vst1q_f64((np_real*)&outv, out);
 vst1q_f64((np_real*)&outv.z, out1);
 return outv;
#else
 float32x4_t out = vmulq_f32(*(float32x4_t*)&a, *(float32x4_t*)&b);
 return *(NPReal3*)&out;
#endif

#else
 NPReal3 out;
 out.x = a.x * b.x;
 out.y = a.y * b.y;
 out.z = a.z * b.z;
 return out;
#endif
}


static inline NPReal3 NP_Div3(NPReal3 a, NPReal3 b) {
#ifdef NP_HAS_NEON

#ifdef NP_USE_F64
 NPReal3 outv;
 float64x2_t out = vdivq_f64(*(float64x2_t*)&a, *(float64x2_t*)&b);
 float64x2_t out1 = vdivq_f64(*(float64x2_t*)&a.z, *(float64x2_t*)&b.z);
 vst1q_f64((np_real*)&outv, out);
 vst1q_f64((np_real*)&outv.z, out1);
 return outv;
#else
 float32x4_t out = vdivq_f32(*(float32x4_t*)&a, *(float32x4_t*)&b);
 return *(NPReal3*)&out;
#endif

#else
 NPReal3 out;
 out.x = a.x / b.x;
 out.y = a.y / b.y;
 out.z = a.z / b.z;
 return out;
#endif
}


static inline NPReal3 NP_MulAc3(NPReal3 a, NPReal3 b, NPReal3 c) {
#ifdef NP_HAS_NEON

#ifdef NP_USE_F64
 NPReal3 outv;
 float64x2_t out = vfmaq_f64(*(float64x2_t*)&c, *(float64x2_t*)&a, *(float64x2_t*)&b);
 float64x2_t out1 = vfmaq_f64(*(float64x2_t*)&c.z, *(float64x2_t*)&a.z, *(float64x2_t*)&b.z);
 vst1q_f64((np_real*)&outv, out);
 vst1q_f64((np_real*)&outv.z, out1);
 return outv;
#else
 float32x4_t out = vfmaq_f32(*(float32x4_t*)&c, *(float32x4_t*)&a, *(float32x4_t*)&b);
 return *(NPReal3*)&out;
#endif

#else
 NPReal3 out;
 out.x = a.x * b.x + c.x;
 out.y = a.y * b.y + c.y;
 out.z = a.z * b.z + c.z;
 return out;
#endif
}


static inline NPReal3 NP_Neg3(NPReal3 a) {
#ifdef NP_HAS_NEON

#ifdef NP_USE_F64
 NPReal3 outv;
 float64x2_t out = vnegq_f64(*(float64x2_t*)&a);
 float64x2_t out1 = vnegq_f64(*(float64x2_t*)&a.z);
 vst1q_f64((np_real*)&outv, out);
 vst1q_f64((np_real*)&outv.z, out1);
 return outv;
#else
 float32x4_t out = vnegq_f32(*(float32x4_t*)&a);
 return *(NPReal3*)&out;
#endif

#else
 NPReal3 out;
 out.x = -a.x;
 out.y = -a.y;
 out.z = -a.z;
 return out;
#endif
}


static inline np_real NP_Dot3(NPReal3 a, NPReal3 b) {
 return a.x * b.x + a.y * b.y + a.z * b.z;
}


static inline NPReal3 NP_Normalize3(NPReal3 a) {
 const np_real il = NP_InvSqrt(NP_Dot3(a, a));
 return NP_Mul3(a, NP_Stv3(il));
}


#endif
