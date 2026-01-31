#ifndef NP_BODY3D_H
#define NP_BODY3D_H

#include "neophysics/real3.h"
#include "neophysics/real4.h"
#include "neophysics/core3d/variant3d.h"

#ifdef __cplusplus
extern "C" {
#endif

#define NPBODY_DYNAMIC_BIT_3D     0b1000000000000000
#define NPBODY_KINEMATIC_BIT_3D   0b0100000000000000
#define NPBODY_STATIC_BIT_3D      0b0010000000000000
#define NPBODY_AWAKE_BIT_3D       0b0001000000000000
#define NPBODY_TRANSFORMED_BIT_3D 0b0000100000000000

#define NPBODY_SET_LOCK_PX_BIT_3D 0x0000010000000000
#define NPBODY_SET_LOCK_PY_BIT_3D 0x0000001000000000
#define NPBODY_SET_LOCK_PZ_BIT_3D 0x0000000100000000

#define NPBODY_SET_LOCK_RX_BIT_3D 0x0000000010000000
#define NPBODY_SET_LOCK_RY_BIT_3D 0x0000000001000000
#define NPBODY_SET_LOCK_RZ_BIT_3D 0x0000000000100000

#define NPBODY_TYPE_MASK          0x0000000000011111
/* 
 remaining 5 bits is for body type 
 2 ^ 5 = 0-32 extra digits.
*/
#define NPBODY_TYPE_BOX_3D        1
#define NPBODY_TYPE_SPHERE_3D     2
#define NPBODY_TYPE_MAX_VALUE     3


struct NPBodyComponents3D {
 np_u16  _info;
 NPIsland3D *_island;
 NPReal3 _position;
 NPReal3 _velocity;
 NPReal3 _rotation;
 NPReal3 _omega;
 NPReal3 _logarithmic_linear_damping;
 NPReal3 _logarithmic_angular_damping;
 NPReal3 _force;
 NPReal3 _torque;
 NPReal3 _aabb_min, _aabb_max;
 NPReal3 _inv_rotational_inertia;
 np_real _inv_mass;
};



struct NPBoxBody3D {
	NPBodyComponents3D _components;
 NPReal3 *_vertices; /* 8 size*/
 NPReal3 *_normals; /* 6 size */
};



union NPBody3D {
 NPBodyComponents3D _components;
 NPBoxBody3D        _box_body;
};


extern NP_Init_Prototype __np_body_init[NPBODY_TYPE_MAX_VALUE];
extern NP_Step_Prototype __np_body_step_transform[NPBODY_TYPE_MAX_VALUE];

extern NP_Step_Prototype __np_body_step_velocity[2];
extern NP_Step_Prototype __np_body_step_position[2];



#ifdef __cplusplus
}
#endif


#endif

