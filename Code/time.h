/*
 * A collection of functions for handling time for use with a timer.
 *
 * For http://github.com/Zalodu/Quadcontrol-2
 * Author: Jesper Larsson (mrlarssonjr)
 * Date: 15/12/16
 */

/*
 * Tick time.
 */
void time_tick();

/*
 * Wait for specified amount of ticks.
 *
 * Parameter(s):
 * ticks - amount of ticks to wait.
 */
void time_blockFor(unsigned int ticks);

/*
 * Get the amount of elapsed ticks.
 * Note that the ticks will wrap around when they overflow.
 */
unsigned int time_getElapsedTicks();

/*
 * Get the length of a tick in seconds.
 */
double time_getLengthOfTick();
