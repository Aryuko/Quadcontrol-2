/*
 * A interface for controlling pwm outputs.
 *
 * For http://github.com/Zalodu/Quadcontrol-2
 * Author: Jesper Larsson (MrLarssonJr)
 * Date: 23/11/16
 */

#include <pic32mx.h>

/*
 * Initialize Timer2 for use as period timer for pwm.
 */
void pwm_initTimer();

/*
 * Initialize the specified output compare module.
 *
 * Parameter:
 * module - specifies module (1 <= module <= 4)
 */
void pwm_initModule(int module);


/*
 * Stop the specified output compare module.
 *
 * Parameter:
 * module - specifies module (1 <= module <= 4)
 */
void pwm_stopModule(int module);


/*
 * Set a new duty cycle.
 *
 * parameter:
 * proportion - the proportion of the period the new duty cycle should be.
 *              (A value between 0 and 1.)
 *
 */
void pwm_setDutyCycle(int module, double proportion);
