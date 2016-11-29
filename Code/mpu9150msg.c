/*
 * A interface for sending messages to a MPU9150.
 *
 * For http://github.com/Zalodu/Quadcontrol-2
 * Authors: Jesper Larsson (MrLarssonJr), Peter Kjellén (Zalodu)
 * Date: 23/11/16
 */

#include <pic32mx.h>
#include <stdlib.h>
#include <stdint.h>
#include "i2cbus.h"
#include "i2cmap.h"
#include "mipslab.h"

#define TRUE 1
#define FALSE 0

#define ACK 0
#define NACK 1

char initialised = FALSE;

/*
 * Initialise the I2C bus, if it isn't already.
 */
void mpu9150msg_initIfNotAllready(void) {
  if(!initialised) {
    i2cbus_init();
    initialised = TRUE;
  }
}

/*
 * Performs an Burst Write Sequence to a MPU9150 device.
 *
 * Returns 0 if successfull, -1 if not.
 */
int mpu9150msg_repeatedSendMessage(char slaveAddress, char firstSlaveRegister, char dataBytes[], int length) {
  mpu9150msg_initIfNotAllready();

  i2cbus_start();

  //Configure address byte for send mode
  slaveAddress = slaveAddress << 1;
  i2cbus_send(slaveAddress);

  if(ACKSTAT_1_READ == NACK) {
    i2cbus_stop();
    return -1;
  }

  i2cbus_send(firstSlaveRegister);

  if(ACKSTAT_1_READ == NACK) {
    i2cbus_stop();
    return -1;
  }

  int i;
  for(i = 0; i < length; ++i) {
    i2cbus_send(dataBytes[i]);

    if(ACKSTAT_1_READ == NACK) {
      i2cbus_stop();
      return -1;
    }
  }

  i2cbus_stop();
  return 0;
}

/*
 * Send a message byte to a MPU9150 device.
 *
 * Returns 0 if successfull, -1 if not.
 */
int mpu9150msg_sendMessage(char slaveAddress, char slaveRegister, char dataByte) {
	char dataBytes[] = {dataByte};

	return mpu9150msg_repeatedSendMessage(slaveAddress, slaveRegister, dataBytes, 1);
}

/*
 * Performs a Burst Read Sequence to a MPU9150 device.
 *
 * Returns 0 if successfull, -1 otherwise.
 */
int mpu9150msg_repeatedReceiveMessage(char slaveAddress, char slaveRegister, int* receivedBytes, int times) {
  mpu9150msg_initIfNotAllready();

  int i;
  for(i = 0; i < times; ++i) {
    receivedBytes[i] = -1;
  }

  int status;

  i2cbus_start();

  //Configure address byte for send mode
  slaveAddress = (slaveAddress << 1);
  i2cbus_send(slaveAddress);

  if(ACKSTAT_1_READ == NACK) {
    i2cbus_stop();
    return 1;
  }

  i2cbus_send(slaveRegister);

  if(ACKSTAT_1_READ == NACK) {
	i2cbus_stop();
    return 2;
  }

  i2cbus_restart();

  //Configure address byte for receive mode
  slaveAddress = slaveAddress | 1;
  i2cbus_send(slaveAddress);

  if(ACKSTAT_1_READ == NACK) {
	i2cbus_stop();
    return 3;
  }

  for(i = 0; i < times - 1; ++i) {
    receivedBytes[i] = i2cbus_receive();

    i2cbus_generateACK(ACK);
  }

  receivedBytes[times - 1] = i2cbus_receive();

  i2cbus_generateACK(NACK);

  i2cbus_stop();

  return 0;
}

/*
 * Receive a message byte to a MPU9150 device.
 *
 * Returns 0 if successfull, -1 otherwise.
 */
int mpu9150msg_receiveMessage(char slaveAddress, char slaveRegister, int* receivedByte) {
  int receivedBytes[1];

  if (mpu9150msg_repeatedReceiveMessage(slaveAddress, slaveRegister, receivedBytes, 1)){
	  return -1;
  }

  *receivedByte = receivedBytes[0];
  return 0;
}
