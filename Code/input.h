/*
 * A simple interface for using the inputs on the I/O shield.
 *
 * For http://github.com/Zalodu/Quadcontrol-2
 * Author: Peter Kjellén (Zalodu)
 * Date: 27/11/16
 */

#pragma once

#define BTN1 1
#define BTN2 2
#define BTN3 3
#define BTN4 4

#define SW1 5
#define SW2 6
#define SW3 7
#define SW4 8

//#define A0 9

void input_init(void);

int input_getInput (int input);

void input_getAllInputs (int* values);
