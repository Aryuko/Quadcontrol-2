/*
 * A interface for controlling pwm outputs.
 *
 * For http://github.com/Zalodu/Quadcontrol-2
 * Author: Jesper Larsson (MrLarssonJr)
 * Date: 23/11/16
 */

#include <pic32mx.h>

//#define OC1CON			PIC32_R (0x3000)
#define OC1CONCLR		PIC32_R (0x3004)
#define OC1CONSET		PIC32_R (0x3008)
#define OC1CONINV		PIC32_R (0x300c)

//#define OC4R			PIC32_R (0x3010)
#define OC1RCLR			PIC32_R (0x3014)
#define OC1RSET			PIC32_R (0x3018)
#define OC1RINV			PIC32_R (0x301c)

//#define OC1RS			PIC32_R (0x3020)
#define OC1RSCLR		PIC32_R (0x3024)
#define OC1RSSET		PIC32_R (0x3028)
#define OC1RSINV		PIC32_R (0x302c)

#define OC2CON			PIC32_R (0x3200)
#define OC2CONCLR		PIC32_R (0x3204)
#define OC2CONSET		PIC32_R (0x3208)
#define OC2CONINV		PIC32_R (0x320c)

#define OC2R			PIC32_R (0x3210)
#define OC2RCLR			PIC32_R (0x3214)
#define OC2RSET			PIC32_R (0x3218)
#define OC2RINV			PIC32_R (0x321c)

#define OC2RS			PIC32_R (0x3220)
#define OC2RSCLR		PIC32_R (0x3224)
#define OC2RSSET		PIC32_R (0x3228)
#define OC2RSINV		PIC32_R (0x322c)


#define OC3CON			PIC32_R (0x3400)
#define OC3CONCLR		PIC32_R (0x3404)
#define OC3CONSET		PIC32_R (0x3408)
#define OC3CONINV		PIC32_R (0x340c)

#define OC3R			PIC32_R (0x3410)
#define OC3RCLR			PIC32_R (0x3414)
#define OC3RSET			PIC32_R (0x3418)
#define OC3RINV			PIC32_R (0x341c)

#define OC3RS			PIC32_R (0x3420)
#define OC3RSCLR		PIC32_R (0x3424)
#define OC3RSSET		PIC32_R (0x3428)
#define OC3RSINV		PIC32_R (0x342c)

//#define OC4CON			PIC32_R (0x3600)
#define OC4CONCLR		PIC32_R (0x3604)
#define OC4CONSET		PIC32_R (0x3608)
#define OC4CONINV		PIC32_R (0x360c)

//#define OC4R			PIC32_R (0x3610)
#define OC4RCLR			PIC32_R (0x3614)
#define OC4RSET			PIC32_R (0x3618)
#define OC4RINV			PIC32_R (0x361c)

//#define OC4RS			PIC32_R (0x3620)
#define OC4RSCLR		PIC32_R (0x3624)
#define OC4RSSET		PIC32_R (0x3628)
#define OC4RSINV		PIC32_R (0x362c)

//0xC350 = 20ms
const int period = 0xC350 >> 3;
const int noThrottle = 2500;
const int fullThrottle = 5000;

/*
 * Calculate a duty cycle value.
 *
 * parameter:
 * proportion - the proportion of the period the new duty cycle should be.
 *              (A value between 0 and 1.)
 */
int pwm_calcDutyCycle(double proportion) {
	int delta = fullThrottle - noThrottle;

	return (int) ((delta*proportion) + 0.5) + noThrottle;
}

/*
 * Set a new duty cycle for the specified module.
 *
 * parameter:
 * module - specifies the module.
 * proportion - the proportion of the period the new duty cycle should be.
 *              (A value between 0 and 1.)
 *
 */
void pwm_setDutyCycle(int module, double proportion) {
	switch (module) {
		case 1: {
			OC1RS = pwm_calcDutyCycle(proportion);
			break;
		}
		case 2: {
			OC2RS = pwm_calcDutyCycle(proportion);
			break;
		}
		case 3: {
			OC3RS = pwm_calcDutyCycle(proportion);
			break;
		}
		case 4: {
			OC4RS = pwm_calcDutyCycle(proportion);
			break;
		}
	}
}

/*
 * Initialize Timer2 for use as period timer for pwm.
 */
void pwm_initTimer() {
	PR2 = period; // Set period
	// Configure Timer2 interrupt. Note that in PWM mode, the
	// corresponding source timer interrupt flag is asserted.
	// OC interrupt is not generated in PWM mode.
	IFSCLR(0) = 0x00000100; // Clear the T2 interrupt flag
	IECSET(0) = 0x00000100; // Enable T2 interrupt
	IPCSET(2) = 0x0000001C; // Set T2 interrupt priority to 7

	T2CONSET = 0x50; // Set prescale to 1:32
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
			OC1R = pwm_calcDutyCycle(1); // Initialize primary Compare register
			OC1RS = pwm_calcDutyCycle(1); // Initialize secondary Compare register
			OC1CONSET = 0x8000; // Enable OC1
			break;
		}
		case 2: {
			OC2CON = 0x0000; // Turn off the OC2 when performing the setup
			OC2CON = 0x0006; // Configure for PWM mode without Fault pin enabled

			//init the duty cycle to 100% of the period
			OC2R = pwm_calcDutyCycle(1.0); // Initialize primary Compare register
			OC2RS = pwm_calcDutyCycle(1.0); // Initialize secondary Compare register
			OC2CONSET = 0x8000; // Enable OC2
			break;
		}
		case 3: {
			OC3CON = 0x0000; // Turn off the OC3 when performing the setup
			OC3CON = 0x0006; // Configure for PWM mode without Fault pin enabled

			//init the duty cycle to 100% of the period
			OC3R = pwm_calcDutyCycle(1.0); // Initialize primary Compare register
			OC3RS = pwm_calcDutyCycle(1.0); // Initialize secondary Compare register
			OC3CONSET = 0x8000; // Enable OC3
			break;
		}
		case 4: {
			OC4CON = 0x0000; // Turn off the OC4 when performing the setup
			OC4CON = 0x0006; // Configure for PWM mode without Fault pin enabled

			//init the duty cycle to 100% of the period
			OC4R = pwm_calcDutyCycle(1.0); // Initialize primary Compare register
			OC4RS = pwm_calcDutyCycle(1.0); // Initialize secondary Compare register
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
