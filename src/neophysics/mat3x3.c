#include "neophysics/mat3x3.h"
#include "neophysics/math.h"
#include <stdalign.h>

/*
 // print matrix code
void pm3(NPMat3x3 m) {
	int mm = 0;
	for(int i = 0; i < 12; i++) {
	 if(((mm % 3) == 0) && mm > 0) {
	 	mm = 0;
	 	printf("\n");
	 	continue;
	 }
	 printf("%.2f, ", *(((np_real*)&m) + i));
	 mm++;
	}
}
*/

/*
 Column Major Matrix
*/
NP_ALIGNED const NPMat3x3 NPMat3x3_Identity = {
	1.0f, 0.0f, 0.0f, 0.0f,
	0.0f, 1.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 1.0f, 0.0f
};


NP_HOT NPMat3x3 NPMat3x3_FromQuaternion(NPReal4 q) {
 NP_ALIGNED NPMat3x3 m;
 NP_ALIGNED np_real tmp[9];
#ifdef NP_HAS_NEON
 
#ifdef NP_USE_F64

 NP_ALIGNED float64x2_t aa[2] = {{q.x, q.x}, {q.x, q.x}};
 NP_ALIGNED float64x2_t bb[2] = {{q.x, q.y}, {q.z, q.w}};
 NP_ALIGNED float64x2_t cc[2] = {{q.y, q.y}, {q.y, q.z}};
 NP_ALIGNED float64x2_t dd[2] = {{q.y, q.z}, {q.w, q.z}};
 NP_ALIGNED float64x2_t ee[2];
 
 ee[0] = vmulq_f64(aa[0], bb[0]);
 ee[1] = vmulq_f64(aa[1], bb[1]);
 NP_ALIGNED float64x2_t ff[2];
 ff[0] = vmulq_f64(cc[0], dd[0]);
 ff[1] = vmulq_f64(cc[1], dd[1]);
 
 vst1q_f64(&tmp[0], ee[0]);
 vst1q_f64(&tmp[2], ee[1]);
 vst1q_f64(&tmp[4], ff[0]);
 vst1q_f64(&tmp[6], ff[1]);
 tmp[8] = q.z * q.w;
 
 NP_ALIGNED float64x2_t tmp1[2];
 NP_ALIGNED float64x2_t r1_a[2] = {{tmp[4], tmp[1]}, {tmp[2], 0.0}};
 NP_ALIGNED float64x2_t r1_b[2] = {{tmp[7], -tmp[8]}, {tmp[6], 0.0}};

 NP_ALIGNED float64x2_t r2_a[2] = {{tmp[1], tmp[0]}, {tmp[5], 0.0}};
 NP_ALIGNED float64x2_t r2_b[2] = {{tmp[8], tmp[7]}, {-tmp[3], 0.0}};

 NP_ALIGNED float64x2_t r3_a[2] = {{tmp[2], tmp[5]}, {tmp[0], 0.0}};
 NP_ALIGNED float64x2_t r3_b[2] = {{-tmp[6], tmp[3]}, {tmp[4], 0.0}};

 tmp1[0] = vaddq_f64(r1_a[0], r1_b[0]);
 tmp1[1] = vaddq_f64(r1_a[1], r1_b[1]);
 vst1q_f64(&m.m_00, vaddq_f64(tmp1[0], tmp1[0]));
 vst1q_f64(&m.m_20, vaddq_f64(tmp1[1], tmp1[1]));

 tmp1[0] = vaddq_f64(r2_a[0], r2_b[0]);
 tmp1[1] = vaddq_f64(r2_a[1], r2_b[1]);
 vst1q_f64(&m.m_01, vaddq_f64(tmp1[0], tmp1[0]));
 vst1q_f64(&m.m_21, vaddq_f64(tmp1[1], tmp1[1]));

 tmp1[0] = vaddq_f64(r3_a[0], r3_b[0]);
 tmp1[1] = vaddq_f64(r3_a[1], r3_b[1]);
 vst1q_f64(&m.m_02, vaddq_f64(tmp1[0], tmp1[0]));
 vst1q_f64(&m.m_22, vaddq_f64(tmp1[1], tmp1[1]));

 m.m_00 = 1.0 - m.m_00;
 m.m_11 = 1.0 - m.m_11;
 m.m_22 = 1.0 - m.m_22;
 return m;

#else

 NP_ALIGNED float32x4_t aa = vdupq_n_f32(q.x);
 NP_ALIGNED float32x4_t bb = {q.x, q.y, q.z, q.w};
 NP_ALIGNED float32x4_t cc = {q.y, q.y, q.y, q.z};
 NP_ALIGNED float32x4_t dd = {q.y, q.z, q.w, q.z};
 NP_ALIGNED float32x4_t ee = vmulq_f32(aa, bb);
 NP_ALIGNED float32x4_t ff = vmulq_f32(cc, dd);
 vst1q_f32(&tmp[0], ee);
 vst1q_f32(&tmp[4], ff);
 tmp[8] = q.z * q.w;
 
 NP_ALIGNED float32x4_t tmp1;
 NP_ALIGNED float32x4_t r1_a = {tmp[4], tmp[1], tmp[2], 0.0};
 NP_ALIGNED float32x4_t r1_b = {tmp[7], -tmp[8], tmp[6], 0.0};

 NP_ALIGNED float32x4_t r2_a = {tmp[1], tmp[0], tmp[5], 0.0};
 NP_ALIGNED float32x4_t r2_b = {tmp[8], tmp[7], -tmp[3], 0.0};

 NP_ALIGNED float32x4_t r3_a = {tmp[2], tmp[5], tmp[0], 0.0};
 NP_ALIGNED float32x4_t r3_b = {-tmp[6], tmp[3], tmp[4], 0.0};
 
 tmp1 = vaddq_f32(r1_a, r1_b);
 vst1q_f32(&m.m_00, vaddq_f32(tmp1, tmp1));
 tmp1 = vaddq_f32(r2_a, r2_b);
 vst1q_f32(&m.m_01, vaddq_f32(tmp1, tmp1));
 tmp1 = vaddq_f32(r3_a, r3_b);
 vst1q_f32(&m.m_02, vaddq_f32(tmp1, tmp1));
 
 m.m_00 = 1.0f - m.m_00;
 m.m_11 = 1.0f - m.m_11;
 m.m_22 = 1.0f - m.m_22;
 return m;

#endif

#else
 tmp[0] = q.x * q.x;
 tmp[1] = q.x * q.y;
 tmp[2] = q.x * q.z;
 tmp[3] = q.x * q.w;

 tmp[4] = q.y * q.y;
 tmp[5] = q.y * q.z;
 tmp[6] = q.y * q.w;

 tmp[7] = q.z * q.z;
 tmp[8] = q.z * q.w;

 m.m_00 = 1.0f - 2.0f * (tmp[4] + tmp[7]);
 m.m_10 =     2.0f * (tmp[1] - tmp[8]);
 m.m_20 =     2.0f * (tmp[2] + tmp[6]);

 m.m_01 =     2.0f * (tmp[1] + tmp[8]);
 m.m_11 = 1.0f - 2.0f * (tmp[0] + tmp[7]);
 m.m_21 =     2.0f * (tmp[5] - tmp[3]);

 m.m_02 =     2.0f * (tmp[2] - tmp[6]);
 m.m_12 =     2.0f * (tmp[5] + tmp[3]);
 m.m_22 = 1.0f - 2.0f * (tmp[0] + tmp[4]);
 return m;
#endif
}


