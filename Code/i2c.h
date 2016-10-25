#pragma once

/*
 * i2c handles the communcations on the i2c bus.
 */

/* Start communicating on the I2C bus.
 *
 * Will initialise the I2C module as master in a single master enviorment and
 * start the bus.
 */
void enableBus(void);

/* Stop communicating on the I2C bus.
 *
 * Will stop the bus and disable the I2C module.
 */
void disableBus(void);

/* Write data to the addressed slave.
 *
 * Writes data to the slaveReg register of the slave with the address slaveAddr.
 */
void writeToSlave(char slaveAddr, char slaveReg, char data);

/* Read data from the addressed salve.
 *
 * Reads the slaveReg register of the slave with the address slaveAddr
 * and returns it.
 */
char readFromSlave(char slaveAddr, char slaveReg);
