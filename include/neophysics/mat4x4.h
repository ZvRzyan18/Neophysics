#ifndef NP_MAT4x4_H
#define NP_MAT4x4_H

#include "neophysics/types.h"
#include "neophysics/real4.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
	np_real m_00, m_10, m_20, m_30;
	np_real m_01, m_11, m_21, m_31;
	np_real m_02, m_12, m_22, m_32;
	np_real m_03, m_13, m_23, m_33;
} NPMat4x4;

extern const NPMat4x4 NPMat4x4_Identity;

NP_HOT NPMat4x4 NPMat4x4_FromQuaternion(NPReal4 q);
NP_HOT NPMat4x4 NPMat4x4_Mul(NPMat4x4 a, NPMat4x4 b);
NP_HOT NPMat4x4 NPMat4x4_Invert(NPMat4x4 a);
NP_HOT NPMat4x4 NPMat4x4_Transpose(NPMat4x4 a);
NP_HOT NPReal4 NPMat4x4_MulVec(NPMat4x4 a, NPReal4 b);


#ifdef __cplusplus
}
#endif

#endif

