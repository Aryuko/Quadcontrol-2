/*
 * A PID controller that aims for stabilization by taking values from the
 * sensors and returns a suitable PWM value.
 *
 * For http://github.com/Zalodu/Quadcontrol-2
 * Author: Peter Kjellén (Zalodu)
 * Date: 29/11/16
 */

/* PID constants */
#define proportionalGain 1.0
#define integralGain 1.0
#define derivativeGain 1.0
#define derivativeDerivativeGain 1.0

/* Integrator anti-windup */
const double integratorMax = (1)/integralGain;
const double integratorMin = (-1)/integralGain;

/* Current integral value */
double integratorState = 0.0;

/* Last error */
double derivatorState = 0.0;

/*
 * Returns a double calculated using the proportional constant.
 */
double calculateP (double error) {
	return error * proportionalGain;
}

/*
 * Returns a double calculated using the integral constant and the sum of past
 * errors.
 */
double calculateI (double error) {
	integratorState += error;

	/* Apply anti-windup */
	if(integratorState > integratorMax) { integratorState = integratorMax; }
	else if(integratorState > integratorMin) { integratorState = integratorMin; }

	return integratorState * integralGain;
}

/*
 * Returns a double calculated using the derivate constant and the last error.
 */
double calculateD (double error, double position) {
	double dTerm = derivativeDerivativeGain * (derivatorState - position);
	derivatorState += dTerm;

	return dTerm * derivativeGain;
}

/*
 * Calculates a vector of doubles between -1 and 1 based on the
 * vector of errors given as well as the vectors of positions.
 *
 * Outputs the results to the given "result" array in the order of:
 * FRONT, BACK, LEFT, RIGHT
 */
void calculateStrategy (double* error, double* position, double* result) {
	int i;
	for (i = 0; i < 3; i++) {
		result[i] = calculateP(error[i]) + calculateI(error[i]) + calculateD(error[i], position[i]);
	}
}
