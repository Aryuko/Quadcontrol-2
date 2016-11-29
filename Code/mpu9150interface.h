/*
 * A simple interface containing functions for the MPU9150.
 *
 * For http://github.com/Zalodu/Quadcontrol-2
 * Author: Peter Kjellén (Zalodu)
 * Date: 16/11/16
 */

#pragma once

int mpu9150interface_notConnected (void);

int mpu9150interface_setup(void);

int mpu9150interface_awaken(void);

int mpu9150interface_sleep(void);

int mpu9150interface_getAccelValues(double* values);

int mpu9150interface_getGyroValues(double* values);
