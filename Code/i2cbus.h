/*
 * A simple interface for the I2C bus.
 *
 * For http://github.com/Zalodu/Quadcontrol-2
 * Author: Jesper Larsson (MrLarssonJr)
 * Date: 11/11/16
 */

#pragma once

int state;

int init(void);

int start(void);

int restart(void);

int stop(void);

int send(char byte);

int recive(void);

int generateACK(int typeACK);
