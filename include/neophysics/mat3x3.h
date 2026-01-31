#ifndef NP_MAT3X3_H
#define NP_MAT3X3_H

#include "neophysics/types.h"
#include "neophysics/real4.h"
#include "neophysics/real3.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
	np_real m_00, m_10, m_20, __pad1;
	np_real m_01, m_11, m_21, __pad2;
	np_real m_02, m_12, m_22, __pad3;
} NPMat3x3;

extern const NPMat3x3 NPMat3x3_Identity;

NP_HOT NPMat3x3 NPMat3x3_FromQuaternion(NPReal4 q);
NP_HOT NPMat3x3 NPMat3x3_Mul(NPMat3x3 a, NPMat3x3 b);
NP_HOT NPMat3x3 NPMat3x3_Invert(NPMat3x3 a);
NP_HOT NPMat3x3 NPMat3x3_Transpose(NPMat3x3 a);
NP_HOT NPReal3 NPMat3x3_MulVec(NPMat3x3 a, NPReal3 b);

#ifdef __cplusplus
}
#endif

#endif


