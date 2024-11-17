/****************************************************************************
 *
 *   Copyright (c) 2021 PX4 Development Team. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name PX4 nor the names of its contributors may be
 *    used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 ****************************************************************************/

#include "ActuatorEffectivenessMorphingBird.hpp"
#include <ControlAllocation/ControlAllocation.hpp>

using namespace matrix;

ActuatorEffectivenessMorphingBird::ActuatorEffectivenessMorphingBird(ModuleParams *parent)
	: ModuleParams(parent), _rotors(this, ActuatorEffectivenessRotors::AxisConfiguration::FixedForward),
	  _control_surfaces(this)
{
}

bool
ActuatorEffectivenessMorphingBird::getEffectivenessMatrix(Configuration &configuration,
		EffectivenessUpdateReason external_update)
{
	// update morphing wing configuration
	bool is_new_config = updateMorphingWingExtent() ;

	if ( (is_new_config == false ) && (external_update == EffectivenessUpdateReason::NO_EXTERNAL_UPDATE) ) {
		return false;
	}
	else {
		// Motors
		_rotors.enablePropellerTorque(false);
		const bool rotors_added_successfully = _rotors.addActuators(configuration);
		_forwards_motors_mask = _rotors.getForwardsMotors();

		// Control Surfaces
		_first_control_surface_idx = configuration.num_actuators_matrix[0];
		const bool surfaces_added_successfully = _control_surfaces.addActuators(configuration);

		return (rotors_added_successfully && surfaces_added_successfully);
	}
}

bool
ActuatorEffectivenessMorphingBird::updateMorphingWingExtent()
{
	if ( !_morphing_wing_extent_sub.updated() )	// not new config
		return false ;

	morphing_wing_extent_s morphing_wing_extent_data ;
	_morphing_wing_extent_sub.copy(&morphing_wing_extent_data) ;

	float shoulder{-1}, wrist{-1} ;
	if ( morphing_wing_extent_data.main_wing_extent < 0 ) {
		shoulder = 2*morphing_wing_extent_data.main_wing_extent + 1 ;
		wrist = -1.0f ;
	}
	else {
		shoulder = 1.0f ;
		wrist = 2*morphing_wing_extent_data.main_wing_extent - 1 ;
	}

	ActuatorEffectivenessControlSurfaces::Params* params ;
	int icount = _control_surfaces.count() ;

	for (int i = 0; i < icount; i++) {
		params = _control_surfaces.getConfig(i) ;

		switch ( params->type ) {
			case ActuatorEffectivenessControlSurfaces::Type::LeftShoulder:
			case ActuatorEffectivenessControlSurfaces::Type::RightShoulder:
				params->trim = shoulder ;
				break ;
			case ActuatorEffectivenessControlSurfaces::Type::LeftWrist:
			case ActuatorEffectivenessControlSurfaces::Type::RightWrist:
				params->trim = wrist ;
				break ;
			case ActuatorEffectivenessControlSurfaces::Type::LeftTailExtend:
			case ActuatorEffectivenessControlSurfaces::Type::RightTailExtend:
				params->trim = morphing_wing_extent_data.tail_extent ;
				break ;
			default:
				break ;

		}
	}

	return true ;
}

void ActuatorEffectivenessMorphingBird::updateSetpoint(const matrix::Vector<float, NUM_AXES> &control_sp,
		int matrix_index, ActuatorVector &actuator_sp, const matrix::Vector<float, NUM_ACTUATORS> &actuator_min,
		const matrix::Vector<float, NUM_ACTUATORS> &actuator_max)
{
	stopMaskedMotorsWithZeroThrust(_forwards_motors_mask, actuator_sp);
}

void ActuatorEffectivenessMorphingBird::allocateAuxilaryControls(const float dt, int matrix_index,
		ActuatorVector &actuator_sp)
{
	// apply flaps
	normalized_unsigned_setpoint_s flaps_setpoint;

	if (_flaps_setpoint_sub.copy(&flaps_setpoint)) {
		_control_surfaces.applyFlaps(flaps_setpoint.normalized_setpoint, _first_control_surface_idx, dt, actuator_sp);
	}

	// apply spoilers
	normalized_unsigned_setpoint_s spoilers_setpoint;

	if (_spoilers_setpoint_sub.copy(&spoilers_setpoint)) {
		_control_surfaces.applySpoilers(spoilers_setpoint.normalized_setpoint, _first_control_surface_idx, dt, actuator_sp);
	}
}
