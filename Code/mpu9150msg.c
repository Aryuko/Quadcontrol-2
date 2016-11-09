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

int initIfNotAllready(void) {
  if(!initialised) {
    if(init()) {
		return -1;
	}
    initialised = TRUE;
  }
  return 0;
}

/*
 * Send a byte message over the I2C bus.
 *
 * Returns -1 if a part of the message was not acknowledged, 1 otherwise.
 */
int sendMessage(char slaveAddress, char slaveRegister, char dataByte) {
  initIfNotAllready();

  start();

  //Configure address byte for send mode
  slaveAddress = slaveAddress & (~1);
  send(slaveAddress);

  if(ACKSTAT_1_READ == NACK) {
    stop();
    return -1;
  }

  send(slaveRegister);

  if(ACKSTAT_1_READ == NACK) {
    stop();
    return -1;
  }

  send(dataByte);

  if(ACKSTAT_1_READ == NACK) {
    stop();
    return -1;
  }

  stop();
  return 1;
}

int repeatedSendMessage(char slaveAddress, char firstSlaveRegister, char dataBytes[], int length) {
  initIfNotAllready();

  start();

  //Configure address byte for send mode
  slaveAddress = slaveAddress & (~1);
  send(slaveAddress);

  if(ACKSTAT_1_READ == NACK) {
    stop();
    return -1;
  }

  send(firstSlaveRegister);

  if(ACKSTAT_1_READ == NACK) {
    stop();
    return -1;
  }

  int i;
  for(i = 0; i < length; ++i) {
    send(dataBytes[i]);

    if(ACKSTAT_1_READ == NACK) {
      stop();
      return -1;
    }
  }

  stop();
  return 1;
}

/*
 * Receive a byte message over the I2C bus.
 *
 * Returns the byte received if successfull, -1 otherwise.
 */
int receiveMessage(char slaveAddress, char slaveRegister) {
  initIfNotAllready();

  int receivedByte;

  start();

  //Configure address byte for send mode
  slaveAddress = (slaveAddress << 1);
  send(slaveAddress);

  if(ACKSTAT_1_READ == NACK) {
    stop();
    return -1;
  }

  send(slaveRegister);

  if(ACKSTAT_1_READ == NACK) {
    stop();
    return -1;
  }

  restart();

  //Configure address byte for receive mode
  slaveAddress = slaveAddress | 1;
  send(slaveAddress);

  if(ACKSTAT_1_READ == NACK) {
    stop();
    return -1;
  }

  receivedByte = receive();

  generateACK(NACK);

  stop();

  return receivedByte;
}

int repeatedReceiveMessage(char slaveAddress, char slaveRegister, int* receivedBytes, int times) {
  if(MASTER_INTERRUPT_1_READ) {
	  return -13;
  }

  if(initIfNotAllready()) {
	  return -1;
  }

  int i;
  for(i = 0; i < times; ++i) {
    receivedBytes[i] = -1;
  }

  int status;

  if(start()) {
	  return -2;
  }

  //Configure address byte for send mode
  slaveAddress = (slaveAddress << 1);
  if(send(slaveAddress)) {
	  return -3;
  }

  if(ACKSTAT_1_READ == NACK) {
    if(stop()) {
		return -4;
	}
    return 1;
  }

  if(send(slaveRegister)) {
	  return -5;
  }

  if(ACKSTAT_1_READ == NACK) {
	  if(stop()) {
  		return -6;
  	}
    return 2;
  }

  if(restart()) {
	  return -7;
  }

  //Configure address byte for receive mode
  slaveAddress = slaveAddress | 1;
  if(send(slaveAddress)) {
	  return -8;
  }

  if(ACKSTAT_1_READ == NACK) {
	  if(stop()) {
  		return -9;
  	}
    return 3;
  }

  for(i = 0; i < times - 1; ++i) {
    receivedBytes[i] = receive();

    if(generateACK(ACK)) {
		return -10;
	}
  }

  receivedBytes[times - 1] = receive();

  if(generateACK(NACK)) {
	  return -11;
  }

  if(stop()) {
	  return -12;
  }

  return 0;
}
