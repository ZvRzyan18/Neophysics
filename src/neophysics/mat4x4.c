#include "neophysics/mat4x4.h"
#include "neophysics/math.h"
#include <stdalign.h>

/*
 //print matrix code
void pm4(NPMat4x4 m) {
	for(int i = 0; i < 16; i++) {
	 if(((i % 4) == 0)) {
	 	printf("\n");
	 }
	 printf("%.2f, ", *(((np_real*)&m) + i));
	}
}

*/

/*
 Column Major Matrix
*/
NP_ALIGNED const NPMat4x4 NPMat4x4_Identity = {
	1.0f, 0.0f, 0.0f, 0.0f,
	0.0f, 1.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 1.0f, 0.0f,
	0.0f, 0.0f, 0.0f, 1.0f,
};


NP_HOT NPMat4x4 NPMat4x4_FromQuaternion(NPReal4 q) {
 NP_ALIGNED NPMat4x4 m;
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

 m.m_03 = 0.0f;
 m.m_13 = 0.0f;
 m.m_23 = 0.0f;
 m.m_33 = 1.0f;

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

 m.m_03 = 0.0f;
 m.m_13 = 0.0f;
 m.m_23 = 0.0f;
 m.m_33 = 1.0f;

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
 m.m_30 = 0.0f;

 m.m_01 =     2.0f * (tmp[1] + tmp[8]);
 m.m_11 = 1.0f - 2.0f * (tmp[0] + tmp[7]);
 m.m_21 =     2.0f * (tmp[5] - tmp[3]);
 m.m_31 = 0.0f;

 m.m_02 =     2.0f * (tmp[2] - tmp[6]);
 m.m_12 =     2.0f * (tmp[5] + tmp[3]);
 m.m_22 = 1.0f - 2.0f * (tmp[0] + tmp[4]);
 m.m_32 = 0.0f;
 
 m.m_03 = 0.0f;
 m.m_13 = 0.0f;
 m.m_23 = 0.0f;
 m.m_33 = 1.0f;
#endif
 return m;
}


