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
	display_string(0, "init input");
	display_update();
	input_init();

	//Init sensors
	display_string(0, "init sensors");
	display_update();
	mpu9150interface_setup();
	mpu9150interface_awaken();

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

void quad_loop(void) {
	// Do good stuffs
}
