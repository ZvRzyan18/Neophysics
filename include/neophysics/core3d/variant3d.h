#ifndef NP_VARIANT3D_H
#define NP_VARIANT3D_H

#include "neophysics/real2.h"
#include "neophysics/real3.h"
#include "neophysics/real4.h"
#include "neophysics/quat.h"
#include "neophysics/mat3x3.h"
#include "neophysics/mat4x4.h"

/* world3d */
typedef struct NPWorldConfig3D NPWorldConfig3D;
typedef struct NPWorldLimits3D NPWorldLimits3D;
typedef struct NPWorld3D NPWorld3D;

/* body3d */
typedef struct NPBodyComponents3D NPBodyComponents3D;
typedef struct NPBoxBody3D NPBoxBody3D;
typedef union NPBody3D NPBody3D;

/* island3d */
typedef struct NPIsland3D NPIsland3D;

/* object functions */
typedef void (*NP_Init_Prototype)(void *body_ptr, NPWorld3D *ctx, void *info);
typedef void (*NP_Step_Prototype)(void *body_ptr, NPWorld3D *ctx, const np_real dt);


/* object create */
typedef struct {
 NPReal3 min_value, max_value;
 NPReal3 linear_damping;
 NPReal3 angular_damping;
 np_real density;
 np_real friction;
} NPCreateBox3D;

#endif

