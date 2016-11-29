#include "input.h"
#include "esc.h"
#include "mpu9150interface.h"

#define START_MODE_SWITCH SW1

void quad_enableInterrupts(void) {
	asm volatile("ei");
}

void quad_init(void) {
	quad_enableInterrupts();

	// Init input
	input_init();

	//Init sensors
	mpu9150interface_setup();
	mpu9150interface_awaken();

	// Init ESCs depending on switch config
	switch (input_getInput(START_MODE_SWITCH)) {
		case 1: {
			esc_init(NORMAL_START);
			break;
		}
		case 0: {
			esc_init(SET_THROTTLE_RANGE_START);
			break;
		}
	}
}

void quad_loop(void) {
	// Do good stuffs
}
