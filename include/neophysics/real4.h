#ifndef NP_REAL4_H
#define NP_REAL4_H

#include "neophysics/math.h"

typedef struct {
	np_real x, y, z, w;
} NPReal4;


static inline NPReal4 NP_Stv4(np_real a) {
 NPReal4 out;
 out.x = a;
 out.y = a;
 out.z = a;
 out.w = a;
 return out;
}


static inline NPReal4 NP_Add4(NPReal4 a, NPReal4 b) {
#ifdef NP_HAS_NEON

#ifdef NP_USE_F64
 NPReal4 outv;
 float64x2_t out = vaddq_f64(*(float64x2_t*)&a, *(float64x2_t*)&b);
 float64x2_t out1 = vaddq_f64(*(float64x2_t*)&a.z, *(float64x2_t*)&b.z);
 vst1q_f64((np_real*)&outv, out);
 vst1q_f64((np_real*)&outv.z, out1);
 return outv;
#else
 float32x4_t out = vaddq_f32(*(float32x4_t*)&a, *(float32x4_t*)&b);
 return *(NPReal4*)&out;
#endif

#else
 NPReal4 out;
 out.x = a.x + b.x;
 out.y = a.y + b.y;
 out.z = a.z + b.z;
 out.w = a.w + b.w;
 return out;
#endif
}


static inline NPReal4 NP_Sub4(NPReal4 a, NPReal4 b) {
#ifdef NP_HAS_NEON

#ifdef NP_USE_F64
 NPReal4 outv;
 float64x2_t out = vsubq_f64(*(float64x2_t*)&a, *(float64x2_t*)&b);
 float64x2_t out1 = vsubq_f64(*(float64x2_t*)&a.z, *(float64x2_t*)&b.z);
 vst1q_f64((np_real*)&outv, out);
 vst1q_f64((np_real*)&outv.z, out1);
 return outv;
#else
 float32x4_t out = vsubq_f32(*(float32x4_t*)&a, *(float32x4_t*)&b);
 return *(NPReal4*)&out;
#endif

#else
 NPReal4 out;
 out.x = a.x - b.x;
 out.y = a.y - b.y;
 out.z = a.z - b.z;
 out.w = a.w - b.w;
 return out;
#endif
}


static inline NPReal4 NP_Mul4(NPReal4 a, NPReal4 b) {
#ifdef NP_HAS_NEON

#ifdef NP_USE_F64
 NPReal4 outv;
 float64x2_t out = vmulq_f64(*(float64x2_t*)&a, *(float64x2_t*)&b);
 float64x2_t out1 = vmulq_f64(*(float64x2_t*)&a.z, *(float64x2_t*)&b.z);
 vst1q_f64((np_real*)&outv, out);
 vst1q_f64((np_real*)&outv.z, out1);
 return outv;
#else
 float32x4_t out = vmulq_f32(*(float32x4_t*)&a, *(float32x4_t*)&b);
 return *(NPReal4*)&out;
#endif

#else
 NPReal4 out;
 out.x = a.x * b.x;
 out.y = a.y * b.y;
 out.z = a.z * b.z;
 out.w = a.w * b.w;
 return out;
#endif
}


static inline NPReal4 NP_Div4(NPReal4 a, NPReal4 b) {
#ifdef NP_HAS_NEON

#ifdef NP_USE_F64
 NPReal4 outv;
 float64x2_t out = vdivq_f64(*(float64x2_t*)&a, *(float64x2_t*)&b);
 float64x2_t out1 = vdivq_f64(*(float64x2_t*)&a.z, *(float64x2_t*)&b.z);
 vst1q_f64((np_real*)&outv, out);
 vst1q_f64((np_real*)&outv.z, out1);
 return outv;
#else
 float32x4_t out = vdivq_f32(*(float32x4_t*)&a, *(float32x4_t*)&b);
 return *(NPReal4*)&out;
#endif

#else
 NPReal4 out;
 out.x = a.x / b.x;
 out.y = a.y / b.y;
 out.z = a.z / b.z;
 out.w = a.w / b.w;
 return out;
#endif
}


static inline NPReal4 NP_MulAc4(NPReal4 a, NPReal4 b, NPReal4 c) {
#ifdef NP_HAS_NEON

#ifdef NP_USE_F64
 NPReal4 outv;
 float64x2_t out = vfmaq_f64(*(float64x2_t*)&c, *(float64x2_t*)&a, *(float64x2_t*)&b);
 float64x2_t out1 = vfmaq_f64(*(float64x2_t*)&c.z, *(float64x2_t*)&a.z, *(float64x2_t*)&b.z);
 vst1q_f64((np_real*)&outv, out);
 vst1q_f64((np_real*)&outv.z, out1);
 return outv;
#else
 float32x4_t out = vfmaq_f32(*(float32x4_t*)&c, *(float32x4_t*)&a, *(float32x4_t*)&b);
 return *(NPReal4*)&out;
#endif

#else
 NPReal4 out;
 out.x = a.x * b.x + c.x;
 out.y = a.y * b.y + c.y;
 out.z = a.z * b.z + c.z;
 out.w = a.w * b.w + c.w;
 return out;
#endif
}


static inline NPReal4 NP_Neg4(NPReal4 a) {
#ifdef NP_HAS_NEON

#ifdef NP_USE_F64
 NPReal4 outv;
 float64x2_t out = vnegq_f64(*(float64x2_t*)&a);
 float64x2_t out1 = vnegq_f64(*(float64x2_t*)&a.z);
 vst1q_f64((np_real*)&outv, out);
 vst1q_f64((np_real*)&outv.z, out1);
 return outv;
#else
 float32x4_t out = vnegq_f32(*(float32x4_t*)&a);
 return *(NPReal4*)&out;
#endif

#else
 NPReal4 out;
 out.x = -a.x;
 out.y = -a.y;
 out.z = -a.z;
 out.w = -a.w;
 return out;
#endif
}


static inline np_real NP_Dot4(NPReal4 a, NPReal4 b) {
 return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
}


static inline NPReal4 NP_Normalize4(NPReal4 a) {
 const np_real il = NP_InvSqrt(NP_Dot4(a, a));
 return NP_Mul4(a, NP_Stv4(il));
}


#endif
