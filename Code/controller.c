/*
 * A PID controller that aims for stabilization by taking values from the
 * sensors and returns a suitable PWM value.
 *
 * For http://github.com/Zalodu/Quadcontrol-2
 * Author: Peter Kjellén (Zalodu)
 * Date: 29/11/16
 */
#include "vector.h"
#include "controller.h"
/*
 * Returns a double calculated using the proportional constant.
 */
double calculateP (double proportionalGain, double error) {
	return error * proportionalGain;
}

/*
 * Returns a double calculated using the integral constant and the sum of past
 * errors.
 */
double calculateI (double integralGain, double integratorMax, double integratorMin, double* integratorState, double error) {
	*integratorState += error;

	/* Apply anti-windup */
	if(*integratorState > integratorMax) { *integratorState = integratorMax; }
	else if(*integratorState < integratorMin) { *integratorState = integratorMin; }

	return *integratorState * integralGain;
}

/*
 * Returns a double calculated using the derivate constant and the last error.
 */
double calculateD (double derivativeGain, double derivativeDerivativeGain, double* derivatorState, double error, double position) {
	double dTerm = derivativeDerivativeGain * (*derivatorState - position);
	*derivatorState += dTerm;

	return dTerm * derivativeGain;
}

/*
 * Calculates a vector of doubles between -1 and 1 based on the
 * vector of errors given as well as the vectors of positions.
 *
 * Returns the results as a Vector3.
 */
Vector3 calculateStrategy (ControllerState state, Vector3 error, Vector3 position) {
	Vector3 result;

	double p, i, d;
	p = calculateP(state.proportionalGain, error.x);
	i = calculateI(state.integralGain, state.integratorMax, state.integratorMin, &state.integratorState, error.x);
	d = calculateD(state.derivativeGain, state.derivativeDerivativeGain, &state.derivatorState, error.x, position.x);
	result.x = p+i+d;

	p = calculateP(state.proportionalGain, error.y);
	i = calculateI(state.integralGain, state.integratorMax, state.integratorMin, &state.integratorState, error.y);
	d = calculateD(state.derivativeGain, state.derivativeDerivativeGain, &state.derivatorState, error.y, position.y);
	result.y = p+i+d;

	p = calculateP(state.proportionalGain, error.z);
	i = calculateI(state.integralGain, state.integratorMax, state.integratorMin, &state.integratorState, error.z);
	d = calculateD(state.derivativeGain, state.derivativeDerivativeGain, &state.derivatorState, error.z, position.z);
	result.z = p+i+d;

	return result;
}
