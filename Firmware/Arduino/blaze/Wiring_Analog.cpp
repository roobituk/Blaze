
#include "Arduino.h"

int analogRead(uint8_t pin) {
	
	
	
}

void analogWrite(int val) {
	
	// Write to the only available DAC output

	// Ensure that the value is 12-bit aligned
	val = val & 0x000FFFFF;
	
	DAC0_BASE_PTR->DAT[0].DATL = (uint8_t)(val);
	DAC0_BASE_PTR->DAT[0].DATH = (uint8_t)(val >> 8);
	
}



void adcConfig() {
	
}

void dacConfig(uint8_t en, uint8_t ref) {
	
	// Disable the DAC
	if (!en) {
		// Disable the DAC
		DAC0_BASE_PTR->C0 &= ~DAC_C0_DACEN_MASK;
		
		// Disable the DAC port clock
		SIM_SCGC2 &= ~SIM_SCGC2_DAC0_MASK;
		
		return;
	}
	
	// Enable the DAC port clock
	SIM_SCGC2 |= SIM_SCGC2_DAC0_MASK;
	
	// Clear any existing register values
	DAC0_BASE_PTR->DAT[0].DATL = 0;
	DAC0_BASE_PTR->DAT[0].DATH = 0;
	
	DAC0_BASE_PTR->SR = 0;
	
	DAC0_BASE_PTR->C1 = (DAC_C1_DACBFWM(0x00) | DAC_C1_DACBFMD(0x00));
	DAC0_BASE_PTR->C2 = (DAC_C2_DACBFRP(0x00) | DAC_C2_DACBFUP(0x0F));
	
	// Set the reference
	DAC0_BASE_PTR->C0 |= (ref << DAC_C0_DACRFS_SHIFT);
	
	// Enable the DAC
	DAC0_BASE_PTR->C0 |= DAC_C0_DACEN_MASK;
	
}

void vrefConfig(uint8_t en, uint16_t trim) {
	
	// Disable the VREF_OUT
	if (!en) {
		
		// Disable VREF_OUT
		VREF_BASE_PTR->SC &= ~VREF_SC_VREFEN_MASK;
		
		// Disable the DAC port clock
		SIM_SCGC4 &= ~SIM_SCGC4_VREF_MASK;
		
		return;
	}
	
	// Enable the DAC port clock
	SIM_SCGC4 |= SIM_SCGC4_VREF_MASK;
	
	// Ensure that the trim value is 6-bit aligned
	trim = trim & 0x00000FFF;
	
	VREF_BASE_PTR->TRM = VREF_TRM_TRIM(trim);
	
	// Enable VREF_OUT
	VREF_BASE_PTR->SC |= VREF_SC_VREFEN_MASK;
	
	// Wait for the VREF_OUT to stabilize
	while ((VREF_BASE_PTR->SC & VREF_SC_VREFST_MASK) != VREF_SC_VREFST_MASK) {
		
		// TODO Insert a time delay here
		
	}
	
}
