/*
 * A simple interface for the I2C bus.
 *
 * For http://github.com/Zalodu/Quadcontrol-2
 * Author: Jesper Larsson (MrLarssonJr)
 * Date: 11/11/16
 */

#pragma once

int i2cbus_init(void);

int i2cbus_start(void);

int i2cbus_restart(void);

int i2cbus_stop(void);

int i2cbus_send(char byte);

int i2cbus_receive(void);

int i2cbus_generateACK(int typeACK);
