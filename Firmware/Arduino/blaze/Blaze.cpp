/*
 * blaze.c
 *
 *  Created on: 20 Mar 2017
 *      Author: k.webster
 */

#include "Blaze.h"

// LCD display context
//tContext 		g_context;

static void wdog_init(void) {

}

static void clocks_init(void) {

	// set up some crystal capacitance
	OSC_CR = OSC_CR_SC2P_MASK | OSC_CR_SC16P_MASK;

	MCG_C2 = MCG_C2_EREFS0_MASK | MCG_C2_RANGE0(2);

	// select the external reference clock and divide by 512 to get 31.25kHz
	MCG_C1 = MCG_C1_CLKS(2) | MCG_C1_FRDIV(4);

	// wait for OSCINIT0
	while (!(MCG_S & MCG_S_OSCINIT0_MASK)) { }

	// wait for IREFST
	while (MCG_S & MCG_S_IREFST_MASK) { }

	// wait until the external reference clock is selected
	while ((MCG_S & MCG_S_CLKST_MASK) != MCG_S_CLKST(2)) { }

	// divide by 4 for a PLL0 frequency of 4MHz
	MCG_C5 = MCG_C5_PRDIV0(3);

	// multiply by 30 to give 120MHz
	MCG_C6 = MCG_C6_CME0_MASK | MCG_C6_PLLS_MASK | MCG_C6_VDIV0(6);

	// wait until PLL is the source for PLLS
	while (!(MCG_S & MCG_S_PLLST_MASK)) { }

	// wait for the PLL to lock
	while (!(MCG_S & MCG_S_LOCK0_MASK)) { }

	// set up the clock divisors
	// core clock 		= 120MHz (div = 1)
	// bus clock		= 60MHz  (div = 2)
	// flexbus clock	= 40MHz  (div = 3)
	// flash clock		= 24MHz  (div = 5)
	SIM_CLKDIV1 = SIM_CLKDIV1_OUTDIV1(0) | SIM_CLKDIV1_OUTDIV2(1) | SIM_CLKDIV1_OUTDIV3(2) | SIM_CLKDIV1_OUTDIV4(4);

	// switch over to pll0
	MCG_C1 = MCG_C1_CLKS(0) | MCG_C1_FRDIV(4);

	// wait until the PLL is selected to feed MCGOUTCLK
	while ((MCG_S & MCG_S_CLKST_MASK) != MCG_S_CLKST(3)) { }

}

