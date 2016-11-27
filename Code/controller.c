/*
 * A PID controller that aims for stabilization by taking values from the
 * sensors and returns a suitable PWM value.
 *
 * For http://github.com/Zalodu/Quadcontrol-2
 * Author: Peter Kjellén (Zalodu)
 * Date: 27/11/16
 */

 /*
  * Subtracts one "vector" (3-element array) from another and outputs the result
  * to the given "result" array.
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
   int gyroError[3];
   vectorSubtract(targetGyroValues, actualGyroValues, gyroError);

   int accelError[3];
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
      int error[6];
      calculateError(targetGyroValues, targetAccelValues, actualGyroValues, actualAccelValues, error);
}
