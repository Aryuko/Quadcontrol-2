/*
 * An interface for the PID controller controller.c
 *
 * For http://github.com/Zalodu/Quadcontrol-2
 * Author: Peter Kjellén (Zalodu)
 * Date: 29/11/16
 */

 #include "controller.h"

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
 * Fills result with the throttle proportions calculated.
 */
void controllerinterface_step (double* targetGyroValues,
		                       double* targetAccelValues,
		                       double* actualGyroValues,
		                       double* actualAccelValues,
		                       double* result) {
	double gyroError[3];
	vectorSubtract(targetGyroValues, actualGyroValues, gyroError);
	double gyroResult[3];
	calculateStrategy(gyroError, actualGyroValues, gyroResult);

	double accelError[3];
	vectorSubtract(targetAccelValues, actualAccelValues, accelError);
	double accelResult[3];
	calculateStrategy(accelError, actualAccelValues, accelResult);

	result[0] = gyroResult[0];
	result[1] = gyroResult[1];
	result[2] = gyroResult[2];
	result[3] = accelResult[0];
	result[4] = accelResult[1];
	result[5] = accelResult[2];
}
