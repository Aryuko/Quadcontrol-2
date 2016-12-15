/*
 * An interface to handle ESCs through PWM.
 *
 * For http://github.com/Zalodu/Quadcontrol-2
 * Author: Jesper Larsson (mrlarssonjr)
 * Date: 15/12/16
 */

#define MOTOR_FRONT 1
#define MOTOR_REAR 2
#define MOTOR_LEFT 3
#define MOTOR_RIGHT 4

#define NORMAL_START 0
#define SET_THROTTLE_RANGE_START 1

/*
 * Initialize all ESCs.
 *
 * Parameter(s):
 * startupProcedure - used to choose what startup procedure to use.
 *                    Possible choices:
 *                    --(Name)-------------------(Value)----------------------
 *                    * Normal start             (NORMAL_START)
 *                    * Set Throttle Range Start (SET_THROTTLE_RANGE_START)
 *
 */
void esc_init(int startupProcedure);

/*
 * Calculate a pwm duty cycle from throttle proportion.
 *
 * Parameter(s):
 * proportion - throttle proportion.
 */
int esc_calcDutyCycle(double proportion);

/*
 * Convert motor constant to module constant.
 *
 * Parameter(s):
 * motor - motor constant.
 */
int esc_motorToModule(int motor);

/*
 * Set the speed of one motor.
 *
 * Parameter(s):
 * motor - motor constant.
 * proportionOfThrottle - proportion of throttle.
 */
void esc_setSpeed(int motor, double proportionOfThrottle);
