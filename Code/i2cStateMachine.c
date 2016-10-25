/*
 * A simple interface for the I2C bus.
 */

#include <pic32mx.h>
#include "i2cModule.h"

#define UNKNOWN -2
#define PREINIT -1
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
int lookupBRG(void) {
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

/*=============================================================================
 * Interface methods
 */

void init(void) {
	//Check that we are in PREINIT state
	if(state != PREINIT) {
		return;
	}

	//Start initialisation
	state = INIT;

	I2C1BRG = lookupBRG();
	ON_1_SET;

	//Transition from INIT to IDLE
	state = IDLE;
}

void start(void) {
	//Check that we are in IDLE state
	if(state != IDLE) {
		return;
	}

	//Start generating the start bus event
	state = START;
	SEN_1_SET;

	//Wait for the start event to finnish
	while(SEN_1_READ == 1) {}

	//Transition from START to WAIT
	state = WAIT;
}

void restart(void) {
	//Check that we are in the WAIT state
	if(state  != WAIT) {
		return;
	}

	//Start generating the restart bus event
	state = RESTART;
	RSEN_1_SET;

	//Wait for the restart event to finnish
	while(RSEN_1_READ == 1) {}

	//Transition from RESTART to WAIT
	state = WAIT;
}

void stop(void) {
	//Check that we are in WAIT state
	if(state != WAIT) {
		return;
	}

	//Start generating stop bus event
	state = STOP;
	PEN_1_SET;

	//Wait for the stop event to finnish
	while(PEN_1_READ == 1) {}

	//Transition from STOP to IDLE
	state = IDLE;
}

void send(char byte) {
	//Check that we are in WAIT state
	if(state != WAIT) {
		return;
	}

	//Start sending byte
	state = SEND;
	I2C1TRN = byte;

	//Wait for the transmition to finnish
	while(TRSTAT_1_READ == 1) {}

	//Transition from SEND to WAIT
	state = WAIT;
}

/*
 * Recieves a byte from the I2C bus.
 *
 * Returns -1 if not in WAIT state, the byte received otherwise.
 */
int receive(void) {
	//Check that we are in WAIT state
	if(state != WAIT) {
		return -1;
	}

	//Start receiving bytes
	state = RECEIVE;
	RCEN_1_SET;

	//Wait for reception to finnish
	while(RCEN_1_READ == 1) {}

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
void generateACK(int typeACK) {
	//Check that we are in WAIT state
	if(state != WAIT) {
		return;
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
	while(ACKEN_1_READ == 1) {}

	//Transition from ACK to WAIT
	state = WAIT;
}
