/*
 * A PID controller that aims for stabilization by taking values from the
 * sensors and returns a suitable PWM value.
 *
 * For http://github.com/Zalodu/Quadcontrol-2
 * Author: Peter Kjellén (Zalodu)
 * Date: 27/11/16
 */

#pragma once

typedef struct {
/* PID constants */
	const double proportionalGain;
  const double integralGain;
	const double derivativeGain;
	const double derivativeDerivativeGain;

	/* Integrator anti-windup */
	double integratorMax;
	double integratorMin;

	/* Current integral value */
	double integratorState;
	/* Last error */
	double derivatorState;
} ControllerState;

Vector3 calculateStrategy (ControllerState state, Vector3 error, Vector3 position);
