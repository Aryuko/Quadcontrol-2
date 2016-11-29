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
	display_string(0, "Initialized input");

	//Init sensors
	mpu9150interface_setup();
	mpu9150interface_awaken();
	display_string(0, "Initialized sensors");

	// Init ESCs depending on switch config
	switch (input_getInput(START_MODE_SWITCH)) {
		case 0: {
			display_string(0, "Doing normal ESC start");
			esc_init(NORMAL_START);
			break;
		}
		case 1: {
			display_string(0, "Setting throttle range on ESC start");
			esc_init(SET_THROTTLE_RANGE_START);
			break;
		}
	}
	display_string(0, "Initialized ESCs");

	display_string(0, "Initialization done");
}

void quad_loop(void) {
	// Do good stuffs
}
