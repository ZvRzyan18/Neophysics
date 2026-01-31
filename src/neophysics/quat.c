#include "neophysics/quat.h"
#include "neophysics/math.h"
#include <stdalign.h>

NP_ALIGNED const NPReal4 NPQuat_Identity = {
 0.0f, 0.0f, 0.0f, 1.0f
};

/*
 multiply two quaternions
*/
NP_HOT NPReal4 NPQuat_Mul(NPReal4 a, NPReal4 b) {
 NP_ALIGNED NPReal4 q;
#ifdef NP_HAS_NEON

#ifdef NP_USE_F64

 NP_ALIGNED float64x2_t tmp_0[2] = {{a.x, -a.x}, {a.x, -a.x}};
 NP_ALIGNED float64x2_t tmp_1[2] = {{b.w, b.z}, {b.y, b.x}};
 NP_ALIGNED float64x2_t tmp_2[2] = {{a.y, a.y}, {-a.y, -a.y}};
 NP_ALIGNED float64x2_t tmp_3[2] = {{b.z, b.w}, {b.x, b.y}};
 NP_ALIGNED float64x2_t tmp_4[2] = {{-a.z, a.z}, {a.z, -a.z}};

 NP_ALIGNED float64x2_t tmp_5[2] = {{b.y, b.x}, {b.w, b.z}};
 NP_ALIGNED float64x2_t tmp_6[2] = {{a.w, a.w}, {a.w, a.w}};
 NP_ALIGNED float64x2_t tmp_7[2] = {{b.x, b.y}, {b.z, b.w}};

 NP_ALIGNED float64x2_t out[2];

 out[0] = vfmaq_f64(vfmaq_f64(vfmaq_f64(vmulq_f64(tmp_6[0], tmp_7[0]), tmp_4[0], tmp_5[0]), tmp_2[0], tmp_3[0]), tmp_0[0], tmp_1[0]);
 out[1] = vfmaq_f64(vfmaq_f64(vfmaq_f64(vmulq_f64(tmp_6[1], tmp_7[1]), tmp_4[1], tmp_5[1]), tmp_2[1], tmp_3[1]), tmp_0[1], tmp_1[1]);

 vst1q_f64((np_real*)&q, out[0]);
 vst1q_f64((np_real*)&q.z, out[1]);
 return q;

#else

 NP_ALIGNED float32x4_t tmp_0 = {a.x, -a.x, a.x, -a.x};
 NP_ALIGNED float32x4_t tmp_1 = {b.w, b.z, b.y, b.x};
 NP_ALIGNED float32x4_t tmp_2 = {a.y, a.y, -a.y, -a.y};
 NP_ALIGNED float32x4_t tmp_3 = {b.z, b.w, b.x, b.y};
 NP_ALIGNED float32x4_t tmp_4 = {-a.z, a.z, a.z, -a.z};

 NP_ALIGNED float32x4_t tmp_5 = {b.y, b.x, b.w, b.z};
 NP_ALIGNED float32x4_t tmp_6 = vdupq_n_f32(a.w);
 NP_ALIGNED float32x4_t tmp_7 = {b.x, b.y, b.z, b.w};

 NP_ALIGNED float32x4_t out;
 out = vfmaq_f32(vfmaq_f32(vfmaq_f32(vmulq_f32(tmp_6, tmp_7), tmp_4, tmp_5), tmp_2, tmp_3), tmp_0, tmp_1);

 vst1q_f32((np_real*)&q, out);
 return q;
 
#endif

#else
 q.x = a.x * b.w + a.y * b.z - a.z * b.y + a.w * b.x;
 q.y = -a.x * b.z + a.y * b.w + a.z * b.x + a.w * b.y;
 q.z = a.x * b.y - a.y * b.x + a.z * b.w + a.w * b.z;
 q.w = -a.x * b.x - a.y * b.y - a.z * b.z + a.w * b.w;
 return q;
#endif
}

/*
 convert euler angle to quaternion 
*/
NP_HOT NPReal4 NPQuat_FromEulerAngle(const np_real roll, const np_real pitch, const np_real yaw) {
 NP_ALIGNED NPReal4 q;
 np_real cy, sy;
 np_real cp, sp;
 np_real cr, sr;
 
 NP_SinCos(yaw * 0.5f, &sy, &cy);
 NP_SinCos(pitch * 0.5f, &sp, &cp);
 NP_SinCos(roll * 0.5f, &sr, &cr);
 
 const np_real crcp = cr*cp;
 const np_real crsp = cr*sp;
 const np_real srcp = sr*cp;
 const np_real srsp = sr*sp;
 
 q.w = crcp * cy + srsp * sy;
 q.x = srcp * cy - crsp * sy;
 q.y = crsp * cy + srcp * sy;
 q.z = crcp * sy - srsp * cy;
 return q;
}