NP_HOT NPMat3x3 NPMat3x3_Mul(NPMat3x3 a, NPMat3x3 b) {
 NP_ALIGNED NPMat3x3 m;
#ifdef NP_HAS_NEON

#ifdef NP_USE_F64

 NP_ALIGNED float64x2_t c0_1[2] = {{a.m_00, a.m_10}, {a.m_20, 0.0}};
 NP_ALIGNED float64x2_t c0_2[2] = {{b.m_00, b.m_00}, {b.m_00, 0.0}};
 NP_ALIGNED float64x2_t c0_3[2] = {{a.m_01, a.m_11}, {a.m_21, 0.0}};
 NP_ALIGNED float64x2_t c0_4[2] = {{b.m_10, b.m_10}, {b.m_10, 0.0}};
 NP_ALIGNED float64x2_t c0_5[2] = {{a.m_02, a.m_12}, {a.m_22, 0.0}};
 NP_ALIGNED float64x2_t c0_6[2] = {{b.m_20, b.m_20}, {b.m_20, 0.0}};

 vst1q_f64(&m.m_00, vfmaq_f64(vfmaq_f64(vmulq_f64(c0_5[0], c0_6[0]), c0_3[0], c0_4[0]), c0_1[0], c0_2[0]) );
 vst1q_f64(&m.m_20, vfmaq_f64(vfmaq_f64(vmulq_f64(c0_5[1], c0_6[1]), c0_3[1], c0_4[1]) , c0_1[1], c0_2[1]) );

 NP_ALIGNED float64x2_t c1_2[2] = {{b.m_01, b.m_01}, {b.m_01, 0.0}};
 NP_ALIGNED float64x2_t c1_4[2] = {{b.m_11, b.m_11}, {b.m_11, 0.0}};
 NP_ALIGNED float64x2_t c1_6[2] = {{b.m_21, b.m_21}, {b.m_21, 0.0}};
 
 vst1q_f64(&m.m_01, vfmaq_f64(vfmaq_f64(vmulq_f64(c0_5[0], c1_6[0]), c0_3[0], c1_4[0]), c0_1[0], c1_2[0]) );
 vst1q_f64(&m.m_21, vfmaq_f64(vfmaq_f64(vmulq_f64(c0_5[1], c1_6[1]), c0_3[1], c1_4[1]), c0_1[1], c1_2[1]) );

 NP_ALIGNED float64x2_t c2_2[2] = {{b.m_02, b.m_02}, {b.m_02, 0.0}};
 NP_ALIGNED float64x2_t c2_4[2] = {{b.m_12, b.m_12}, {b.m_12, 0.0}};
 NP_ALIGNED float64x2_t c2_6[2] = {{b.m_22, b.m_22}, {b.m_22, 0.0}};
 
 vst1q_f64(&m.m_02, vfmaq_f64(vfmaq_f64(vmulq_f64(c0_5[0], c2_6[0]), c0_3[0], c2_4[0]), c0_1[0], c2_2[0]) );
 vst1q_f64(&m.m_22, vfmaq_f64(vfmaq_f64(vmulq_f64(c0_5[1], c2_6[1]), c0_3[1], c2_4[1]), c0_1[1], c2_2[1]) );

#else

 NP_ALIGNED float32x4_t c0_1 = {a.m_00, a.m_10, a.m_20, 0.0};
 NP_ALIGNED float32x4_t c0_2 = vdupq_n_f32(b.m_00);
 NP_ALIGNED float32x4_t c0_3 = {a.m_01, b.m_11, b.m_21, 0.0};
 NP_ALIGNED float32x4_t c0_4 = vdupq_n_f32(b.m_10);
 NP_ALIGNED float32x4_t c0_5 = {a.m_02, a.m_12, a.m_22, 0.0};
 NP_ALIGNED float32x4_t c0_6 = vdupq_n_f32(b.m_20);

 vst1q_f32(&m.m_00, vfmaq_f32(vfmaq_f32(vmulq_f32(c0_5, c0_6), c0_3, c0_4), c0_1, c0_2) );
 
 NP_ALIGNED float32x4_t c1_2 = vdupq_n_f32(b.m_01);
 NP_ALIGNED float32x4_t c1_4 = vdupq_n_f32(b.m_11);
 NP_ALIGNED float32x4_t c1_6 = vdupq_n_f32(b.m_21);
 
 vst1q_f32(&m.m_01, vfmaq_f32(vfmaq_f32(vmulq_f32(c0_5, c1_6), c0_3, c1_4), c0_1, c1_2) );

 NP_ALIGNED float32x4_t c2_2 = vdupq_n_f32(b.m_02);
 NP_ALIGNED float32x4_t c2_4 = vdupq_n_f32(b.m_12);
 NP_ALIGNED float32x4_t c2_6 = vdupq_n_f32(b.m_22);
 
 vst1q_f32(&m.m_02, vfmaq_f32(vfmaq_f32(vmulq_f32(c0_5, c2_6), c0_3, c2_4), c0_1, c2_2) );

#endif

#else

	m.m_00 = b.m_00 * a.m_00 + b.m_10 * a.m_01 + b.m_20 * a.m_02;
	m.m_10 = b.m_00 * a.m_10 + b.m_10 * a.m_11 + b.m_20 * a.m_12;
	m.m_20 = b.m_00 * a.m_20 + b.m_10 * a.m_21 + b.m_20 * a.m_22;

	m.m_01 = b.m_01 * a.m_00 + b.m_11 * a.m_01 + b.m_21 * a.m_02;
	m.m_11 = b.m_01 * a.m_10 + b.m_11 * a.m_11 + b.m_21 * a.m_12;
	m.m_21 = b.m_01 * a.m_20 + b.m_11 * a.m_21 + b.m_21 * a.m_22;

	m.m_02 = b.m_02 * a.m_00 + b.m_12 * a.m_01 + b.m_22 * a.m_02;
	m.m_12 = b.m_02 * a.m_10 + b.m_12 * a.m_11 + b.m_22 * a.m_12;
	m.m_22 = b.m_02 * a.m_20 + b.m_12 * a.m_21 + b.m_22 * a.m_22;

#endif
 return m;
}


