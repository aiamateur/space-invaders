#include "uartControl.h"

// ----------------------------------------------------------------------------

void uartControl_handle(char key) {
	switch(key) {
		/* Tank Controls */
		case '4': // left
			xil_printf("tank left\r\n");
			break;
		case '6': // right
			xil_printf("tank left\r\n");
			break;
		case '5': // shoot
			xil_printf("tank shoot\r\n");
			break;


		/* Alien Controls */
		case '8': // move
			xil_printf("alien move\r\n");
			break;
		case '2': // kill
			xil_printf("alien kill\r\n");
			break;
		case '3': // shoot
			xil_printf("alien shoot\r\n");
			break;


		/* Update All Bullets */
		case '9':
			xil_printf("update all bullets\r\n");
			break;


		/* Erode Bunker */
		case '7': // query 0-3
			xil_printf("Which bunker should I erode?: ");
			char bunker = getchar(); // query for the bunker
			if (bunker >= ASCII_0 && bunker <= ASCII_3) {
				xil_printf("%c. Okay, will do.\r\n", bunker);
			} else {
				xil_printf("%c. Bailing.\r\n", bunker);
			}
			break;


		default:
			break;
	}
}

// ----------------------------------------------------------------------------