/*
 convert quaternion to euler angle 
*/
NP_HOT NPReal3 NPQuat_ToEulerAngle(NPReal4 q) {
 NPReal3 v;
 NPRealBit bits;
 const np_real y2 = q.y * q.y;
 np_real mpitch = 2.0f * (q.w * q.y - q.z * q.x);
 if(mpitch >= 1.0f) {
  bits.f = mpitch;
  v.y =  (bits.i & NP_SIGN_BIT) ? -1.57079632f : 1.57079632f;
 } else
  v.y = NP_Asin(mpitch);
 /* TODO : use vector instruction in atan2 */
 v.x = NP_Atan2(2.0f * (q.w * q.x + q.y * q.z), 1.0f - 2.0f * (q.x * q.x + y2));
 v.z = NP_Atan2(2.0f * (q.w * q.z + q.x * q.y), 1.0f - 2.0f * (y2 + q.z * q.z));
 return v;
}

/*
 convert axis angle to quaternion
*/
NP_HOT NPReal4 NPQuat_FromAxisAngle(const np_real r, const np_real x, const np_real y, const np_real z) {
 NPReal4 q;
 np_real s;
 NP_SinCos(r * 0.5f, &s, &q.w);
	q.x = x * s;
 q.y = y * s;
	q.z = z * s;
	return q;
}

/*
 convert quaternion to axis angle
*/
NP_HOT NPReal4 NPQuat_ToAxisAngle(NPReal4 x) {
 NPReal4 a;
	a.w = NP_Acos(x.w) * 2.0f;
 const np_real b = NP_InvSqrt(1.0f - x.w * x.w);
 a.x = x.x * b;
 a.y = x.y * b;
 a.z = x.z * b;
 return a;
}

/*
 linear interpolation
*/
NP_HOT NPReal4 NPQuat_Lerp(NPReal4 a, NPReal4 b, const np_real t) {
 NPReal4 q;
 NPRealBit bits;
	bits.f = NP_Dot4(a, b);
	if(bits.i & NP_SIGN_BIT) {
  q.x = a.x - t * (a.x - (-b.x));
  q.y = a.y - t * (a.y - (-b.y));
  q.z = a.z - t * (a.z - (-b.z));
  q.w = a.w - t * (a.w - (-b.w));
	} else {
  q.x = a.x - t * (a.x - (b.x));
  q.y = a.y - t * (a.y - (b.y));
  q.z = a.z - t * (a.z - (b.z));
  q.w = a.w - t * (a.w - (b.w));
	}
	return q;
}


/*
 spherical linear interpolation
*/
NP_HOT NPReal4 NPQuat_Slerp(NPReal4 a, NPReal4 b, const np_real t) {
 NPReal4 q;
 NPRealBit bits;
 float s1, s2, db, omega, rs_omega;
	db = NP_Dot4(a, b);
	bits.f = db;
	if(bits.i & NP_SIGN_BIT) {
	 db = NP_Abs(db);
   if(db > (1.0f - 1e-4f)) { //db > 0.999900, or db > (1.0 - 1e-4)
    s1 = 1.0f - t;
   	s2 = -t;
   } else {
    omega = NP_Acos(db);
    rs_omega = 1.0f / NP_Sin(omega);
    /* TODO : use vector instruction here in sin */
    s1 = NP_Sin((1.0f - t) * omega) * rs_omega;
    s2 = -NP_Sin(t * omega) * rs_omega;
   }
 	} else {
   if(db > (1.0f - 1e-4f)) { //db > 0.999900, or db > (1.0 - 1e-4)
    s1 = 1.0f - t;
   	s2 = t;
   } else {
    omega = NP_Acos(db);
    rs_omega = 1.0f / NP_Sin(omega);
    /* TODO : use vector instruction here in sin */
    s1 = NP_Sin((1.0f - t) * omega) * rs_omega;
    s2 = NP_Sin(t * omega) * rs_omega;
  }
	}
 q.x = s1 * a.x + s2 * b.x;
 q.y = s1 * a.y + s2 * b.y;
 q.z = s1 * a.z + s2 * b.z;
 q.w = s1 * a.w + s2 * b.w;
 return q;
}


/*
 inverse, can be used to divide quaternion
 div(q1, q2) = q1 * q2 ^ -1 
*/
NP_HOT NPReal4 NPQuat_Invert(NPReal4 a) {
 NPReal4 q;
 const np_real rl = NP_InvSqrt(a.x * a.x + a.y * a.y + a.z * a.z + a.w * a.w);
 q.w *= rl;
	q.x *= (-rl);
	q.y *= (-rl);
	q.z *= (-rl);
	return q;
}


/*
 negate vector3 (x, y, z), and w component is still the same
*/
NP_HOT NPReal4 NPQuat_Conjugate(NPReal4 a) {
 NPReal4 q;
 q.w = a.w;
	q.x = -a.x;
	q.y = -a.y;
	q.z = -a.z;
	return q;
}


