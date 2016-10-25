#include <pic32mx.h>
#include "i2c.h"


/* NOTES ABOUT BOARD CONFIG
 * ------------------------
 * x = 1
 * That is: it is the first I2C module that is wired up.
 *
 * I2CxBRG = 44 = 0x2C for 400khz and PBCLK divisor of 1:2 (PBCLK = 40MHz)
 */

#define BRG_VALUE_80MHz 0x5A
#define BRG_VALUE_40MHz 0x2C
#define BRG_VALUE_20MHz 0x15
#define BRG_VALUE_10MHz 0x09

#define PBCLK_80MHz 0
#define PBCLK_40MHz 1
#define PBCLK_20MHz 2
#define PBCLK_10MHz 3

#define PBCLK_START_INDEX 12
#define PBCLK_REGISTER_MASK 3

#define bool char
#define TRUE 1
#define FALSE 0

//Internal helper methods======================================================
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

/* Generates a start bus event.
 *
 * If the bus is idle, then will this method generates a start sequence.
 */
bool start(void) {
	int stopped = I2C2STAT & 0x10;
	if(stopped) {
		//Set start enable bit
		I2C2CONSET = 0x1;

		//Wait for the start sequence to complete
		//Sequence complete when the start status bit is set
		while(!(I2C2STAT & 0x8)) {}
	} else {
		return FALSE;
	}
}

/* Generates a stop bus event.
 *
 *
 */
bool stop(void) {
	//Check that the master logic is inactive
	if(!(I2C2CON & 0x1F)) {
		//Master logic active
		return FALSE;
	}

	I2C2CONSET = 0x4;
}

/* Write one byte to the I2C bus.
 *
 * Attempts to write one byte to the bus, and waits until the write is complete.
 *
 * Returns TRUE if write succesful, FALSE otherwise.
 */
bool writeByte(char byte) {
	//Check if in state to send
	int bitSTART = I2C2STAT & 0x8;
	int bitTBF = I2C2STAT & 0x1;
	if(bitSTART && bitTBF == TRUE) {
		//Not in valid send state
		return FALSE;
	}

	//Attempt to send byte
	byte = byte & 0xFF;
	I2C2TRN = byte;

	//Check if attempt to send was succesful
	int flagIWCOL = I2C2STAT & 0x80;	//Supposed to be 0
	if(flagIWCOL) {
		//IWCOL flag set, write failed. Flag needs to be cleared.
		I2C2STATCLR = 0x80;
		return FALSE;
	}

	//Wait for the transmit to complete
	while(I2C2STAT & 0x4001) {}

	int bitACKSTAT = I2C2STAT & 0x8000;
	if(bitACKSTAT) {
		//ACK not received
		return FALSE;
	}

	//Transmit succesful
	return TRUE;
}

//Interface methods============================================================
void enableBus(void) {
	initModulue();

	//Start the I2C module by setting the ON-bit.
	I2C1CONSET = 0x8000;
}

void disableBus(void) {
	//Stop the I2C module by clearing the ON-bit-
	I2C1CONCLR = 0x8000;
}

void writeToSlave(char slaveAddr, char slaveReg, char data) {
	start();
	writeByte(slaveAddr);
	writeByte(slaveReg);
	writeByte(data);
	stop();
}

char readFromSlave(char slaveAddr, char slaveReg) {
	bool ableToStart = start();

}
