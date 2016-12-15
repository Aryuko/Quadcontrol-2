/*
 * The main function for the project.
 *
 * For http://github.com/Zalodu/Quadcontrol-2
 * Author: Peter Kjellén (Zalodu), Jesper Larsson (mrlarssonjr)
 * Date: 15/12/16
 */

#include <stdint.h>
#include <pic32mx.h>
#include "quad.h"
#include "mipslab.h"
#include "input.h"

#define DEBUG

#ifdef DEBUG
	#define DEBUG_SWITCH SW2
#endif

#define SET_OFFSET_BUTTON BTN1

int main(void) {
	// Black magic initialization copied from mipslabmain
	//==========================================================================
	/* Set up peripheral bus clock */
    /* OSCCONbits.PBDIV = 1; */
    OSCCONCLR = 0x100000; /* clear PBDIV bit 1 */
	OSCCONSET = 0x080000; /* set PBDIV bit 0 */

	/* Set up output pins */
	AD1PCFG = 0xFFFF;
	ODCE = 0x0;
	TRISECLR = 0xFF;
	PORTE = 0x0;

	/* Output pins for display signals */
	PORTF = 0xFFFF;
	PORTG = (1 << 9);
	ODCF = 0x0;
	ODCG = 0x0;
	TRISFCLR = 0x70;
	TRISGCLR = 0x200;

	/* Set up input pins */
	TRISDSET = (1 << 8);
	TRISFSET = (1 << 1);

	/* Set up SPI as master */
	SPI2CON = 0;
	SPI2BRG = 4;
	/* SPI2STAT bit SPIROV = 0; */
	SPI2STATCLR = 0x40;
	/* SPI2CON bit CKP = 1; */
        SPI2CONSET = 0x40;
	/* SPI2CON bit MSTEN = 1; */
	SPI2CONSET = 0x20;
	/* SPI2CON bit ON = 1; */
	SPI2CONSET = 0x8000;

	display_init();
	display_string(0, "Awaiting orders");
	display_string(1, "Quadcopter 2.0");
	display_string(2, "By Jesper&Peter");
	display_string(3, "For IS1500");
	display_update();

	display_image(96, icon);
	//==========================================================================

	// Initialize the quad
	quad_init();

	while(1) {
		// Run the quad step function continuously
		if(input_getInput(SET_OFFSET_BUTTON)) {
			mpu9150ExtendedInterface_reset();
		}
		#ifdef DEBUG
			if (input_getInput(DEBUG_SWITCH)) {
				quad_debug();
			}
			else {
				quad_loop();
			}
		#else
			quad_loop();
		#endif
	}
	return 0;
}
