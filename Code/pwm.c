/*
 * A interface for controlling pwm outputs.
 *
 * For http://github.com/Zalodu/Quadcontrol-2
 * Author: Jesper Larsson (MrLarssonJr)
 * Date: 23/11/16
 */

#include <pic32mx.h>

const int period = 0x00C7;


/*
 * Initialize Timer2 for use as period timer for pwm.
 */
void pwm_initTimer() {
	PR2 = period; // Set period
	// Configure Timer2 interrupt. Note that in PWM mode, the
	// corresponding source timer interrupt flag is asserted.
	// OC interrupt is not generated in PWM mode.
	IFS0CLR = 0x00000100; // Clear the T2 interrupt flag
	IEC0SET = 0x00000100; // Enable T2 interrupt
	IPC2SET = 0x0000001C; // Set T2 interrupt priority to 7
	T2CONSET = 0x8000; // Enable Timer2
}

/*
 * Initialize the specified output compare module.
 *
 * Parameter:
 * module - specifies module (1 <= module <= 4)
 */
void pwm_initModule(int module) {
	switch (module) {
		case 1: {
			OC1CON = 0x0000; // Turn off the OC1 when performing the setup
			OC1CON = 0x0006; // Configure for PWM mode without Fault pin enabled

			//init the duty cycle to 100% of the period
			OC1R = period; // Initialize primary Compare register
			OC1RS = period; // Initialize secondary Compare register
			OC1CONSET = 0x8000; // Enable OC1
			break;
		}
		case 2: {
			OC2CON = 0x0000; // Turn off the OC2 when performing the setup
			OC2CON = 0x0006; // Configure for PWM mode without Fault pin enabled

			//init the duty cycle to 100% of the period
			OC2R = period; // Initialize primary Compare register
			OC2RS = period; // Initialize secondary Compare register
			OC2CONSET = 0x8000; // Enable OC2
			break;
		}
		case 3: {
			OC3CON = 0x0000; // Turn off the OC3 when performing the setup
			OC3CON = 0x0006; // Configure for PWM mode without Fault pin enabled

			//init the duty cycle to 100% of the period
			OC3R = period; // Initialize primary Compare register
			OC3RS = period; // Initialize secondary Compare register
			OC3CONSET = 0x8000; // Enable OC3
			break;
		}
		case 4: {
			OC4CON = 0x0000; // Turn off the OC4 when performing the setup
			OC4CON = 0x0006; // Configure for PWM mode without Fault pin enabled

			//init the duty cycle to 100% of the period
			OC4R = period; // Initialize primary Compare register
			OC4RS = period; // Initialize secondary Compare register
			OC4CONSET = 0x8000; // Enable OC4
			break;
		}
	}
}


/*
 * Stop the specified output compare module.
 *
 * Parameter:
 * module - specifies module (1 <= module <= 4)
 */
void pwm_stopModule(int module) {
	switch (module) {
		case 1: {
			OC1CON = 0x0000; // Turn off the OC1 when performing the setup
			break;
		}
		case 2: {
			OC2CON = 0x0000; // Turn off the OC2 when performing the setup
			break;
		}
		case 3: {
			OC2CON = 0x0000; // Turn off the OC3 when performing the setup
			break;
		}
		case 4: {
			OC2CON = 0x0000; // Turn off the OC4 when performing the setup
			break;
		}
	}
	OC1CON = 0x0000; // Turn off the OC1 when performing the setup
}


/*
 * Set a new duty cycle.
 *
 * parameter:
 * proportion - the proportion of the period the new duty cycle should be.
 *              (A value between 0 and 1.)
 *
 */
void pwm_setDutyCycle(int module, double proportion) {
	int newDutyCycle = (int) ((period*proportion) + 0.5);

	switch (module) {
		case 1: {
			OC1RS = newDutyCycle + 0.5;
			break;
		}
		case 2: {
			OC2RS = newDutyCycle + 0.5;
			break;
		}
		case 3: {
			OC3RS = newDutyCycle + 0.5;
			break;
		}
		case 4: {
			OC4RS = newDutyCycle + 0.5;
			break;
		}
	}
}
