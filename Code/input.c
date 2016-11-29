/*
 * A simple interface for using the inputs on the I/O shield.
 *
 * For http://github.com/Zalodu/Quadcontrol-2
 * Author: Peter Kjellén (Zalodu)
 * Date: 27/11/16
 */

#include <stdint.h>
#include <pic32mx.h>
#include "input.h"

/*
 * Returns the value of the specified input. (See header file for definitions)
 */
int input_getInput (int input) {
  int value;

  if (input == BTN1) {
    //BTN 1 at RF1
    value = *((volatile int*) PORTF);
  }
  else if (input <= SW4) {
    //BTN2 at RD5, BTN3 at RD6, BTN4 at RD7
    //SW1 at RD8, SW2 at RD9, SW3 at RD10, SW4 at RD11
    input += 3; //(to line it up with the RD registers, 2=>RD5, 6=>RD7 etc.)
    value = *((volatile int*) PORTD);
  }
  /*else if (input == A0) {
    //Potentiometer at RB2
    input = 2; //(to line it up with RB2)
    value = *((volatile int*) PORTB);
}*/

  int mask = 1 << input;
  return (value & mask) >> input;
}


/*
 * Fills the specified array with the values of all inputs.
 */
void input_getAllInputs (int* values) {
  int value;

  //BTN 1 at RF1
  value = *((volatile int*) PORTF);
  values[0] = (value & 2) >> 1;

  //BTN2 at RD5, BTN3 at RD6, BTN4 at RD7
  //SW1 at RD8, SW2 at RD9, SW3 at RD10, SW4 at RD11
  value = *((volatile int*) PORTD);
  int i;
  for(i = 5; i <= 11; i++) {
    int mask = 1 << i;
    values[i-4] = (value & mask) >> i;
  }

  //Potentiometer at RB2
  value = *((volatile int*) PORTB);
  int mask = 1 << i;
  values[8] = (value & 1) >> 1;
  return;
}
