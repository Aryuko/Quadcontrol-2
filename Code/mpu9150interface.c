/*
 * A simple interface containing functions for the MPU9150.
 *
 * For http://github.com/Zalodu/Quadcontrol-2
 * Author: Peter Kjellén (Zalodu)
 * Date: 15/11/16
 */

#include <stdint.h>   /* Declarations of uint_32 and the like */
#include <pic32mx.h>  /* Declarations of system-specific addresses etc */
#include "mpu9150msg.h"
#include "mpu9150registers.h"
#include "i2caddresses.h"

//Digital Low Pass Filter, 0-7
char DLPF = 0;

//Gyroscope range scale, 0-3
char gyroScale = 0;

//Accelerometer range scale, 0-3
char accelScale = 0;


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

	//Setup gyroscope config
	int gyroConfig = gyroScale << 3;
	if(sendMessage(MPU6150, GYRO_CONFIG, gyroConfig)){
		return -1;
	}

	//Setup accellerometer config
	int accelConfig = accelScale << 3;
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
