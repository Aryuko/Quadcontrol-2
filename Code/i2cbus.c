 /*
  * A simple interface for the I2C bus.
  *
  * For http://github.com/Zalodu/Quadcontrol-2
  * Author: Jesper Larsson (MrLarssonJr)
  * Date: 11/11/16
  */

#include <pic32mx.h>
#include <stdint.h>
#include "i2cmap.h"
#include "mipslab.h"

#define UNKNOWN 10
#define PREINIT 9
#define INIT 0
#define IDLE 1
#define START 2
#define RESTART 3
#define STOP 4
#define WAIT 5
#define SEND 6
#define RECEIVE 7
#define ACK 8

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

const int waitDuration = 150;

/*=============================================================================
 * State
 */
int state = PREINIT;


/*=============================================================================
 * Helper methods
 */

/*
 * Lookups what value BRG should have for 400khz mode.
 */
int i2cbus_lookupBRG100(void) {
	int PBCLK = (DEVCFG1 >> PBCLK_START_INDEX) & PBCLK_REGISTER_MASK;

	int valueBRG;

	switch (PBCLK) {
		case PBCLK_80MHz:
			valueBRG = 0x186;
			break;
		case PBCLK_40MHz:
			valueBRG = 0x0C2;
			break;
		case PBCLK_20MHz:
			valueBRG = 0x060;
			break;
		case PBCLK_10MHz:
			valueBRG = 0x02F;
			break;
	}

	return valueBRG;
}

/*
 * Lookups what value BRG should have for 100khz mode.
 */
int i2cbus_lookupBRG400(void) {
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

	return valueBRG;
}

void i2cbus_clearMasterInterruptFlag(void) {
	IFSCLR(0) = 0x80000000;
}

void i2cbus_waitForMasterInterrupt(void) {
	while(MASTER_INTERRUPT_1_READ == 0) {}
	i2cbus_clearMasterInterruptFlag();
}

/*=============================================================================
 * Interface methods
 */

int i2cbus_init(void) {
	//Check that we are in PREINIT state
	if(state != PREINIT) {
		return -1;
	}

	//Start initialisation
	state = INIT;

	I2C1BRG = i2cbus_lookupBRG100();
	ON_1_SET;

	//Transition from INIT to IDLE
	state = IDLE;
	return 0;
}

int i2cbus_start(void) {
	//Check that we are in IDLE state
	if(state != IDLE) {
		return -1;
	}

	//Start generating the start bus event
	state = START;
	SEN_1_SET;

	//Wait for the start event to finnish
	quicksleep(waitDuration);

	//Transition from START to WAIT
	state = WAIT;
	return 0;
}

int i2cbus_restart(void) {
	//Check that we are in the WAIT state
	if(state  != WAIT) {
		return -1;
	}

	//Start generating the restart bus event
	state = RESTART;
	RSEN_1_SET;

	//Wait for the restart event to finnish
	quicksleep(waitDuration);

	//Transition from RESTART to WAIT
	state = WAIT;
	return 0;
}

int i2cbus_stop(void) {
	//Check that we are in WAIT state
	if(state != WAIT) {
		return -1;
	}

	//Start generating stop bus event
	state = STOP;
	PEN_1_SET;

	//Wait for the stop event to finnish
	quicksleep(waitDuration);

	//Transition from STOP to IDLE
	state = IDLE;
	return 0;
}

int i2cbus_send(char byte) {
	//Check that we are in WAIT state
	if(state != WAIT) {
		return -1;
	}

	//Start sending byte
	state = SEND;
	I2C1TRN = byte;

	if(IWCOL_1_READ) {
		IWCOL_1_CLR;
		state = WAIT;
		return -4;
	}

	//Wait for the transmition to finnish
	quicksleep(waitDuration);

	//Transition from SEND to WAIT
	state = WAIT;
	return 0;
}

/*
 * Recieves a byte from the I2C bus.
 *
 * Returns -1 if not in WAIT state, the byte received otherwise.
 */
int i2cbus_receive(void) {
	//Check that we are in WAIT state
	if(state != WAIT) {
		return -1;
	}

	//Start receiving bytes
	state = RECEIVE;
	RCEN_1_SET;

	//Wait for reception to finnish
	quicksleep(waitDuration);

	//Transition from RECEIVE to WAIT
	state = WAIT;

	//Return received byte
	return I2C1RCV;
}

/*
 * Generates an ACK or NACK response.
 *
 * typeACK = 0 => ACK
 * typeACK = 1 => NACK
 */
int i2cbus_generateACK(int typeACK) {
	//Check that we are in WAIT state
	if(state != WAIT) {
		return -1;
	}

	//Start generating an acknowledge event
	state = ACK;
	switch (typeACK) {
		case 0:
			ACKDT_1_CLR;
			break;
		case 1:
			ACKDT_1_SET;
			break;
	}

	ACKEN_1_SET;

	//Wait for acknowledge generation to finnish
	quicksleep(waitDuration);

	//Transition from ACK to WAIT
	state = WAIT;
	return 0;
}