static void gpio_init(void) {

	// Enable ALL port clocks for the GPIO ports
	SIM_SCGC5 = SIM_SCGC5_PORTA_MASK | SIM_SCGC5_PORTB_MASK | SIM_SCGC5_PORTC_MASK | SIM_SCGC5_PORTD_MASK | SIM_SCGC5_PORTE_MASK;

	// PORTA
	//PORTA_PCR0 = PORT_PCR_MUX(7);												// EXT.IO0 (JTAG)
	//PORTA_PCR1 = PORT_PCR_MUX(7);												// EXT.IO1 (JTAG)
	//PORTA_PCR2 = PORT_PCR_MUX(7);												// EXT.IO2 (JTAG)
	//PORTA_PCR3 = PORT_PCR_MUX(7);												// EXT.IO3 (JTAG)
	PORTA_PCR0 = PORT_PCR_MUX(1);												// EXT.IO0 (GPIO)
	PORTA_PCR1 = PORT_PCR_MUX(1);												// EXT.IO1 (GPIO)
	PORTA_PCR2 = PORT_PCR_MUX(1);												// EXT.IO2 (GPIO)
	PORTA_PCR3 = PORT_PCR_MUX(1);												// EXT.IO3 (GPIO)
	PORTA_PCR4 = PORT_PCR_MUX(7);												// NMIb (NMI)
	PORTA_PCR5 = PORT_PCR_MUX(3);												// LCD.BL (PWM)
	PORTA_PCR12 = PORT_PCR_MUX(1) | PORT_PCR_PE_MASK | PORT_PCR_PS_MASK;		// LCD.RST# (GPIO)
	PORTA_PCR13 = PORT_PCR_MUX(1) | PORT_PCR_PE_MASK | PORT_PCR_PS_MASK;		// LCD.TE# (GPIO)

	GPIOA_PDDR = BIT(12);
	GPIOA_PSOR = BIT(12);

	// PORTB
	PORTB_PCR0 = PORT_PCR_MUX(2) | PORT_PCR_ODE_MASK;							// TSI.SCL (I2C)
	PORTB_PCR1 = PORT_PCR_MUX(2) | PORT_PCR_ODE_MASK;							// TSI.SDA (I2C)
	PORTB_PCR2 = PORT_PCR_MUX(1) | PORT_PCR_PE_MASK | PORT_PCR_PS_MASK;			// TSI.RST# (GPIO)
	PORTB_PCR3 = PORT_PCR_MUX(1) | PORT_PCR_PE_MASK;							// TSI.INT (GPIO)
	PORTB_PCR16 = PORT_PCR_MUX(1) | PORT_PCR_PE_MASK | PORT_PCR_PS_MASK;		// SYS.LED# (GPIO)
	PORTB_PCR17 = PORT_PCR_MUX(5);												// LCD.RS (FLEXBUS)
	PORTB_PCR18 = PORT_PCR_MUX(1);												// EXT.IO4 (GPIO)
	PORTB_PCR19 = PORT_PCR_MUX(1);												// EXT.IO5 (GPIO)

	GPIOB_PDDR = (BIT(2) | BIT(16));
	GPIOB_PSOR = (BIT(2) | BIT(16));

	// PORTC
	PORTC_PCR0 = PORT_PCR_MUX(2);												// SD.CS# (SPI)
	PORTC_PCR1 = PORT_PCR_MUX(1);												// EXT.IO6 (GPIO)
	PORTC_PCR2 = PORT_PCR_MUX(1);												// EXT.IO7 (GPIO)
	PORTC_PCR3 = PORT_PCR_MUX(1);												// EXT.IO8 (GPIO)
	PORTC_PCR4 = PORT_PCR_MUX(1);												// EXT.IO9 (GPIO)
	PORTC_PCR5 = PORT_PCR_MUX(2);												// SD.SCLK (SPI)
	PORTC_PCR6 = PORT_PCR_MUX(2);												// SD.MOSI (SPI)
	PORTC_PCR7 = PORT_PCR_MUX(2);												// SD.MISO (SPI)
	PORTC_PCR8 = PORT_PCR_MUX(5);												// LCD.AD7 (FLEXBUS)
	PORTC_PCR9 = PORT_PCR_MUX(5);												// LCD.AD6 (FLEXBUS)
	PORTC_PCR10 = PORT_PCR_MUX(5);												// LCD.AD5 (FLEXBUS)
	PORTC_PCR11 = PORT_PCR_MUX(5);												// LCD.WR# (FLEXBUS)

	GPIOC_PDDR = (BIT(3) | BIT(4));
	GPIOC_PSOR = (BIT(3) | BIT(4));

	// PORTD
	PORTD_PCR0 = PORT_PCR_MUX(1);												// LCD.RD# (FLEXBUS)
	PORTD_PCR1 = PORT_PCR_MUX(5);												// LCD.CS# (FLEXBUS)
	PORTD_PCR2 = PORT_PCR_MUX(5);												// LCD.AD4 (FLEXBUS)
	PORTD_PCR3 = PORT_PCR_MUX(5);												// LCD.AD3 (FLEXBUS)
	PORTD_PCR4 = PORT_PCR_MUX(5);												// LCD.AD2 (FLEXBUS)
	PORTD_PCR5 = PORT_PCR_MUX(5);												// LCD.AD1 (FLEXBUS)
	PORTD_PCR6 = PORT_PCR_MUX(5);												// LCD.AD0 (FLEXBUS)
	PORTD_PCR7 = PORT_PCR_MUX(1);												// SD.DET (GPIO)

	PORTE_PCR0 = PORT_PCR_MUX(1);												// EXT.IO10 (GPIO)
	PORTE_PCR1 = PORT_PCR_MUX(1);												// EXT.IO11 (GPIO)

	GPIOC_PDDR = BIT(0);
	GPIOC_PSOR = BIT(0);

}

