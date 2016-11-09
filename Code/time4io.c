/*time4io.c
	This file was written by Peter Kjellén and Jesper Larsson 2016-09-27
*/

#include <stdint.h>
#include <pic32mx.h>
#include "mipslab.h"

int getsw( void )
{
	int r = *((volatile int*) PORTD);

	r = (r & 0xF00) >> 8;

	return r;
}

int getbtns( void ) {
	int d = *((volatile int*) PORTD);
	int f = *((volatile int*) PORTF);

	d = (d & 0xE0) >> 4;
	f = (f & 2) >> 1;

	int r = d | f;

	return r;
}
