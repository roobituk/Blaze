
#include "Arduino.h"


// Digital pin mapping
const digital_pin digital_pin_map[] = {
	{ PTA0, 	PORTA_BASE_PTR, 	PTA_BASE_PTR },			// D0
	{ PTA1, 	PORTA_BASE_PTR, 	PTA_BASE_PTR },			// D1
	{ PTA2, 	PORTA_BASE_PTR, 	PTA_BASE_PTR },			// D2
	{ PTA3, 	PORTA_BASE_PTR, 	PTA_BASE_PTR },			// D3
	{ PTB18, 	PORTB_BASE_PTR, 	PTB_BASE_PTR },			// D4
	{ PTB19, 	PORTB_BASE_PTR, 	PTB_BASE_PTR },			// D5
	{ PTC1, 	PORTC_BASE_PTR, 	PTC_BASE_PTR },			// D6
	{ PTC2, 	PORTC_BASE_PTR, 	PTC_BASE_PTR },			// D7
	{ PTC3, 	PORTC_BASE_PTR, 	PTC_BASE_PTR },			// D8
	{ PTC4, 	PORTC_BASE_PTR, 	PTC_BASE_PTR },			// D9
	{ PTE0, 	PORTE_BASE_PTR, 	PTE_BASE_PTR },			// D10
	{ PTE1, 	PORTE_BASE_PTR, 	PTE_BASE_PTR }			// D11
};


void pinMode(uint8_t pin, uint8_t mode) {
	
	// Check for bad pin
	if (pin > NUM_DIGITAL_PINS)
		return;
	
	// Change the digital IO "pin" to be of the specified "mode"
	digital_pin map = digital_pin_map[pin];
	
	// Get the pin from the mapping table
	uint8_t index = map.pin;
	PORT_MemMapPtr port = map.port;
	GPIO_MemMapPtr gpio = map.gpio;
	
	// Change the mux if needs be
	port->PCR[index] |= PORT_PCR_MUX(1);
	
	if (mode == OUTPUT) {
		// Enable the "output" in the data direction register
		gpio->PDDR |= BIT(index);
	}
	else {
		// Enable the pull-up/down resistor
		if (mode == INPUT_PULLUP) {
			port->PCR[index] |= PORT_PCR_PE_MASK | PORT_PCR_PS_MASK;
		}
		else if (mode == INPUT_PULLDOWN) {
			port->PCR[index] |= PORT_PCR_PE_MASK;
		}
			
		// Clear the data direction register
		gpio->PDDR &= ~BIT(index); 
	}
	
}

/**
 * @brief Write a HIGH or LOW value to a digital output pin
 *
 * @param pin The digital IO pin number to write to
 * @param value HIGH or LOW
 */
void digitalWrite(uint8_t pin, uint8_t val) {
	
	// Write the specified "val" to the specified "pin"
	
	// Check for bad pin
	if (pin > NUM_DIGITAL_PINS)
		return;
	
	if (val == HIGH) {
		digitalSet(pin);
	}
	else {
		digitalClear(pin);
	}
	
}

/**
 * @brief Reads a HIGH or LOW state of a digital input pin
 *
 * @param pin The digital IO pin number to write to
 * @return 
 * 		- HIGH (1)
 *		- LOW (0)
 *		- ERROR (pin is not a digital input)
 */
int digitalRead(uint8_t pin) {
	
	// Read the value from the specified "pin"
		
	// Check for bad pin
	if (pin > NUM_DIGITAL_PINS)
		return -1;
	
	// Change the digital IO "pin" to be of the specified "mode"
	digital_pin map = digital_pin_map[pin];
	
	uint8_t index = map.pin;
	GPIO_MemMapPtr gpio = map.gpio;
	
	if (gpio->PDIR & BIT(index)) {
		return HIGH;
	}
	
	return LOW;

}

