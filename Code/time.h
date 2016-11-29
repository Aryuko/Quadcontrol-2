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