NP_HOT NPMat3x3 NPMat3x3_Invert(NPMat3x3 a) {
 NP_ALIGNED NPMat3x3 m;
 NP_ALIGNED np_real temp[4];
#ifdef NP_HAS_NEON

#ifdef NP_USE_F64

 NP_ALIGNED float64x2_t aa[2] = {{a.m_11, a.m_10}, {a.m_10, 0.0}};
 NP_ALIGNED float64x2_t bb[2] = {{a.m_22, a.m_22}, {a.m_21, 0.0}};
 NP_ALIGNED float64x2_t cc[2] = {{a.m_21, a.m_12}, {a.m_11, 0.0}};
 NP_ALIGNED float64x2_t dd[2] = {{a.m_12, a.m_20}, {a.m_20, 0.0}};
 vst1q_f64(temp, vfmaq_f64(vnegq_f64(vmulq_f64(cc[0], dd[0])), aa[0], bb[0]));
 vst1q_f64(temp+2, vfmaq_f64(vnegq_f64(vmulq_f64(cc[1], dd[1])), aa[1], bb[1]));

	NP_ALIGNED np_real inv_det = (a.m_00 * temp[0] - a.m_01 * temp[1] + a.m_02 * temp[2]);

 if(NP_UNLIKELY(NP_Abs(inv_det) <= 1e-4f)) {
  vst1q_f64(&m.m_00, vdupq_n_f64(NP_Nan()));
  vst1q_f64(&m.m_20, vdupq_n_f64(NP_Nan()));
  vst1q_f64(&m.m_01, vdupq_n_f64(NP_Nan()));
  vst1q_f64(&m.m_21, vdupq_n_f64(NP_Nan()));
  vst1q_f64(&m.m_02, vdupq_n_f64(NP_Nan()));
  vst1q_f64(&m.m_22, vdupq_n_f64(NP_Nan()));
  return m;
 }
 inv_det = 1.0f / inv_det;
 
 NP_ALIGNED float64x2_t inv_det_vec = vdupq_n_f64(inv_det);
 NP_ALIGNED float64x2_t c0_1[2] = {{temp[0], -temp[1]}, {temp[2], 0.0}};

 vst1q_f64(&m.m_00, vmulq_f64(inv_det_vec, c0_1[0]));
 vst1q_f64(&m.m_20, vmulq_f64(inv_det_vec, c0_1[1]));

 NP_ALIGNED float64x2_t c1_1[2] = {{a.m_01, a.m_00}, {a.m_00, 0.0}};
 NP_ALIGNED float64x2_t c1_2[2] = {{a.m_22, a.m_22}, {a.m_21, 0.0}};
 NP_ALIGNED float64x2_t c1_3[2] = {{a.m_02, a.m_02}, {a.m_02, 0.0}};
 NP_ALIGNED float64x2_t c1_4[2] = {{a.m_21, a.m_20}, {a.m_20, 0.0}};

 vst1q_f64(&m.m_01, vmulq_f64(vfmaq_f64(vnegq_f64(vmulq_f64(c1_3[0], c1_4[0])), c1_1[0], c1_2[0]), inv_det_vec));
 vst1q_f64(&m.m_21, vmulq_f64(vfmaq_f64(vnegq_f64(vmulq_f64(c1_3[1], c1_4[1])), c1_1[1], c1_2[1]), inv_det_vec));

 m.m_01 = -m.m_01;
 m.m_21 = -m.m_21;

 NP_ALIGNED float64x2_t c2_1[2] = {{a.m_01, a.m_00}, {a.m_00, 0.0}};
 NP_ALIGNED float64x2_t c2_2[2] = {{a.m_12, a.m_12}, {a.m_11, 0.0}};
 NP_ALIGNED float64x2_t c2_3[2] = {{a.m_02, a.m_02}, {a.m_10, 0.0}};
 NP_ALIGNED float64x2_t c2_4[2] = {{a.m_11, a.m_10}, {a.m_01, 0.0}};

 vst1q_f64(&m.m_02, vmulq_f64(vfmaq_f64(vnegq_f64(vmulq_f64(c2_3[0], c2_4[0])), c2_1[0], c2_2[0]), inv_det_vec));
 vst1q_f64(&m.m_22, vmulq_f64(vfmaq_f64(vnegq_f64(vmulq_f64(c2_3[1], c2_4[1])), c2_1[1], c2_2[1]), inv_det_vec));
 m.m_12 = -m.m_12;

#else
 
 NP_ALIGNED float32x4_t aa = {a.m_11, a.m_10, a.m_10, 0.0};
 NP_ALIGNED float32x4_t bb = {a.m_22, a.m_22, a.m_21, 0.0};
 NP_ALIGNED float32x4_t cc = {a.m_21, a.m_12, a.m_11, 0.0};
 NP_ALIGNED float32x4_t dd = {a.m_12, a.m_20, a.m_20, 0.0};
 vst1q_f32(temp, vfmaq_f32(vnegq_f32(vmulq_f32(cc, dd)), aa, bb) );
	NP_ALIGNED np_real inv_det = (a.m_00 * temp[0] - a.m_01 * temp[1] + a.m_02 * temp[2]);

 if(NP_UNLIKELY(NP_Abs(inv_det) <= 1e-4f)) {
  vst1q_f32(&m.m_00, vdupq_n_f32(NP_Nan()));
  vst1q_f32(&m.m_01, vdupq_n_f32(NP_Nan()));
  vst1q_f32(&m.m_02, vdupq_n_f32(NP_Nan()));
  return m;
 }
 inv_det = 1.0f / inv_det;
 
 NP_ALIGNED float32x4_t inv_det_vec = vdupq_n_f32(inv_det);
 NP_ALIGNED float32x4_t c0_1 = {temp[0], -temp[1], temp[2], 0.0};

 vst1q_f32(&m.m_00, vmulq_f32(inv_det_vec, c0_1));

 NP_ALIGNED float32x4_t c1_1 = {a.m_01, a.m_00, a.m_00, 0.0};
 NP_ALIGNED float32x4_t c1_2 = {a.m_22, a.m_22, a.m_21, 0.0};
 NP_ALIGNED float32x4_t c1_3 = {a.m_02, a.m_02, a.m_02, 0.0};
 NP_ALIGNED float32x4_t c1_4 = {a.m_21, a.m_20, a.m_20, 0.0};

 vst1q_f32(&m.m_01, vmulq_f32(vfmaq_f32(vnegq_f32(vmulq_f32(c1_3, c1_4)), c1_1, c1_2), inv_det_vec));

 m.m_01 = -m.m_01;
 m.m_21 = -m.m_21;

 NP_ALIGNED float32x4_t c2_1 = {a.m_01, a.m_00, a.m_00, 0.0};
 NP_ALIGNED float32x4_t c2_2 = {a.m_12, a.m_12, a.m_11, 0.0};
 NP_ALIGNED float32x4_t c2_3 = {a.m_02, a.m_02, a.m_10, 0.0};
 NP_ALIGNED float32x4_t c2_4 = {a.m_11, a.m_10, a.m_01, 0.0};

 vst1q_f32(&m.m_02, vmulq_f32(vfmaq_f32(vnegq_f32(vmulq_f32(c2_3, c2_4)), c2_1, c2_2), inv_det_vec));

 m.m_12 = -m.m_12;

#endif

#else
 temp[0] = a.m_11 * a.m_22 - a.m_21 * a.m_12;
	temp[1] = a.m_10 * a.m_22 - a.m_12 * a.m_20;
	temp[2] = a.m_10 * a.m_21 - a.m_11 * a.m_20;

	np_real inv_det = (a.m_00 * temp[0] - a.m_01 * temp[1] + a.m_02 * temp[2]);

 if(NP_UNLIKELY(NP_Abs(inv_det) <= 1e-4f)) {
  m.m_00 = NP_Nan();
  m.m_01 = NP_Nan();
  m.m_02 = NP_Nan();

  m.m_10 = NP_Nan();
  m.m_11 = NP_Nan();
  m.m_12 = NP_Nan();

  m.m_20 = NP_Nan();
  m.m_21 = NP_Nan();
  m.m_22 = NP_Nan();
  return m;
 }

 inv_det = 1.0f / inv_det;
 
	m.m_00 = (temp[0]) * inv_det;
 m.m_01 = -(a.m_01 * a.m_22 - a.m_02 * a.m_21) * inv_det;
 m.m_02 = (a.m_01 * a.m_12 - a.m_02 * a.m_11) * inv_det;
 
 m.m_10 = -(temp[1]) * inv_det;
 m.m_11 = (a.m_00 * a.m_22 - a.m_02 * a.m_20) * inv_det;
 m.m_12 = -(a.m_00 * a.m_12 - a.m_02 * a.m_10) * inv_det;
 
 m.m_20 = (temp[2]) * inv_det;
 m.m_21 = -(a.m_00 * a.m_21 - a.m_02 * a.m_20) * inv_det;
 m.m_22 = (a.m_00 * a.m_11 - a.m_10 * a.m_01) * inv_det;
#endif
 return m;
}



