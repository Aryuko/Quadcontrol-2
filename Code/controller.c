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
 * Calculates a vector of doubles between 0 and 1 based on the
 * vector of errors given as well as the vectors of positions.
 *
 * CURRENTLY LIMITED TO ONE DIMENSION
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


 /*
  * Subtracts one "vector" b (3-element array) from another "vector" a and
  * outputs the result to the given "result" array.
  */
 void vectorSubtract (double* a, double* b, double* result) {
   result[0] = a[0] - b[0];
   result[1] = a[1] - b[1];
   result[2] = a[2] - b[2];
 }

 /*
  * Calculates the error between the the actual sensor values and the
  * desired sensor values.
  *
  * Fills calculatedError with the calculated errors.
  */
 void calculateError (double* targetGyroValues,
                     double* targetAccelValues,
                     double* actualGyroValues,
                     double* actualAccelValues,
                     double* calculatedError) {
   double gyroError[3];
   vectorSubtract(targetGyroValues, actualGyroValues, gyroError);

   double accelError[3];
   vectorSubtract(targetAccelValues, actualAccelValues, accelError);

   calculatedError[0] = gyroError[0];
   calculatedError[1] = gyroError[1];
   calculatedError[2] = gyroError[2];
   calculatedError[3] = accelError[0];
   calculatedError[4] = accelError[1];
   calculatedError[5] = accelError[2];
 }

/*
 * Calculates the throttle proportions required to reach the target.
 *
 * Fills resultingThrottle with the throttle proportions calculated.
 */
void controller_step (double* targetGyroValues,
                      double* targetAccelValues,
                      double* actualGyroValues,
                      double* actualAccelValues,
                      double* resultingThrottle) {
      double error[6];
      calculateError(targetGyroValues, targetAccelValues, actualGyroValues, actualAccelValues, error);
}
