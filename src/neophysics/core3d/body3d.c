#include "neophysics/core3d/body3d.h"
#include "neophysics/core3d/world3d.h"
#include "neophysics/math.h"

#include <string.h>

static void init_box3d(NPBoxBody3D *body_ptr, NPWorld3D *ctx, NPCreateBox3D *info);
static void step_transform_box3d(NPBoxBody3D *body_ptr, NPWorld3D *ctx, const np_real dt);

static void step_velocity(NPBody3D *body_ptr, NPWorld3D *ctx, const np_real dt);
static void step_position(NPBody3D *body_ptr, NPWorld3D *ctx, const np_real dt);
static void step_velocity_empty(NPBody3D *body_ptr, NPWorld3D *ctx, const np_real dt);
static void step_position_empty(NPBody3D *body_ptr, NPWorld3D *ctx, const np_real dt);

NP_Init_Prototype __np_body_init[NPBODY_TYPE_MAX_VALUE] = {
	NULL, 
	(NP_Init_Prototype)init_box3d
};


NP_Step_Prototype __np_body_step_transform[NPBODY_TYPE_MAX_VALUE] = {
	NULL,
	(NP_Step_Prototype)step_transform_box3d
};


NP_Step_Prototype __np_body_step_velocity[2] = {
	(NP_Step_Prototype)step_velocity,
	(NP_Step_Prototype)step_velocity_empty
};

NP_Step_Prototype __np_body_step_position[2] = {
	(NP_Step_Prototype)step_position,
	(NP_Step_Prototype)step_position_empty
};


static void init_box3d(NPBoxBody3D *body_ptr, NPWorld3D *ctx, NPCreateBox3D *info) {
 (void)ctx;
 memset(body_ptr, 0, sizeof(NPBoxBody3D));
 body_ptr->_components._logarithmic_linear_damping.x = NP_Log2(info->linear_damping.x);
 body_ptr->_components._logarithmic_linear_damping.y = NP_Log2(info->linear_damping.y);
 body_ptr->_components._logarithmic_linear_damping.z = NP_Log2(info->linear_damping.z);

 body_ptr->_components._logarithmic_angular_damping.x = NP_Log2(info->angular_damping.x);
 body_ptr->_components._logarithmic_angular_damping.y = NP_Log2(info->angular_damping.y);
 body_ptr->_components._logarithmic_angular_damping.z = NP_Log2(info->angular_damping.z);
}


static void step_transform_box3d(NPBoxBody3D *body_ptr, NPWorld3D *ctx, const np_real dt) {
 (void)body_ptr;
 (void)ctx;
 (void)dt;
}




static void step_velocity(NPBody3D *body_ptr, NPWorld3D *ctx, const np_real dt) {
/*
 Euler Integration of velocity step
 
  acceleration = force / mass
  velocity = (velocity + acceleration) * dt
*/
 NPReal3 dt_vec = NP_Stv3(dt);
 NPReal3 inv_mass = NP_Stv3(body_ptr->_components._inv_mass);
 NPReal3 linear_acceleration = NP_Mul3(NP_Add3(body_ptr->_components._force, ctx->_config.gravity), inv_mass);

 body_ptr->_components._velocity = NP_MulAc3(linear_acceleration, dt_vec, body_ptr->_components._velocity);
 body_ptr->_components._force = NP_Stv3(0.0f);

 NPReal3 angular_acceleration = NP_Mul3(body_ptr->_components._torque, body_ptr->_components._inv_rotational_inertia);
 body_ptr->_components._omega = NP_MulAc3(angular_acceleration, dt_vec, body_ptr->_components._omega);
 body_ptr->_components._torque = NP_Stv3(0.0f);

/*
 Damping
 Exponential Decay
 
 Genera Formula :
 
   velocity = velocity * e ^ (-damping_factor * dt)
 
 Optimized Solution:
 
   velocity = velocity *  2 ^ (dt * -log2(damping_factor))
 
 where : log2(damping_factor) can be pre calculated
 and exp2 is easy to approximate.
 
 this is to make sure that damping is consistent accross frames.
*/

/*
 TODO : use simd for exp2
*/
 body_ptr->_components._velocity.x *= NP_Exp2(dt * -body_ptr->_components._logarithmic_linear_damping.x);
 body_ptr->_components._velocity.y *= NP_Exp2(dt * -body_ptr->_components._logarithmic_linear_damping.y);
 body_ptr->_components._velocity.z *= NP_Exp2(dt * -body_ptr->_components._logarithmic_linear_damping.z);

 body_ptr->_components._omega.x *= NP_Exp2(dt * -body_ptr->_components._logarithmic_angular_damping.x);
 body_ptr->_components._omega.y *= NP_Exp2(dt * -body_ptr->_components._logarithmic_angular_damping.y);
 body_ptr->_components._omega.z *= NP_Exp2(dt * -body_ptr->_components._logarithmic_angular_damping.z);
}


static void step_position(NPBody3D *body_ptr, NPWorld3D *ctx, const np_real dt) {
	(void)ctx;
/*
 Euler Integration of position step
 
  position = (position + velocity) * dt
*/
 NPReal3 dt_vec = NP_Stv3(dt);
 body_ptr->_components._position = NP_Mul3(NP_Add3(body_ptr->_components._position, body_ptr->_components._velocity), dt_vec);
 body_ptr->_components._rotation = NP_Mul3(NP_Add3(body_ptr->_components._rotation, body_ptr->_components._omega), dt_vec);
 /* transform the body 
 __np_body_step_transform[body_ptr->_components._info & NPBODY_TYPE_MASK](body_ptr, ctx, dt);
 */
}

static void step_velocity_empty(NPBody3D *body_ptr, NPWorld3D *ctx, const np_real dt) {
 (void)body_ptr;
 (void)ctx;
 (void)dt;
}
static void step_position_empty(NPBody3D *body_ptr, NPWorld3D *ctx, const np_real dt) {
 (void)body_ptr;
 (void)ctx;
 (void)dt;
}
