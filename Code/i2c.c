#include <pic32mx.h>
#include "i2c.h"


/* NOTES ABOUT BOARD CONFIG
 * ------------------------
 * x = 1
 * That is: it is the first I2C module that is wired up.
 *
 * I2CxBRG = 44 = 0x2C for 400khz and PBCLK divisor of 1:2 (PBCLK = 40MHz)
 */

const int BRG_VALUE_80MHz = 0x5A;
const int BRG_VALUE_40MHz = 0x2C;
const int BRG_VALUE_20MHz = 0x15;
const int BRG_VALUE_10MHz = 0x09;

const int PBCLK_80MHz = 0;
const int PBCLK_40MHz = 1;
const int PBCLK_20MHz = 2;
const int PBCLK_10MHz = 3;

const int PBCLK_START_INDEX = 12;
const int PBCLK_REGISTER_MASK = 3;


/* Initialise the I2C module without starting it.
 *
 * Sets up the module for operating in 400khz mode.
 * The set up is independent of what the PBCLK is.
 * (Note: if the PBCLK divisor is 1:1, the software should NOT read nor write to
 *  the peripheral’s SFRs in the SYSCLK cycle immediately following the
 *  instruction that clears the module’s ON bit.
 *  - "Inter-Integrated Circuit.pdf", page 24-7, note 2)
 */
void initModulue(void) {
	int PBCLK = (DEVCFG1 >> PBCLK_START_INDEX) & PBCLK_REGISTER_MASK;

	int valueBRG;

	switch (PBCLK) {
		case PBCLK_80MHz:
			valueBRG = BRG_VALUE_80MHz;
			break;
		case PBCLK_40MHz:
			valueBRG = BRG_VALUE_40MHz;
			break;
		case PBCLK_20MHz:
			valueBRG = BRG_VALUE_20MHz;
			break;
		case PBCLK_10MHz:
			valueBRG = BRG_VALUE_10MHz;
			break;
	}

	I2C1BRG = valueBRG;
}

void enableBus(void) {
	initModulue();

	//Start the I2C module by setting the ON-bit.
	I2C1CONSET = 0x8000;
}

void disableBus(void) {
	//Stop the I2C module by clearing the ON-bit-
	I2C1CONCLR = 0x8000;
}
