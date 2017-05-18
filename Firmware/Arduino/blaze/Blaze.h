/*
 * blaze.h
 *
 *  Created on: 20 Mar 2017
 *      Author: k.webster
 */

#ifndef BLAZE_H_
#define BLAZE_H_

#include "Common.h"

//#include "grlib/grlib.h"

//#include "display.h"


typedef struct {
	uint8_t pin;
	PORT_MemMapPtr port;
	GPIO_MemMapPtr gpio;
} digital_pin;


// LCD display context
//extern tContext 		g_context;

void blaze_init(void);






#endif /* BLAZE_H_ */
