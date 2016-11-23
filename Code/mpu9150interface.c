/*
 * A simple interface containing functions for the MPU9150.
 *
 * For http://github.com/Zalodu/Quadcontrol-2
 * Author: Peter Kjellén (Zalodu)
 * Date: 16/11/16
 */

#include <stdint.h>   /* Declarations of uint_32 and the like */
#include <pic32mx.h>  /* Declarations of system-specific addresses etc */
#include "mpu9150msg.h"
#include "mpu9150registers.h"
#include "i2caddresses.h"

//Digital Low Pass Filter, 0-7
char DLPF = 6;

//Clock source, 0-7
char clockSource = 1;

//Gyroscope range scale, 0-3
char gyroScale = 3;

//Accelerometer range scale, 0-3
char accelScale = 3;


/*
 * Applies the desired settings by writing to config registers on the MPU9150
 *
 * Returns 0 if successfull, -1 otherwise
 */
int setupMPU9150 (void) {
	//Setup Digital Low Pass Filter, EXT_SYNC_SET is ignored
	if(sendMessage(MPU6150, CONFIG, DLPF)){
		return -1;
	}

	//Setup clock source
	int data;
	if (receiveMessage(MPU6150, POWER_MGMT_1, &data)){
		return -1;
	}

	char clockConfig = (data & 0xF8) | clockSource;
	if(sendMessage(MPU6150, POWER_MGMT_1, clockConfig)){
		return -1;
	}


	//Setup gyroscope config
	char gyroConfig = gyroScale << 3;
	if(sendMessage(MPU6150, GYRO_CONFIG, gyroConfig)){
		return -1;
	}

	//Setup accellerometer config
	char accelConfig = accelScale << 3;
	if(sendMessage(MPU6150, ACCEL_CONFIG, accelConfig)){
		return -1;
	}
}

/*
 * Wakes the MPU9150 sensor
 *
 * Returns 0 if successfull, -1 otherwise
 */
int awakenMPU9150 (void) {
	int data;
	if (receiveMessage(MPU6150, POWER_MGMT_1, &data)){
		return -1;
	}

	data = data & ~(1 << 6);
	if (sendMessage(MPU6150, POWER_MGMT_1, data)){
		return -1;
	}
	return 0;
}

/*
 * Sleeps the MPU9150 sensor
 *
 * Returns 0 if successfull, -1 otherwise
 */
int sleepMPU9150 (void) {
	int data;
	if (receiveMessage(MPU6150, POWER_MGMT_1, &data)){
		return -1;
	}

	data = data | (1 << 6);
	if (sendMessage(MPU6150, POWER_MGMT_1, data)){
		return -1;
	}
	return 0;
}

/*
 * Converts the given relative value to degrees.
 *
 * Returns the given value converted to degrees.
 */
double convertToDegrees (int a) {
	//32767(0x7FFF) => "fullScale" degrees
 	double fullScale = 250;

	//fullScale = 250*2^gyroScale
	//since fullScale is 250/500/1000/2000
	char i;
	for (i = 0; i < gyroScale; i++) {
		fullScale *= 2;
	}

	return a * (fullScale/(double)0x7FFF);
 }

 /*
  * Converts the given relative value to newtons.
  *
  * Returns the given value converted to newtons.
  */
 double convertToNewtons (int a) {
 	//32767(0x7FFF) => "fullScale" g forces
  	double fullScale = 2;

 	//fullScale = 2*2^accelScale
 	//Since fullScale is 2/4/8/16
 	char i;
 	for (i = 0; i < accelScale; i++) {
 		fullScale *= 2;
 	}

 	return a * (fullScale/(double)0x7FFF) * 9.80665;
  }

/*
 * Fetches and returns the accelerometer values from the MPU9150.
 * The x, y, and z values of the accelerometer are added in order to the
 * specified pointer/array.
 *
 * Returns 0 if successfull, -1 otherwise
 */
int getAccelValues (double* values) {
	int valueL;
	int valueH;

	//Accelerometer x
	if (receiveMessage(MPU6150, ACCEL_XOUT_L, &valueL)) { return -1; }
	if (receiveMessage(MPU6150, ACCEL_XOUT_H, &valueH)) { return -1; }
	values[0] = convertToNewtons(signExtend16To32((valueH << 8) | valueL));

	//Accelerometer y
	if (receiveMessage(MPU6150, ACCEL_YOUT_L, &valueL)) { return -1; }
	if (receiveMessage(MPU6150, ACCEL_YOUT_H, &valueH)) { return -1; }
	values[1] = convertToNewtons(signExtend16To32((valueH << 8) | valueL));

	//Accelerometer z
	if (receiveMessage(MPU6150, ACCEL_ZOUT_L, &valueL)) { return -1; }
	if (receiveMessage(MPU6150, ACCEL_ZOUT_H, &valueH)) { return -1; }
	values[2] = convertToNewtons(signExtend16To32((valueH << 8) | valueL));

	return 0;
}

/*
 * Fetches and returns the gyroscope values from the MPU9150.
 * The x, y, and z values of the gyroscope are added in order to the
 * specified pointer/array.
 *
 * Returns 0 if successfull, -1 otherwise
 */
int getGyroValues (double* values) {
	int valueL;
	int valueH;

	//Gyroscope x
	if (receiveMessage(MPU6150, GYRO_XOUT_L, &valueL)) { return -1; }
	if (receiveMessage(MPU6150, GYRO_XOUT_H, &valueH)) { return -1; }
	values[0] = convertToDegrees(signExtend16To32((valueH << 8) | valueL));

	//Gyroscope y
	if (receiveMessage(MPU6150, GYRO_YOUT_L, &valueL)) { return -1; }
	if (receiveMessage(MPU6150, GYRO_YOUT_H, &valueH)) { return -1; }
	values[1] = convertToDegrees(signExtend16To32((valueH << 8) | valueL));

	//Gyroscope z
	if (receiveMessage(MPU6150, GYRO_ZOUT_L, &valueL)) { return -1; }
	if (receiveMessage(MPU6150, GYRO_ZOUT_H, &valueH)) { return -1; }
	values[2] = convertToDegrees(signExtend16To32((valueH << 8) | valueL));

	return 0;
}

/*
 * Sign extends the given 16-bit integer into a 32-bit integer.
 *
 * Returns the given 16-bit value sign extended to 32-bits.
 */
int signExtend16To32 (int a) {
	return (a & 0x8000 ? a | 0xFFFF0000 : a & 0xFFFF);
}
