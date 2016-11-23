/*
 * A simple interface containing functions for the MPU9150.
 *
 * For http://github.com/Zalodu/Quadcontrol-2
 * Author: Peter Kjellén (Zalodu)
 * Date: 16/11/16
 */

#pragma once

int MPU9150_setup(void);

int MPU9150_awaken(void);

int MPU9150_sleep(void);

int MPU9150_getAccelValues(double* values);

int MPU9150_getGyroValues(double* values);
