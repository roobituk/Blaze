/*
 ============================================================================
 Name        : main.c
 Author      : Kevin Webster
 Version     :
 Copyright   : 
 Description : Hello World in C
 ============================================================================
 */

#include "Blaze.h"

#include "Arduino.h"

/*
 *
 * Print a greeting message on standard output and exit.
 *
 * On embedded platforms this might require semi-hosting or similar.
 *
 * For example, for toolchains derived from GNU Tools for Embedded,
 * to enable semi-hosting, the following was added to the linker:
 *
 * --specs=rdimon.specs -Wl,--start-group -lgcc -lc -lc -lm -lrdimon -Wl,--end-group
 *
 * Adjust it for other toolchains.
 *
 */

int main(void) {

	// Initialize the Blaze platform
	blaze_init();

	setup();

	for (;;) {

		// Poll the touch screen
		loop();

	}

	return 0;
}