NP_HOT NPMat4x4 NPMat4x4_Mul(NPMat4x4 a, NPMat4x4 b) {
 NP_ALIGNED	NPMat4x4 m;

#ifdef NP_HAS_NEON

#ifdef NP_USE_F64

 NP_ALIGNED float64x2_t c0_1[2] = {{a.m_00, a.m_10}, {a.m_20, a.m_30}};
 NP_ALIGNED float64x2_t c0_2[2] = {{b.m_00, b.m_00}, {b.m_00, b.m_00}};
 NP_ALIGNED float64x2_t c0_3[2] = {{a.m_01, b.m_11}, {b.m_21, a.m_31}};
 NP_ALIGNED float64x2_t c0_4[2] = {{b.m_10, b.m_10}, {b.m_10, b.m_10}};
 NP_ALIGNED float64x2_t c0_5[2] = {{a.m_02, a.m_12}, {a.m_22, a.m_32}};
 NP_ALIGNED float64x2_t c0_6[2] = {{b.m_20, b.m_20}, {b.m_20, b.m_20}};
 NP_ALIGNED float64x2_t c0_7[2] = {{a.m_03, a.m_13}, {a.m_23, a.m_33}};
 NP_ALIGNED float64x2_t c0_8[2] = {{b.m_30, b.m_30}, {b.m_30, b.m_30}};

 vst1q_f64(&m.m_00, vfmaq_f64(vfmaq_f64(vfmaq_f64(vmulq_f64(c0_7[0], c0_8[0]), c0_5[0], c0_6[0]), c0_3[0], c0_4[0]), c0_1[0], c0_2[0]) );
 vst1q_f64(&m.m_20, vfmaq_f64(vfmaq_f64(vfmaq_f64(vmulq_f64(c0_7[1], c0_8[1]), c0_5[1], c0_6[1]), c0_3[1], c0_4[1]), c0_1[1], c0_2[1]) );

 NP_ALIGNED float64x2_t c1_2[2] = {{b.m_01, b.m_01}, {b.m_01, b.m_01}};
 NP_ALIGNED float64x2_t c1_4[2] = {{b.m_11, b.m_11}, {b.m_11, b.m_11}};
 NP_ALIGNED float64x2_t c1_6[2] = {{b.m_21, b.m_21}, {b.m_21, b.m_21}};
 NP_ALIGNED float64x2_t c1_8[2] = {{b.m_31, b.m_31}, {b.m_31, b.m_31}};

 vst1q_f64(&m.m_01, vfmaq_f64(vfmaq_f64(vfmaq_f64(vmulq_f64(c0_7[0], c1_8[0]), c0_5[0], c1_6[0]), c0_3[0], c1_4[0]), c0_1[0], c1_2[0]) );
 vst1q_f64(&m.m_21, vfmaq_f64(vfmaq_f64(vfmaq_f64(vmulq_f64(c0_7[1], c1_8[1]), c0_5[1], c1_6[1]), c0_3[1], c1_4[1]), c0_1[1], c1_2[1]) );

 NP_ALIGNED float64x2_t c2_2[2] = {{b.m_02, b.m_02}, {b.m_02, b.m_02}};
 NP_ALIGNED float64x2_t c2_4[2] = {{b.m_12, b.m_12}, {b.m_12, b.m_12}};
 NP_ALIGNED float64x2_t c2_6[2] = {{b.m_22, b.m_22}, {b.m_22, b.m_22}};
 NP_ALIGNED float64x2_t c2_8[2] = {{b.m_32, b.m_32}, {b.m_32, b.m_32}};

 vst1q_f64(&m.m_02, vfmaq_f64(vfmaq_f64(vfmaq_f64(vmulq_f64(c0_7[0], c2_8[0]), c0_5[0], c2_6[0]), c0_3[0], c2_4[0]), c0_1[0], c2_2[0]) );
 vst1q_f64(&m.m_22, vfmaq_f64(vfmaq_f64(vfmaq_f64(vmulq_f64(c0_7[1], c2_8[1]), c0_5[1], c2_6[1]), c0_3[1], c2_4[1]), c0_1[1], c2_2[1]) );

 NP_ALIGNED float64x2_t c3_2[2] = {{b.m_03, b.m_03}, {b.m_03, b.m_03}};
 NP_ALIGNED float64x2_t c3_4[2] = {{b.m_13, b.m_13}, {b.m_13, b.m_13}};
 NP_ALIGNED float64x2_t c3_6[2] = {{b.m_23, b.m_23}, {b.m_23, b.m_23}};
 NP_ALIGNED float64x2_t c3_8[2] = {{b.m_33, b.m_33}, {b.m_33, b.m_33}};

 vst1q_f64(&m.m_03, vfmaq_f64(vfmaq_f64(vfmaq_f64(vmulq_f64(c0_7[0], c3_8[0]), c0_5[0], c3_6[0]), c0_3[0], c3_4[0]), c0_1[0], c3_2[0]) );
 vst1q_f64(&m.m_23, vfmaq_f64(vfmaq_f64(vfmaq_f64(vmulq_f64(c0_7[1], c3_8[1]), c0_5[1], c3_6[1]), c0_3[1], c3_4[1]), c0_1[1], c3_2[1]) );

#else

 NP_ALIGNED float32x4_t c0_1 = {a.m_00, a.m_10, a.m_20, a.m_30};
 NP_ALIGNED float32x4_t c0_2 = vdupq_n_f32(b.m_00);
 NP_ALIGNED float32x4_t c0_3 = {a.m_01, b.m_11, b.m_21, a.m_31};
 NP_ALIGNED float32x4_t c0_4 = vdupq_n_f32(b.m_10);
 NP_ALIGNED float32x4_t c0_5 = {a.m_02, a.m_12, a.m_22, a.m_32};
 NP_ALIGNED float32x4_t c0_6 = vdupq_n_f32(b.m_20);
 NP_ALIGNED float32x4_t c0_7 = {a.m_03, a.m_13, a.m_23, a.m_33};
 NP_ALIGNED float32x4_t c0_8 = vdupq_n_f32(b.m_30);

 vst1q_f32(&m.m_00, vfmaq_f32(vfmaq_f32(vfmaq_f32(vmulq_f32(c0_7, c0_8), c0_5, c0_6), c0_3, c0_4), c0_1, c0_2) );
 
 NP_ALIGNED float32x4_t c1_2 = vdupq_n_f32(b.m_01);
 NP_ALIGNED float32x4_t c1_4 = vdupq_n_f32(b.m_11);
 NP_ALIGNED float32x4_t c1_6 = vdupq_n_f32(b.m_21);
 NP_ALIGNED float32x4_t c1_8 = vdupq_n_f32(b.m_31);

 vst1q_f32(&m.m_01, vfmaq_f32(vfmaq_f32(vfmaq_f32(vmulq_f32(c0_7, c1_8), c0_5, c1_6), c0_3, c1_4), c0_1, c1_2) );

 NP_ALIGNED float32x4_t c2_2 = vdupq_n_f32(b.m_02);
 NP_ALIGNED float32x4_t c2_4 = vdupq_n_f32(b.m_12);
 NP_ALIGNED float32x4_t c2_6 = vdupq_n_f32(b.m_22);
 NP_ALIGNED float32x4_t c2_8 = vdupq_n_f32(b.m_32);

 vst1q_f32(&m.m_02, vfmaq_f32(vfmaq_f32(vfmaq_f32(vmulq_f32(c0_7, c2_8), c0_5, c2_6), c0_3, c2_4), c0_1, c2_2) );

 NP_ALIGNED float32x4_t c3_2 = vdupq_n_f32(b.m_03);
 NP_ALIGNED float32x4_t c3_4 = vdupq_n_f32(b.m_13);
 NP_ALIGNED float32x4_t c3_6 = vdupq_n_f32(b.m_23);
 NP_ALIGNED float32x4_t c3_8 = vdupq_n_f32(b.m_33);

 vst1q_f32(&m.m_03, vfmaq_f32(vfmaq_f32(vfmaq_f32(vmulq_f32(c0_7, c3_8), c0_5, c3_6), c0_3, c3_4), c0_1, c3_2) );

#endif

#else
	m.m_00 = b.m_00 * a.m_00 + b.m_10 * a.m_01 + b.m_20 * a.m_02 + b.m_30 * a.m_03;
	m.m_10 = b.m_00 * a.m_10 + b.m_10 * a.m_11 + b.m_20 * a.m_12 + b.m_30 * a.m_13;
	m.m_20 = b.m_00 * a.m_20 + b.m_10 * a.m_21 + b.m_20 * a.m_22 + b.m_30 * a.m_23;
	m.m_30 = b.m_00 * a.m_30 + b.m_10 * a.m_31 + b.m_20 * a.m_32 + b.m_30 * a.m_33;
	
	m.m_01 = b.m_01 * a.m_00 + b.m_11 * a.m_01 + b.m_21 * a.m_02 + b.m_31 * a.m_03;
	m.m_11 = b.m_01 * a.m_10 + b.m_11 * a.m_11 + b.m_21 * a.m_12 + b.m_31 * a.m_13;
	m.m_21 = b.m_01 * a.m_20 + b.m_11 * a.m_21 + b.m_21 * a.m_22 + b.m_31 * a.m_23;
	m.m_31 = b.m_01 * a.m_30 + b.m_11 * a.m_31 + b.m_21 * a.m_32 + b.m_31 * a.m_33;

	m.m_02 = b.m_02 * a.m_00 + b.m_12 * a.m_01 + b.m_22 * a.m_02 + b.m_32 * a.m_03;
	m.m_12 = b.m_02 * a.m_10 + b.m_12 * a.m_11 + b.m_22 * a.m_12 + b.m_32 * a.m_13;
	m.m_22 = b.m_02 * a.m_20 + b.m_12 * a.m_21 + b.m_22 * a.m_22 + b.m_32 * a.m_23;
	m.m_32 = b.m_02 * a.m_30 + b.m_12 * a.m_31 + b.m_22 * a.m_32 + b.m_32 * a.m_33;

	m.m_03 = b.m_03 * a.m_00 + b.m_13 * a.m_01 + b.m_23 * a.m_02 + b.m_33 * a.m_03;
	m.m_13 = b.m_03 * a.m_10 + b.m_13 * a.m_11 + b.m_23 * a.m_12 + b.m_33 * a.m_13;
	m.m_23 = b.m_03 * a.m_20 + b.m_13 * a.m_21 + b.m_23 * a.m_22 + b.m_33 * a.m_23;
	m.m_33 = b.m_03 * a.m_30 + b.m_13 * a.m_31 + b.m_23 * a.m_32 + b.m_33 * a.m_33;
#endif
 return m;
}


