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

#define bool char

int mytime = 0x5957;
int timeoutcount = 0;

char textstring[] = "text, more text, and even more text!";

volatile int* trisE = (volatile int*) 0xBF886100;
volatile int* portE = (volatile int*) 0xBF886110;

/* Interrupt Service Routine */
void user_isr( void )
{
	// Reset interrupt flag
	IFSCLR(0) = 0x100;

	if(++timeoutcount == 10) {
		timeoutcount = 0;

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

	// Init timer using period = 8E06/256/10 = 31250, for 10 updates per second
	initTimer2(31250);
	enable_interrupts();

	// Start Timer 2
	T2CON |= 0x8000;

	init();
	/*start();
	send(0x69);*/

	/*int status1 = start();
	quicksleep(1000000);
	int status3 = send(0x69);
	int status2 = stop();

	display_string(0, itoaconv(status1));
	display_string(1, itoaconv(status3));
	display_string(2, itoaconv(status2));
	display_update();

	/*status1 = start();
	status3 = send(0x69);
	status2 = stop();

	display_string(0, itoaconv(status1));
	display_string(1, itoaconv(status3));
	display_string(2, itoaconv(status2));
	display_update();*/

	return;
}

/* This function is called repetitively from the main program */
void labwork( void )
{
	buttons2Time();



	int status1 = start();
	quicksleep(50);
	int status3 = send(0x69);
	quicksleep(150);
	int status2 = stop();

	display_string(0, itoaconv(status1));
	display_string(1, itoaconv(status3));
	display_string(2, itoaconv(status2));

	/*int data[1];
	int status = repeatedReceiveMessage(0x69, 0x75, data, 1);
	int temp = data[0];

	display_string(0, itoaconv(MASTER_INTERRUPT_1_READ));
	display_string(1, itoaconv(status));
	display_string(2, itoaconv(temp));*/


	display_update();
}
