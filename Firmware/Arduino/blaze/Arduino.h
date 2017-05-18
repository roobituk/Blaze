#ifndef ARDUINO_H
#define ARDUINO_H

#include <stdint.h>
#include <stdlib.h>

#include "Blaze.h"
/*
#ifdef __cplusplus
extern "C" {
#endif
*/
// Digital pin states
#define HIGH 0x1
#define LOW  0x0

// Digital pin modes
#define INPUT 0x0
#define OUTPUT 0x1
#define INPUT_PULLUP 0x2
#define INPUT_PULLDOWN 0x3

// Interrupt configuration
#define IRQC_DISABLED 0x0
#define IRQC_DMA_RISING 0x1
#define IRQC_DMA_FALLING 0x2
#define IRQC_DMA_EDGE 0x3
#define IRQC_LOGIC_ZERO 0x4
#define IRQC_RISING 0x5
#define IRQC_FALLING 0x6
#define IRQC_EDGE 0x7
#define IRQC_LOGIC_ONE 0x8

// DAC references 
#define DAC_REF_VREF 0x0
#define DAC_REF_VDDA 0x1



// Mapping for the digital pins
extern const digital_pin 	digital_pin_map[];
// Mapping for the analog inputs
//extern const analog_pin 	analog_pin_map[];


// ----------------------------------------------------
// Arduino compatibility methods
void pinMode(uint8_t, uint8_t);

void digitalWrite(uint8_t, uint8_t);
int digitalRead(uint8_t);

int analogRead(uint8_t);
void analogReference(uint8_t mode);
void analogWrite(int);

void setup();
void loop();

void attachInterrupt(uint8_t, void (*isr)(void), int);
void detachInterrupt(uint8_t);

unsigned long millis();
unsigned long micros();

void delay(unsigned long);
void delayMicroseconds(unsigned long);

long random(long);
//long random(long, long);
void randomSeed(unsigned long);
long map(long, long, long, long, long); 

// ----------------------------------------------------
// Blaze extension methods
void digitalToggle(uint8_t);
void digitalClear(uint8_t);
void digitalSet(uint8_t);

// Configure the digital outputs
void outputConfig(uint8_t, uint8_t, uint8_t, uint8_t);
// Configure the digitial input
void inputConfig(uint8_t, uint8_t);
// Configure the interrupt source for the digital pin
void interruptConfig(uint8_t, uint8_t);

// Configure the DAC output
void dacConfig(uint8_t, uint8_t);
// Configure the ADC inputs
void adcConfig();

// Configure the external voltage reference
void vrefConfig(uint8_t, uint16_t);




// Enter and exit critical sections
void enterCritical(uint8_t sr);
void exitCritical(uint8_t sr);
/*
#ifdef __cplusplus
}
#endif
*/
#include "Pins_Blaze.h"

#endif