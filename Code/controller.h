/*
 * A PID controller that aims for stabilization by taking values from the
 * sensors and returns a suitable PWM value.
 *
 * For http://github.com/Zalodu/Quadcontrol-2
 * Author: Peter Kjellén (Zalodu)
 * Date: 27/11/16
 */

#pragma once

void controller_step (double* targetGyroValues,
                      double* targetAccelValues,
                      double* actualGyroValues,
                      double* actualAccelValues,
                      double* resultingThrottle);
