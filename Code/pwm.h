/*
 * A interface for controlling pwm outputs.
 *
 * For http://github.com/Zalodu/Quadcontrol-2
 * Author: Jesper Larsson (MrLarssonJr)
 * Date: 23/11/16
 */

#include <pic32mx.h>

/*
 * Set a new duty cycle for the specified module.
 *
 * Parameter(s):
 * module - specifies the module.
 * dutyCycle - at what timer value the pinb should be driven low.
 *
 */
void pwm_setDutyCycle(int module, int dutyCycle);

/*
 * Initialize Timer2 for use as period timer for pwm.
 *
 * Parameter(s):
 * period - timer period.
 */
void pwm_initTimer(int period);

/*
 * Initialize the specified output compare module.
 *
 * Parameter(s):
 * module - specifies module (1 <= module <= 4)
 * initialDutyCycle - the initialDutyCycle (see pwm_setDutyCycle)
 */
void pwm_initModule(int module, int initialDutyCycle);


/*
 * Stop the specified output compare module.
 *
 * Parameter(s):
 * module - specifies module (1 <= module <= 4)
 */
void pwm_stopModule(int module);