NP_HOT NPMat3x3 NPMat3x3_Transpose(NPMat3x3 a) {
 NPMat3x3 m;
	m.m_00 = a.m_00;
	m.m_10 = a.m_01;
	m.m_20 = a.m_02;
	
	m.m_01 = a.m_10;
	m.m_11 = a.m_11;
	m.m_21 = a.m_12;
	
	m.m_02 = a.m_20;
	m.m_12 = a.m_21;
	m.m_22 = a.m_22;
	return m;
}


NP_HOT NPReal3 NPMat3x3_MulVec(NPMat3x3 a, NPReal3 b) {
 NP_ALIGNED NPReal3 v;
#ifdef NP_HAS_NEON

#ifdef NP_USE_F64

 NP_ALIGNED float64x2_t aa[2] = {{a.m_00, a.m_10}, {a.m_20, 0.0}};
 NP_ALIGNED float64x2_t bb[2] = {{b.x, b.x}, {b.x, 0.0}};
 NP_ALIGNED float64x2_t cc[2] = {{a.m_01, a.m_11}, {a.m_21, 0.0}};
 NP_ALIGNED float64x2_t dd[2] = {{b.y, b.y}, {b.y, 0.0}};
 NP_ALIGNED float64x2_t ee[2] = {{a.m_02, a.m_12}, {a.m_22, 0.0}};
 NP_ALIGNED float64x2_t ff[2] = {{b.z, b.z}, {b.z, 0.0}};

 vst1q_f64((np_real*)&v, vfmaq_f64(vfmaq_f64(vmulq_f64(ee[0], ff[0]), cc[0], dd[0]), aa[0], bb[0]));
 vst1q_f64((np_real*)&v.z, vfmaq_f64(vfmaq_f64(vmulq_f64(ee[1], ff[1]), cc[1], dd[1]), aa[1], bb[1]));

#else

 NP_ALIGNED float32x4_t aa = {a.m_00, a.m_10, a.m_20, 0.0};
 NP_ALIGNED float32x4_t bb = vdupq_n_f32(b.x);
 NP_ALIGNED float32x4_t cc = {a.m_01, a.m_11, a.m_21, 0.0};
 NP_ALIGNED float32x4_t dd = vdupq_n_f32(b.y);
 NP_ALIGNED float32x4_t ee = {a.m_02, a.m_12, a.m_22, 0.0};
 NP_ALIGNED float32x4_t ff = vdupq_n_f32(b.z);

 vst1q_f32((np_real*)&v, vfmaq_f32(vfmaq_f32(vmulq_f32(ee, ff), cc, dd), aa, bb));
#endif

#else
	v.x = a.m_00 * b.x + a.m_01 * b.y + a.m_02 * b.z;
 v.y = a.m_10 * b.x + a.m_11 * b.y + a.m_12 * b.z;
	v.z = a.m_20 * b.x + a.m_21 * b.y + a.m_22 * b.z;
#endif
 return v;
}