/**
 * @brief Toggles the state of a digital output pin.
 *		  @note If the output is HIGH, pin is set to
 *		  LOW. If the output is LOW, pin is set to
 *		  HIGH.
 *
 * @param pin The digital IO pin number to toggle
 */
void digitalToggle(uint8_t pin) {
	
	// Toggle the specified pin
	
	// Check for bad pin
	if (pin > NUM_DIGITAL_PINS)
		return;
	
	// Change the digital IO "pin" to be of the specified "mode"
	digital_pin map = digital_pin_map[pin];
	
	uint8_t index = map.pin;
	GPIO_MemMapPtr gpio = map.gpio;
	
	gpio->PTOR = BIT(index);
	
}

/**
 * @brief Sets the digital output pin to HIGH
 *
 * @param pin The digital IO pin number to set
 */
void digitalSet(uint8_t pin) {
	
	// Sets the specified "pin" to be "HIGH" (1)
		
	// Check for bad pin
	if (pin > NUM_DIGITAL_PINS)
		return;
	
	// Change the digital IO "pin" to be of the specified "mode"
	digital_pin map = digital_pin_map[pin];
	
	uint8_t index = map.pin;
	GPIO_MemMapPtr gpio = map.gpio;
	
	gpio->PSOR = BIT(index);
	
}

/**
 * @brief Clears the digital output pin to LOW
 *
 * @param pin The digital IO pin number to clear
 */
void digitalClear(uint8_t pin) {
	
	// Sets the specified "pin" to be "LOW" (0)
		
	// Check for bad pin
	if (pin > NUM_DIGITAL_PINS)
		return;
	
	// Change the digital IO "pin" to be of the specified "mode"
	digital_pin map = digital_pin_map[pin];
	
	uint8_t index = map.pin;
	GPIO_MemMapPtr gpio = map.gpio;
	
	gpio->PCOR = BIT(index);
	
}

/**
 * @brief Allows the configuration of the digital output pin
 *
 * @param pin The digital IO pin number to toggle
 * @param sre Slew-rate enable
 * @param ode Open-drain enable
 * @param dse Drive-strength enable
 */
void outputConfig(uint8_t pin, uint8_t sre, uint8_t ode, uint8_t dse) {
	
	// Sets the digital output configuration registers
	// SRE = slew rate enable
	// ODE = open drain enable
	// DSE = drive strength enable
	
	// Check for bad pin
	if (pin > NUM_DIGITAL_PINS)
		return;
	
	// Change the digital IO "pin" to be of the specified "mode"
	digital_pin map = digital_pin_map[pin];
	
	uint8_t index = map.pin;
	PORT_MemMapPtr port = map.port;

	port->PCR[index] |= ((sre << PORT_PCR_SRE_SHIFT) | (ode << PORT_PCR_ODE_SHIFT) | (dse << PORT_PCR_DSE_SHIFT));
	
}

void inputConfig(uint8_t pin, uint8_t pfe) {
	
	// Sets the digital input configuration registers
	// PFE = passive filter enable
	
	// Check for bad pin
	if (pin > NUM_DIGITAL_PINS)
		return;
	
	// Change the digital IO "pin" to be of the specified "mode"
	digital_pin map = digital_pin_map[pin];
	
	uint8_t index = map.pin;
	PORT_MemMapPtr port = map.port;

	port->PCR[index] |= (pfe << PORT_PCR_PFE_SHIFT);
	
}

void interruptConfig(uint8_t pin, uint8_t irqc) {
	
	// Check for bad pin
	if (pin > NUM_DIGITAL_PINS)
		return;
	
	// Change the digital IO "pin" to be of the specified "mode"
	digital_pin map = digital_pin_map[pin];
	
	uint8_t index = map.pin;
	PORT_MemMapPtr port = map.port;

	// Clear any pending interrupt
	if (irqc > 0) {
		port->PCR[index] |= PORT_PCR_ISF_MASK;
	}
	
	// Set the required interrupt
	port->PCR[index] |= PORT_PCR_IRQC(irqc);
	
}
