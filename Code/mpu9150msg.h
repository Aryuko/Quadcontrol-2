#pragma once

/*
 * Performs an Burst Write Sequence to a MPU9150 device.
 *
 * Returns 0 if successfull, -1 if not.
 */
int mpu9150msg_repeatedSendMessage(char slaveAddress, char firstSlaveRegister, char dataBytes[], int length);

/*
 * Send a message byte to a MPU9150 device.
 *
 * Returns 0 if successfull, -1 if not.
 */
int mpu9150msg_sendMessage(char slaveAddress, char slaveRegister, char dataByte);

/*
 * Performs a Burst Read Sequence to a MPU9150 device.
 *
 * Returns 1 if successfull, -1 otherwise.
 */
int mpu9150msg_repeatedReceiveMessage(char slaveAddress, char slaveRegister, int* receivedBytes, int times);

/*
 * Receive a message byte to a MPU9150 device.
 *
 * Returns 1 if successfull, -1 otherwise.
 */
int mpu9150msg_receiveMessage(char slaveAddress, char slaveRegister, int* receivedByte);