NP_HOT NPMat4x4 NPMat4x4_Invert(NPMat4x4 a) {
 NP_ALIGNED NPMat4x4 m;
 NP_ALIGNED np_real temp[12];
#ifdef NP_HAS_NEON

#ifdef NP_USE_F64

 NP_ALIGNED float64x2_t aa[2] = {{a.m_00, a.m_00}, {a.m_00, a.m_01}};
 NP_ALIGNED float64x2_t bb[2] = {{a.m_11, a.m_12}, {a.m_13, a.m_12}};
 NP_ALIGNED float64x2_t cc[2] = {{a.m_10, a.m_10}, {a.m_10, a.m_11}};
 NP_ALIGNED float64x2_t dd[2] = {{a.m_01, a.m_02}, {a.m_03, a.m_02}};

 vst1q_f64(&temp[0], vfmaq_f64(vnegq_f64(vmulq_f64(cc[0], dd[0])), aa[0], bb[0]));
 vst1q_f64(&temp[2], vfmaq_f64(vnegq_f64(vmulq_f64(cc[1], dd[1])), aa[1], bb[1]));

 NP_ALIGNED float64x2_t ee[2] = {{a.m_01, a.m_02}, {a.m_22, a.m_21}};
 NP_ALIGNED float64x2_t ff[2] = {{a.m_13, a.m_13}, {a.m_33, a.m_33}};
 NP_ALIGNED float64x2_t gg[2] = {{a.m_11, a.m_12}, {a.m_32, a.m_31}};
 NP_ALIGNED float64x2_t hh[2] = {{a.m_03, a.m_03}, {a.m_23, a.m_23}};

 vst1q_f64(&temp[4], vfmaq_f64(vnegq_f64(vmulq_f64(gg[0], hh[0])), ee[0], ff[0]));
 vst1q_f64(&temp[6], vfmaq_f64(vnegq_f64(vmulq_f64(gg[1], hh[1])), ee[1], ff[1]));

 NP_ALIGNED float64x2_t ii[2] = {{a.m_21, a.m_20}, {a.m_20, a.m_20}};
 NP_ALIGNED float64x2_t jj[2] = {{a.m_32, a.m_33}, {a.m_32, a.m_31}};
 NP_ALIGNED float64x2_t kk[2] = {{a.m_31, a.m_30}, {a.m_30, a.m_30}};
 NP_ALIGNED float64x2_t ll[2] = {{a.m_22, a.m_23}, {a.m_22, a.m_21}};

 vst1q_f64(&temp[8], vfmaq_f64(vnegq_f64(vmulq_f64(kk[0], ll[0])), ii[0], jj[0]));
 vst1q_f64(&temp[10], vfmaq_f64(vnegq_f64(vmulq_f64(kk[1], ll[1])), ii[1], jj[1]));

 np_real inv_det = (temp[0] * temp[6] - temp[1] * temp[7] + temp[2] * temp[8] + temp[3] * temp[9] - temp[4] * temp[10] + temp[5] * temp[11]);

 if(NP_UNLIKELY(NP_Abs(inv_det) <= 1e-4f)) {
  vst1q_f64(&m.m_00, vdupq_n_f64(NP_Nan()));
  vst1q_f64(&m.m_01, vdupq_n_f64(NP_Nan()));
  vst1q_f64(&m.m_02, vdupq_n_f64(NP_Nan()));
  vst1q_f64(&m.m_03, vdupq_n_f64(NP_Nan()));
  return m;
 }

 inv_det = 1.0 / inv_det;
 NP_ALIGNED float64x2_t inv_det_vec = vdupq_n_f64(inv_det);
 
 NP_ALIGNED float64x2_t c0_1[2] = {{a.m_11, -a.m_10}, {a.m_10, -a.m_10}};
 NP_ALIGNED float64x2_t c0_2[2] = {{temp[6], temp[6]}, {temp[7], temp[8]}};
 NP_ALIGNED float64x2_t c0_3[2] = {{-a.m_12, a.m_12}, {-a.m_11, a.m_11}};
 NP_ALIGNED float64x2_t c0_4[2] = {{temp[7], temp[9]}, {temp[9], temp[10]}};
 NP_ALIGNED float64x2_t c0_5[2] = {{a.m_13, -a.m_13}, {a.m_13, -a.m_12}};
 NP_ALIGNED float64x2_t c0_6[2] = {{temp[8], temp[10]}, {temp[11], temp[11]}};

 vst1q_f64(&m.m_00, vmulq_f64(vfmaq_f64(vfmaq_f64(vmulq_f64(c0_5[0], c0_6[0]), c0_3[0], c0_4[0]), c0_1[0], c0_2[0]), inv_det_vec));
 vst1q_f64(&m.m_20, vmulq_f64(vfmaq_f64(vfmaq_f64(vmulq_f64(c0_5[1], c0_6[1]), c0_3[1], c0_4[1]), c0_1[1], c0_2[1]), inv_det_vec));

 NP_ALIGNED float64x2_t c1_1[2] = {{-a.m_01, a.m_00}, {-a.m_00, a.m_00}};
 NP_ALIGNED float64x2_t c1_3[2] = {{a.m_02, -a.m_02}, {a.m_01, -a.m_01}};
 NP_ALIGNED float64x2_t c1_5[2] = {{-a.m_03, a.m_03}, {-a.m_03, a.m_02}};

 vst1q_f64(&m.m_01, vmulq_f64(vfmaq_f64(vfmaq_f64(vmulq_f64(c1_5[0], c0_6[0]), c1_3[0], c0_4[0]), c1_1[0], c0_2[0]), inv_det_vec));
 vst1q_f64(&m.m_21, vmulq_f64(vfmaq_f64(vfmaq_f64(vmulq_f64(c1_5[1], c0_6[1]), c1_3[1], c0_4[1]), c1_1[1], c0_2[1]), inv_det_vec));

 NP_ALIGNED float64x2_t c2_1[2] = {{a.m_31, -a.m_30}, {a.m_30, -a.m_30}};
 NP_ALIGNED float64x2_t c2_2[2] = {{temp[5], temp[5]}, {temp[4], temp[3]}};
 NP_ALIGNED float64x2_t c2_3[2] = {{-a.m_32, a.m_32}, {-a.m_31, a.m_31}};
 NP_ALIGNED float64x2_t c2_4[2] = {{temp[4], temp[2]}, {temp[2], temp[1]}};
 NP_ALIGNED float64x2_t c2_5[2] = {{a.m_33, -a.m_33}, {a.m_33, -a.m_32}};
 NP_ALIGNED float64x2_t c2_6[2] = {{temp[3], temp[1]}, {temp[0], temp[0]}};

 vst1q_f64(&m.m_02, vmulq_f64(vfmaq_f64(vfmaq_f64(vmulq_f64(c2_5[0], c2_6[0]), c2_3[0], c2_4[0]), c2_1[0], c2_2[0]), inv_det_vec));
 vst1q_f64(&m.m_22, vmulq_f64(vfmaq_f64(vfmaq_f64(vmulq_f64(c2_5[1], c2_6[1]), c2_3[1], c2_4[1]), c2_1[1], c2_2[1]), inv_det_vec));

 NP_ALIGNED float64x2_t c3_1[2] = {{-a.m_21, a.m_20}, {-a.m_20, a.m_20}};
 NP_ALIGNED float64x2_t c3_3[2] = {{a.m_22, -a.m_22}, {a.m_21, -a.m_21}};
 NP_ALIGNED float64x2_t c3_5[2] = {{-a.m_23, a.m_23}, {-a.m_23, a.m_22}};

 vst1q_f64(&m.m_03, vmulq_f64(vfmaq_f64(vfmaq_f64(vmulq_f64(c3_5[0], c2_6[0]), c3_3[0], c2_4[0]), c3_1[0], c2_2[0]), inv_det_vec));
 vst1q_f64(&m.m_23, vmulq_f64(vfmaq_f64(vfmaq_f64(vmulq_f64(c3_5[1], c2_6[1]), c3_3[1], c2_4[1]), c3_1[1], c2_2[1]), inv_det_vec));

#else

 NP_ALIGNED float32x4_t aa = {a.m_00, a.m_00, a.m_00, a.m_01};
 NP_ALIGNED float32x4_t bb = {a.m_11, a.m_12, a.m_13, a.m_12};
 NP_ALIGNED float32x4_t cc = {a.m_10, a.m_10, a.m_10, a.m_11};
 NP_ALIGNED float32x4_t dd = {a.m_01, a.m_02, a.m_03, a.m_02};

 vst1q_f32(&temp[0], vfmaq_f32(vnegq_f32(vmulq_f32(cc, dd)), aa, bb));

 NP_ALIGNED float32x4_t ee = {a.m_01, a.m_02, a.m_22, a.m_21};
 NP_ALIGNED float32x4_t ff = {a.m_13, a.m_13, a.m_33, a.m_33};
 NP_ALIGNED float32x4_t gg = {a.m_11, a.m_12, a.m_32, a.m_31};
 NP_ALIGNED float32x4_t hh = {a.m_03, a.m_03, a.m_23, a.m_23};

 vst1q_f32(&temp[4], vfmaq_f32(vnegq_f32(vmulq_f32(gg, hh)), ee, ff));

 NP_ALIGNED float32x4_t ii = {a.m_21, a.m_20, a.m_20, a.m_20};
 NP_ALIGNED float32x4_t jj = {a.m_32, a.m_33, a.m_32, a.m_31};
 NP_ALIGNED float32x4_t kk = {a.m_31, a.m_30, a.m_30, a.m_30};
 NP_ALIGNED float32x4_t ll = {a.m_22, a.m_23, a.m_22, a.m_21};

 vst1q_f32(&temp[8], vfmaq_f32(vnegq_f32(vmulq_f32(kk, ll)), ii, jj));

 np_real inv_det = (temp[0] * temp[6] - temp[1] * temp[7] + temp[2] * temp[8] + temp[3] * temp[9] - temp[4] * temp[10] + temp[5] * temp[11]);

 if(NP_UNLIKELY(NP_Abs(inv_det) <= 1e-4f)) {
  vst1q_f32(&m.m_00, vdupq_n_f32(NP_Nan()));
  vst1q_f32(&m.m_01, vdupq_n_f32(NP_Nan()));
  vst1q_f32(&m.m_02, vdupq_n_f32(NP_Nan()));
  vst1q_f32(&m.m_03, vdupq_n_f32(NP_Nan()));
  return m;
 }

 inv_det = 1.0f / inv_det;
 NP_ALIGNED float32x4_t inv_det_vec = vdupq_n_f32(inv_det);
 
 NP_ALIGNED float32x4_t c0_1 = {a.m_11, -a.m_10, a.m_10, -a.m_10};
 NP_ALIGNED float32x4_t c0_2 = {temp[6], temp[6], temp[7], temp[8]};
 NP_ALIGNED float32x4_t c0_3 = {-a.m_12, a.m_12, -a.m_11, a.m_11};
 NP_ALIGNED float32x4_t c0_4 = {temp[7], temp[9], temp[9], temp[10]};
 NP_ALIGNED float32x4_t c0_5 = {a.m_13, -a.m_13, a.m_13, -a.m_12};
 NP_ALIGNED float32x4_t c0_6 = {temp[8], temp[10], temp[11], temp[11]};

 vst1q_f32(&m.m_00, vmulq_f32(vfmaq_f32(vfmaq_f32(vmulq_f32(c0_5, c0_6), c0_3, c0_4), c0_1, c0_2), inv_det_vec));

 NP_ALIGNED float32x4_t c1_1 = {-a.m_01, a.m_00, -a.m_00, a.m_00};
 NP_ALIGNED float32x4_t c1_3 = {a.m_02, -a.m_02, a.m_01, -a.m_01};
 NP_ALIGNED float32x4_t c1_5 = {-a.m_03, a.m_03, -a.m_03, a.m_02};

 vst1q_f32(&m.m_01, vmulq_f32(vfmaq_f32(vfmaq_f32(vmulq_f32(c1_5, c0_6), c1_3, c0_4), c1_1, c0_2), inv_det_vec));

 NP_ALIGNED float32x4_t c2_1 = {a.m_31, -a.m_30, a.m_30, -a.m_30};
 NP_ALIGNED float32x4_t c2_2 = {temp[5], temp[5], temp[4], temp[3]};
 NP_ALIGNED float32x4_t c2_3 = {-a.m_32, a.m_32, -a.m_31, a.m_31};
 NP_ALIGNED float32x4_t c2_4 = {temp[4], temp[2], temp[2], temp[1]};
 NP_ALIGNED float32x4_t c2_5 = {a.m_33, -a.m_33, a.m_33, -a.m_32};
 NP_ALIGNED float32x4_t c2_6 = {temp[3], temp[1], temp[0], temp[0]};

 vst1q_f32(&m.m_02, vmulq_f32(vfmaq_f32(vfmaq_f32(vmulq_f32(c2_5, c2_6), c2_3, c2_4), c2_1, c2_2), inv_det_vec));

 NP_ALIGNED float32x4_t c3_1 = {-a.m_21, a.m_20, -a.m_20, a.m_20};
 NP_ALIGNED float32x4_t c3_3 = {a.m_22, -a.m_22, a.m_21, -a.m_21};
 NP_ALIGNED float32x4_t c3_5 = {-a.m_23, a.m_23, -a.m_23, a.m_22};

 vst1q_f32(&m.m_03, vmulq_f32(vfmaq_f32(vfmaq_f32(vmulq_f32(c3_5, c2_6), c3_3, c2_4), c3_1, c2_2), inv_det_vec));

#endif

#else
	temp[0] = a.m_00 * a.m_11 - a.m_10 * a.m_01;
	temp[1] = a.m_00 * a.m_12 - a.m_10 * a.m_02;
	temp[2] = a.m_00 * a.m_13 - a.m_10 * a.m_03;
	temp[3] = a.m_01 * a.m_12 - a.m_11 * a.m_02;
	
	temp[4] = a.m_01 * a.m_13 - a.m_11 * a.m_03;
	temp[5] = a.m_02 * a.m_13 - a.m_12 * a.m_03;
	temp[6] = a.m_22 * a.m_33 - a.m_32 * a.m_23;
	temp[7] = a.m_21 * a.m_33 - a.m_31 * a.m_23;
	
	temp[8] = a.m_21 * a.m_32 - a.m_31 * a.m_22;
	temp[9] = a.m_20 * a.m_33 - a.m_30 * a.m_23;
	temp[10] =a.m_20 * a.m_32 - a.m_30 * a.m_22;
	temp[11] =a.m_20 * a.m_31 - a.m_30 * a.m_21;

 np_real inv_det = (temp[0] * temp[6] - temp[1] * temp[7] + temp[2] * temp[8] + temp[3] * temp[9] - temp[4] * temp[10] + temp[5] * temp[11]);

 if(NP_UNLIKELY(NP_Abs(inv_det) <= 1e-4f)) {
  m.m_00 = NP_Nan();
  m.m_01 = NP_Nan();
  m.m_02 = NP_Nan();
  m.m_03 = NP_Nan();

  m.m_10 = NP_Nan();
  m.m_11 = NP_Nan();
  m.m_12 = NP_Nan();
  m.m_13 = NP_Nan();

  m.m_20 = NP_Nan();
  m.m_21 = NP_Nan();
  m.m_22 = NP_Nan();
  m.m_23 = NP_Nan();

  m.m_30 = NP_Nan();
  m.m_31 = NP_Nan();
  m.m_32 = NP_Nan();
  m.m_33 = NP_Nan();
  return m;
 }

 inv_det = 1.0f / inv_det;
 
 m.m_00 = (a.m_11 * temp[6] - a.m_12 * temp[7] + a.m_13 * temp[8]) * inv_det;
 m.m_01 = (-a.m_01 * temp[6] + a.m_02 * temp[7] - a.m_03 * temp[8]) * inv_det;
 m.m_02 = (a.m_31 * temp[5] - a.m_32 * temp[4] + a.m_33 * temp[3]) * inv_det;
 m.m_03 = (-a.m_21 * temp[5] + a.m_22 * temp[4] - a.m_23 * temp[3]) * inv_det;

 m.m_10 = (-a.m_10 * temp[6] + a.m_12 * temp[9] - a.m_13 * temp[10]) * inv_det;
 m.m_11 = (a.m_00 * temp[6] - a.m_02 * temp[9] + a.m_03 * temp[10]) * inv_det;
 m.m_12 = (-a.m_30 * temp[5] + a.m_32 * temp[2] - a.m_33 * temp[1]) * inv_det;
 m.m_13 = (a.m_20 * temp[5] - a.m_22 * temp[2] + a.m_23 * temp[1]) * inv_det;

 m.m_20 = (a.m_10 * temp[7] - a.m_11 * temp[9] + a.m_13 * temp[11]) * inv_det;
 m.m_21 = (-a.m_00 * temp[7] + a.m_01 * temp[9] - a.m_03 * temp[11]) * inv_det;
 m.m_22 = (a.m_30 * temp[4] - a.m_31 * temp[2] + a.m_33 * temp[0]) * inv_det;
 m.m_23 = (-a.m_20 * temp[4] + a.m_21 * temp[2] - a.m_23 * temp[0]) * inv_det;

 m.m_30 = (-a.m_10 * temp[8] + a.m_11 * temp[10] - a.m_12 * temp[11]) * inv_det;
 m.m_31 = (a.m_00 * temp[8] - a.m_01 * temp[10] + a.m_02 * temp[11]) * inv_det;
 m.m_32 = (-a.m_30 * temp[3] + a.m_31 * temp[1] - a.m_32 * temp[0]) * inv_det;
 m.m_33 = (a.m_20 * temp[3] - a.m_21 * temp[1] + a.m_22 * temp[0]) * inv_det;
#endif
 return m;
}