static void tsi_init(void) {

}

static void lcd_init(void) {

/*
	// enable the clock gate
	SIM_SCGC7 |= SIM_SCGC7_FLEXBUS_MASK;

	// this may not be needed (page 298 of reference manual)
	SIM_SOPT2 |= SIM_SOPT2_FBSL(3);

	// set up the chip select register
	FB_BASE_PTR->CS[0].CSAR = DISPLAY_ADDR_CMD;
	// set 16-bit port size, auto-ack and adjusted for FB_AD[15:0]
	FB_BASE_PTR->CS[0].CSCR = (FB_CSCR_PS(2) | FB_CSCR_AA_MASK | FB_CSCR_BLS_MASK);
	FB_BASE_PTR->CS[0].CSMR = (FB_CSMR_BAM(0x01) | FB_CSMR_V_MASK);

	// Assert LCD.RST#
	GPIOA_PCOR = (1 << 12);

	for (int i = 0; i < 12000; i++)
		__asm__("NOP");

	// Deassert LCD.RST#
	GPIOA_PSOR = (1 << 12);

	for (int i = 0; i < 12000; i++)
		__asm__("NOP");*/

}

static void display_init(void) {

/*
	// Initialize the graphics context
	GrContextInit(&g_context, &g_display);

	// Set the default context values
	GrContextForegroundSet(&g_context, ClrGreen);
	GrContextBackgroundSet(&g_context, ClrBlack);

	// Clear the screen
	tRectangle rect = { 0, 0, DISPLAY_WIDTH, DISPLAY_HEIGHT };
	GrRectFill(&g_context, &rect);
*/
}

static void backlight_init(void) {
	
	// enable the correct clock gate
	SIM_SCGC6 |= SIM_SCGC6_FTM0_MASK;

	// disable write protection
	FTM0_BASE_PTR->MODE |= FTM_MODE_WPDIS_MASK;

	// ensure the ftm is off
	FTM0_BASE_PTR->SC = 0;

	// enable the ftm registers
	FTM0_BASE_PTR->MODE &= ~FTM_MODE_FTMEN_MASK;

	FTM0_BASE_PTR->QDCTRL &= ~FTM_QDCTRL_QUADEN_MASK;

	FTM0_BASE_PTR->CNTIN = 0;
	FTM0_BASE_PTR->CNT = 0;

	// !!!! THIS MUST REMAIN AT THIS VALUE FOR 1kHz PWM !!!
	FTM0_BASE_PTR->MOD = 30000;

	FTM0_BASE_PTR->CONTROLS[2].CnSC |= FTM_CnSC_ELSB_MASK;
	FTM0_BASE_PTR->CONTROLS[2].CnSC &= ~FTM_CnSC_ELSA_MASK;
	FTM0_BASE_PTR->CONTROLS[2].CnSC |= FTM_CnSC_MSB_MASK;

	// Initialize the brightness to be 50% of maximum
	FTM0_BASE_PTR->CONTROLS[2].CnV = (FTM0_BASE_PTR->MOD / 2);

	FTM0_BASE_PTR->SC = FTM_SC_PS(1) | FTM_SC_CLKS(1);
	
}



void blaze_init(void) {

	// Disable the watchdog
	wdog_init();

	// Initialize the clocks
	clocks_init();

	// Initialize the GPIO
	gpio_init();

	// Initialize the touch screen
	//tsi_init();

	// Initialize the LCD
	//lcd_init();

	// Initialize the display system
	//display_init();
	
	// Initialize the backlight
	backlight_init();

}





// Enter a critical section
void enterCritical(uint8_t sr) {
	
	asm(
		"MRS r0, PRIMASK 	\n\t"
		"CPSID i 			\n\t"
		"STRB r0, %[output] \n\t"
		: [output] "=m" (sr) :: "r0"
	);
}

// Exit a critical section
void exitCritical(uint8_t sr) {
	
	asm (
		"LDRB r0, %[input]  \n\t"
		"MSR PRIMASK, r0    \n\t"
		::[input] "m" (sr) : "r0"
	); 
}


