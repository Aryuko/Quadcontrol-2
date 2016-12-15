/*
 * An extended interface for more specific use of the MPU9150.
 *
 * For http://github.com/Zalodu/Quadcontrol-2
 * Author: Jesper Larsson (mrlarssonjr)
 * Date: 15/12/16
 */

#pragma once

int mpu9150ExtendedInterface_notConnected (void);

void mpu9150ExtendedInterface_init(void);

void mpu9150ExtendedInterface_tick(void);

Vector3 mpu9150ExtendedInterface_getInclinationDerivative(void);

Vector3 mpu9150ExtendedInterface_getSpeedDerivative(void);


Vector3 mpu9150ExtendedInterface_getInclination(void);

Vector3 mpu9150ExtendedInterface_getSpeed(void);