NP_HOT NPMat4x4 NPMat4x4_Transpose(NPMat4x4 a) {
 NPMat4x4 m;
	m.m_00 = a.m_00;
	m.m_10 = a.m_01;
	m.m_20 = a.m_02;
	m.m_30 = a.m_03;
	
	m.m_01 = a.m_10;
	m.m_11 = a.m_11;
	m.m_21 = a.m_12;
	m.m_31 = a.m_13;
	
	m.m_02 = a.m_20;
	m.m_12 = a.m_21;
	m.m_22 = a.m_22;
	m.m_32 = a.m_23;
	
	m.m_03 = a.m_30;
	m.m_13 = a.m_31;
	m.m_23 = a.m_32;
	m.m_33 = a.m_33;
	return m;
}


NP_HOT NPReal4 NPMat4x4_MulVec(NPMat4x4 a, NPReal4 b) {
 NP_ALIGNED NPReal4 v;
#ifdef NP_HAS_NEON

#ifdef NP_USE_F64

 NP_ALIGNED float64x2_t aa[2] = {{a.m_00, a.m_10}, {a.m_20, a.m_30}};
 NP_ALIGNED float64x2_t bb[2] = {{b.x, b.x}, {b.x, b.x}};
 NP_ALIGNED float64x2_t cc[2] = {{a.m_01, a.m_11}, {a.m_21, a.m_31}};
 NP_ALIGNED float64x2_t dd[2] = {{b.y, b.y}, {b.y, b.y}};
 NP_ALIGNED float64x2_t ee[2] = {{a.m_02, a.m_12}, {a.m_22, a.m_32}};
 NP_ALIGNED float64x2_t ff[2] = {{b.z, b.z}, {b.z, b.z}};
 NP_ALIGNED float64x2_t gg[2] = {a.m_02, a.m_12, a.m_22, a.m_33};
 NP_ALIGNED float64x2_t hh[2] = {{b.w, b.w}, {b.w, b.w}};

 vst1q_f64((np_real*)&v, vfmaq_f64(vfmaq_f64(vfmaq_f64(vmulq_f64(gg[0], hh[0]), ee[0], ff[0]), cc[0], dd[0]), aa[0], bb[0]));
 vst1q_f64((np_real*)&v.z, vfmaq_f64(vfmaq_f64(vfmaq_f64(vmulq_f64(gg[1], hh[1]), ee[1], ff[1]), cc[1], dd[1]), aa[1], bb[1]));

#else

 NP_ALIGNED float32x4_t aa = {a.m_00, a.m_10, a.m_20, a.m_30};
 NP_ALIGNED float32x4_t bb = vdupq_n_f32(b.x);
 NP_ALIGNED float32x4_t cc = {a.m_01, a.m_11, a.m_21, a.m_31};
 NP_ALIGNED float32x4_t dd = vdupq_n_f32(b.y);
 NP_ALIGNED float32x4_t ee = {a.m_02, a.m_12, a.m_22, a.m_32};
 NP_ALIGNED float32x4_t ff = vdupq_n_f32(b.z);
 NP_ALIGNED float32x4_t gg = {a.m_02, a.m_12, a.m_22, a.m_33};
 NP_ALIGNED float32x4_t hh = vdupq_n_f32(b.w);

 vst1q_f32((np_real*)&v, vfmaq_f32(vfmaq_f32(vfmaq_f32(vmulq_f32(gg, hh), ee, ff), cc, dd), aa, bb));
#endif

#else
	v.x = a.m_00 * b.x + a.m_01 * b.y + a.m_02 * b.z + a.m_03 * b.w;
 v.y = a.m_10 * b.x + a.m_11 * b.y + a.m_12 * b.z + a.m_13 * b.w;
	v.z = a.m_20 * b.x + a.m_21 * b.y + a.m_22 * b.z + a.m_23 * b.w;
	v.w = a.m_30 * b.x + a.m_31 * b.y + a.m_32 * b.z + a.m_33 * b.w;
#endif
 return v;
}



