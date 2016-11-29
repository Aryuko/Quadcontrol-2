/*
 * An interface for the PID controller controller.c
 *
 * For http://github.com/Zalodu/Quadcontrol-2
 * Author: Peter Kjellén (Zalodu)
 * Date: 29/11/16
 */

#pragma once

void controller_step (double* targetGyroValues,
                      double* targetAccelValues,
                      double* actualGyroValues,
                      double* actualAccelValues,
                      double* resultingThrottle);
