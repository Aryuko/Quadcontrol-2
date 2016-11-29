#include "input.h"
#include "esc.h"
#include "vector.h"
#include "mpu9150interface.h"
#include "mpu9150ExtendedInterface.h"

#define START_MODE_SWITCH SW1

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
	mpu9150ExtendedInterface_init();

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
void quad_debug (void) {
	if(mpu9150interface_notConnected()) {
		display_string(0, "connection fail");
		display_update();
		mpu9150ExtendedInterface_init();
	} else {
		mpu9150ExtendedInterface_tick();
		display_string(0, itoaconv(mpu9150ExtendedInterface_getInclination().x));
	}

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

void quad_loop(void) {
	// Do good stuffs
	quad_debug();
}
