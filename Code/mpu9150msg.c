#include <pic32mx.h>
#include "i2cbus.h"
#include "i2cmap.h"

#define TRUE 1
#define FALSE 0

#define ACK 0
#define NACK 1

char initialised = FALSE;

void initIfNotAllready(void) {
  if(!initialised) {
    init();
    initialised = TRUE;
  }
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

  for(int i = 0; i < length; ++i) {
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
  slaveAddress = slaveAddress & ~1;
  send(slaveAddress);

  iif(ACKSTAT_1_READ == NACK) {
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

int* repeatedReceiveMessage(char slaveAddress, char slaveRegister, int times) {
  initIfNotAllready();

  int receivedBytes[times];

  for(int i = 0; i < times; ++i) {
    receivedBytes[i] = -1;
  }

  start();

  //Configure address byte for send mode
  slaveAddress = slaveAddress & ~1;
  send(slaveAddress);

  iif(ACKSTAT_1_READ == NACK) {
    stop();
    return receivedBytes;
  }

  send(slaveRegister);

  if(ACKSTAT_1_READ == NACK) {
    stop();
    return receivedBytes;
  }

  restart();

  //Configure address byte for receive mode
  slaveAddress = slaveAddress | 1;
  send(slaveAddress);

  if(ACKSTAT_1_READ == NACK) {
    stop();
    return receivedBytes;
  }

  for(int i = 0; i < times - 1; ++i) {
    receivedBytes[i] = receive();
    generateACK(ACK);
  }

  receivedBytes[times - 1] = receive();
  generateACK(NACK);

  stop();

  return receivedBytes;

}
