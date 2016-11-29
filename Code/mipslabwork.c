/* mipslabwork.c

This file written 2015 by F Lundevall

This file should be changed by YOU! So add something here:

This file modified 2016-09-29 by Peter Kjellén and Jesper Larsson

Latest update 2015-08-28 by F Lundevall

For copyright and licensing, see file COPYING */

#include <stdint.h>   /* Declarations of uint_32 and the like */
#include <pic32mx.h>  /* Declarations of system-specific addresses etc */
#include "mipslab.h"  /* Declatations for these labs */
#include "mpu9150msg.h"
#include "i2cbus.h"
#include "i2cmap.h"
#include "i2caddresses.h"
#include "mpu9150registers.h"
#include "mpu9150interface.h"
#include "esc.h"

#define bool char

int mytime = 0x5957;
double x = 0;

char textstring[] = "text, more text, and even more text!";

volatile int* trisE = (volatile int*) 0xBF886100;
volatile int* portE = (volatile int*) 0xBF886110;

/* Interrupt Service Routine */
void user_isr( void )
{
	// Reset interrupt flag
	IFSCLR(0) = 0x100;
	time_tick();

	if(time_getElapsedTicks() % 400 == 0) {

		time2string( textstring, mytime );
		display_string( 3, textstring );
		display_update();

		tick( &mytime );
		*portE += 1;
	}
}

void initTimer2 (int period)
{
	// Disable Timer 2 for set up by clearing
	T2CON = 0x0000;

	// Set up Timer 2 to use 1:256 scaling and set TCS bit to 0 for internal clock
	T2CON = 0x70;

	// Reset Timer register
	TMR2 = 0;

	// Set period register to given parameter
	PR2 = period;

	// Reset T2IF interrupt flag in the ISF0 register
	IFSCLR(0) = 0x100;

	// Set interrupt priorities to THE MAXIMUM
	IPCSET(2) = 0x1F;

	// Enable T2IE interrupt bit in the IEC0 register
	IECSET(0) = 0x100;
}

void enable_interrupts ( void )
{
	asm volatile("ei");

	return;
}

/* Lab-specific initialization goes here */
void labinit( void )
{
	*trisE &= ~0xFF;
	*portE = 0;

	*((volatile int*) TRISD) |= 0x7F0;
	*((volatile int*) TRISF) |= 2;

	enable_interrupts();

	esc_init(NORMAL_START);
	display_string(1, "done");

	if(!mpu9150interface_notConnected) {
		MPU9150_setup();
		MPU9150_awaken();
	}

	return;
}

/* This function is called repetitively from the main program */
void labwork( void )
{
	time_blockFor(100);
	x += 0.0025;
	if(x > 1) {
		x = 0;
	}
	display_string(0, itoaconv(x * 10000));

	esc_setSpeed(MOTOR_FRONT, x);
	esc_setSpeed(MOTOR_REAR, x);
	esc_setSpeed(MOTOR_LEFT, x);
	esc_setSpeed(MOTOR_RIGHT, x);
}
