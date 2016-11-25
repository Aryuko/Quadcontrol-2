//PWM timings
#define MAX_THROTTLE 5000 //2ms
#define MIN_THROTTLE 2500 //1ms
#define PERIOD 6250 //2.5ms

//Startup wait times
#define NORMAL_START_SLEEP_TIME 48000000 //ish 3s
#define SET_THROTTLE_RANGE_START_SLEEP_TIME 48000000 //ish 3s

#include "esc.h"
#include "pwm.h"
#include "hardwareConfig.h"

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
void esc_init(int startupProcedure) {
  pwm_initTimer(PERIOD);

  switch (startupProcedure) {
    case NORMAL_START: {
      pwm_initModule(MOTOR_FRONT_MODULE, MIN_THROTTLE);
      pwm_initModule(MOTOR_REAR_MODULE, MIN_THROTTLE);
      pwm_initModule(MOTOR_LEFT_MODULE, MIN_THROTTLE);
      pwm_initModule(MOTOR_RIGHT_MODULE, MIN_THROTTLE);

      quicksleep(NORMAL_START_SLEEP_TIME);
      break;
    }
    case SET_THROTTLE_RANGE_START: {
      pwm_initModule(MOTOR_FRONT_MODULE, MAX_THROTTLE);
      pwm_initModule(MOTOR_REAR_MODULE, MAX_THROTTLE);
      pwm_initModule(MOTOR_LEFT_MODULE, MAX_THROTTLE);
      pwm_initModule(MOTOR_RIGHT_MODULE, MAX_THROTTLE);

      quicksleep(SET_THROTTLE_RANGE_START_SLEEP_TIME);

      esc_setSpeed(MOTOR_FRONT, 0);
      esc_setSpeed(MOTOR_REAR, 0);
      esc_setSpeed(MOTOR_LEFT, 0);
      esc_setSpeed(MOTOR_RIGHT, 0);
      break;
    }
  }
}

/*
 * Calculate a pwm duty cycle from throttle proportion.
 *
 * Parameter(s):
 * proportion - throttle proportion.
 */
int esc_calcDutyCycle(double proportion) {
  int delta = MAX_THROTTLE - MIN_THROTTLE;

  return (int) ((delta*proportion) + 0.5) + MIN_THROTTLE;
}

/*
 * Convert motor constant to module constant.
 *
 * Parameter(s):
 * motor - motor constant.
 */
int esc_motorToModule(int motor) {
  switch (motor) {
    case MOTOR_FRONT: {
      return MOTOR_FRONT_MODULE;
    }
    case MOTOR_REAR: {
      return MOTOR_REAR_MODULE;
    }
    case MOTOR_LEFT: {
      return MOTOR_LEFT_MODULE;
    }
    case MOTOR_RIGHT: {
      return MOTOR_RIGHT_MODULE;
    }
    default: {
      return -1;
    }
  }
}

/*
 * Set the speed of one motor.
 *
 * Parameter(s):
 * motor - motor constant.
 * proportionOfThrottle - proportion of throttle.
 */
void esc_setSpeed(int motor, double proportionOfThrottle) {
  pwm_setDutyCycle(esc_motorToModule(motor), esc_calcDutyCycle(proportionOfThrottle));
}
