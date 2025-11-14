#ifndef NP_TYPES_H
#define NP_TYPES_H

#include <stdint.h>
#include <stddef.h>

typedef uint8_t NPUint8;
typedef uint16_t NPUint16;
typedef uint32_t NPUint32;
typedef int16_t NPInt16;
typedef int32_t NPInt32;
typedef void*    NPPointer;

#ifdef NP_USE_FLOAT64
typedef double NPReal;
typedef union {
 double f;
 NPUint64 i;
} NPRealBit;
#else 
typedef float NPReal;
typedef union {
 float f;
 NPUint32 i;
} NPRealBit;
#endif

#define NPNULL NULL
#define NP_API

enum NPErrorCode {
	NP_ALLOCATION_FAILED,
	NP_DEALLOCATION_FAILED,
	NP_INPUT_FAILED,
	NP_OBJECT_NOT_FOUND,
	NP_INVALID_STALE_POINTER,
	NP_ZERO_SCALE, 
};


#endif


