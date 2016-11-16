/*
 * A simple interface containing functions for the MPU9150.
 *
 * For http://github.com/Zalodu/Quadcontrol-2
 * Author: Peter Kjellén (Zalodu)
 * Date: 16/11/16
 */

#pragma once

int setupMPU9150(void);

int awakenMPU9150(void);

int sleepMPU9150(void);

int getAccelValues(int* values);

int getGyroValues(int* values);
