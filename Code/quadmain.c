#include <stdint.h>
#include <pic32mx.h>
#include "quad.h"
#include "mipslab.h"

#define DEBUG

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
	display_string(3, "TIME");
	display_update();

	display_image(96, icon);
	//==========================================================================

	// Initialize the quad
	quad_init();

	while(1) {
		// Run the quad step function continuously
		#ifdef DEBUG
		quad_loop();
		#else
		quad_debug();
		#endif
	}
	return 0;
}
