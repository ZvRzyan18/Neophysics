#ifndef NP_TYPES_H
#define NP_TYPES_H

#include <stdint.h>

#define NP_ENABLE_EXPERIMENTAL_ARM_NEON 
#define NP_USE_F64

#define NP_ALIGNED alignas(16)

#if defined(NP_ENABLE_EXPERIMENTAL_ARM_NEON) && (defined(__ARM_NEON) || defined(__ARM_NEON__))
#define NP_HAS_NEON
#include <arm_neon.h> 
#endif

#ifdef NP_USE_F64
typedef double np_real;
typedef uint64_t np_urb;
typedef int64_t np_srb;
#else
typedef float np_real;
typedef uint32_t np_urb;
typedef int32_t np_srb;
#endif

#define NP_TRUE 1
#define NP_FALSE 0

typedef uint64_t np_u64;
typedef int64_t  np_s64;
typedef uint32_t np_u32;
typedef int32_t  np_s32;
typedef uint16_t np_u16;
typedef int16_t  np_s16;
typedef uint8_t np_u8;
typedef int8_t  np_s8;

typedef size_t np_size;

#if defined(__GNUC__) || defined(__clang__)
#define NP_LIKELY(x)   __builtin_expect(!!(x), 1)
#define NP_UNLIKELY(x) __builtin_expect(!!(x), 0)
#define NP_HOT __attribute__((hot, unused))
#define NP_COLD __attribute__((cold, unused))
#else
#define NP_LIKELY(x)  (x)
#define NP_UNLIKELY(x) (x)
#define NP_HOT 
#define MP_COLD 
#endif

#endif

