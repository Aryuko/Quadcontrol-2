#include <pic32mx.h>
#include "input.h"
#include "esc.h"
#include "vector.h"
#include "mpu9150interface.h"
#include "mpu9150ExtendedInterface.h"
#include "mpu9150msg.h"
#include "mpu9150registers.h"
#include "i2caddresses.h"

#define START_MODE_SWITCH SW1

int mytime = 0x5957;
volatile int* portE = (volatile int*) 0xBF886110;
char textstring[] = "text, more text, and even more text!";

/* Interrupt Service Routine */
void user_isr( void )
{
	// Reset interrupt flag
	IFSCLR(0) = 0x100;
	time_tick();

	if(time_getElapsedTicks() % 400 == 0) {
		/* Increment value for leds */
		*portE += 1;
	}
}

void quad_enableInterrupts(void) {
	asm volatile("ei");
}

void quad_init(void) {
	quad_enableInterrupts();

	// Init input
	display_string(0, "init input");
	display_update();
	input_init();

	//Init sensors
	display_string(0, "init sensors");
	display_update();
	if(!mpu9150interface_notConnected()) {
		mpu9150ExtendedInterface_init();
	}

	// Init ESCs depending on switch config
	display_string(0, "init ESCs");
	display_update();
	switch (input_getInput(START_MODE_SWITCH)) {
		case 0: {
			display_string(0, "ESC normal start");
			display_update();
			esc_init(NORMAL_START);
			break;
		}
		case 1: {
			display_string(0, "ESC tRange start");
			display_update();
			esc_init(SET_THROTTLE_RANGE_START);
			break;
		}
	}

	display_string(0, "init done");
	display_update();
}

/*
 * Volatile functions used for testing purposes
 */
double x = 0;
void quad_debug (void) {
	if(mpu9150interface_notConnected()) {
		mpu9150ExtendedInterface_init();
	} else {
		mpu9150ExtendedInterface_tick();

		display_string(0, itoaconv((int) (mpu9150ExtendedInterface_getInclinationDerivative().x + 0.5)));
		display_string(1, itoaconv((int) (mpu9150ExtendedInterface_getInclinationDerivative().y + 0.5)));
		display_string(2, itoaconv((int) (mpu9150ExtendedInterface_getInclinationDerivative().z + 0.5)));
	}

	display_update();

	time_blockFor(100);

	x += 0.0025;
	if(x > 1.0) {
		x = 0.0;
	}

	display_string(3, itoaconv((int) (x * 10000)));

	esc_setSpeed(MOTOR_FRONT, x);
	esc_setSpeed(MOTOR_REAR, x);
	esc_setSpeed(MOTOR_LEFT, x);
	esc_setSpeed(MOTOR_RIGHT, x);
}

void quad_loop(void) {
	// Do good stuffs
	quad_debug();
}
