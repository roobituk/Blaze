/* ###################################################################
**     Filename    : main.c
**     Project     : BlazeUSB
**     Processor   : MK22FN1M0VLQ12
**     Version     : Driver 01.01
**     Compiler    : GNU C Compiler
**     Date/Time   : 2016-08-30, 20:01, # CodeGen: 0
**     Abstract    :
**         Main module.
**         This module contains user's application code.
**     Settings    :
**     Contents    :
**         No public methods
**
** ###################################################################*/
/*!
** @file main.c
** @version 01.01
** @brief
**         Main module.
**         This module contains user's application code.
*/         
/*!
**  @addtogroup main_module main module documentation
**  @{
*/         
/* MODULE main */


/* Including needed modules to compile this module/procedure */
#include "Cpu.h"
#include "Events.h"
#include "KSDK1.h"
#include "CS1.h"
#include "TMOUT1.h"
#include "WAIT1.h"
#include "GI2C1.h"
#include "CI2C1.h"
#include "CS1.h"
#include "UTIL1.h"
#include "SysTick.h"
#include "TU1.h"
#include "USB1.h"
#include "CDC1.h"
#include "Tx1.h"
#include "Rx1.h"
#include "USB0.h"
/* Including shared modules, which are used for whole project */
#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"
/* User includes (#include below this line is not maintained by Processor Expert) */
#include "common.h"
#include "grlib/grlib.h"
#include "grlib/lcd.h"
#include "grlib/gfx.h"
#include "grlib/st7789s.h"
#include "main.h"


// LCD address spaces
#define LCD_ADDRESS_CMD									0x60000000
#define LCD_ADDRESS_DAT									0x60010000

// I2C definitions
#define I2C0_SRC_NUM       								18
#define ONE_BYTE                        				1
#define MASTER_MODE 									1
#define SLAVE_MODE  									0
#define BUFFER_LENGTH_8BIT              				512

// The LCD graphics context
lcd_context_t g_context;



// I2C tx and rx buffers
uint8_t i2c_rx_buffer[BUFFER_LENGTH_8BIT];
uint8_t i2c_tx_buffer[BUFFER_LENGTH_8BIT];


// Variables used for the touch controller
uint8_t g_num = 0xFF;
uint8_t g_lastNum = 0;

// Finger press action
typedef enum {
	PRESS_NONE,
	PRESS_DOWN,
	PRESS_UP,
	PRESS_HOLD
} PRESS_ACTION;

// Stores the current press action
PRESS_ACTION g_action = PRESS_NONE;


// Polling variable used to periodically poll the touch screen
bool g_poll = TRUE;

uint8_t g_count = 0;



// ----------------------------------------------------------------------------
// Initialize the pin mux registers
static void Init_GPIO(void) {

	SIM_SCGC5 = SIM_SCGC5_PORTA_MASK | SIM_SCGC5_PORTB_MASK | SIM_SCGC5_PORTC_MASK | SIM_SCGC5_PORTD_MASK | SIM_SCGC5_PORTE_MASK;

	// PORTA
	PORTA_PCR0 = PORT_PCR_MUX(7);												// EXT.IO0 (JTAG)
	PORTA_PCR1 = PORT_PCR_MUX(7);												// EXT.IO1 (JTAG)
	PORTA_PCR2 = PORT_PCR_MUX(7);												// EXT.IO2 (JTAG)
	PORTA_PCR3 = PORT_PCR_MUX(7);												// EXT.IO3 (JTAG)
	PORTA_PCR4 = PORT_PCR_MUX(7);												// NMIb (NMI)
	PORTA_PCR5 = PORT_PCR_MUX(3);												// LCD.BL (PWM)
	//PORTA_PCR5 = PORT_PCR_MUX(1);												// LCD.BL (PWM)
	PORTA_PCR12 = PORT_PCR_MUX(1) | PORT_PCR_PE_MASK | PORT_PCR_PS_MASK;		// LCD.RST# (GPIO)
	PORTA_PCR13 = PORT_PCR_MUX(1) | PORT_PCR_PE_MASK | PORT_PCR_PS_MASK;		// LCD.TE# (GPIO)

	GPIOA_PDDR = (/*(1 << 0) | */(1 << 12)/* | (1 << 5)*/);
	GPIOA_PSOR = ((1 << 12)/* | (1 << 5)*/);
	//GPIOA_PCOR = (1 << 0);

	// PORTB
	PORTB_PCR0 = PORT_PCR_MUX(2) | PORT_PCR_ODE_MASK;							// TSI.SCL (I2C)
	PORTB_PCR1 = PORT_PCR_MUX(2) | PORT_PCR_ODE_MASK;							// TSI.SDA (I2C)
	PORTB_PCR2 = PORT_PCR_MUX(1) | PORT_PCR_PE_MASK | PORT_PCR_PS_MASK;			// TSI.RST# (GPIO)
	PORTB_PCR3 = PORT_PCR_MUX(1) | PORT_PCR_PE_MASK;							// TSI.INT (GPIO)
	PORTB_PCR16 = PORT_PCR_MUX(1) | PORT_PCR_PE_MASK | PORT_PCR_PS_MASK;		// SYS.LED# (GPIO)
	PORTB_PCR17 = PORT_PCR_MUX(5);												// LCD.RS (FLEXBUS)
	PORTB_PCR18 = PORT_PCR_MUX(1);												// EXT.IO4 (GPIO)
	PORTB_PCR19 = PORT_PCR_MUX(1);												// EXT.IO5 (GPIO)

	GPIOB_PDDR = ((1 << 2) | (1 << 16));
	GPIOB_PSOR = ((1 << 2) | (1 << 16));

	// PORTC
	PORTC_PCR0 = PORT_PCR_MUX(2);												// SD.CS# (SPI)
	PORTC_PCR1 = PORT_PCR_MUX(1);												// EXT.IO6 (GPIO)
	PORTC_PCR2 = PORT_PCR_MUX(1);												// EXT.IO7 (GPIO)
	PORTC_PCR3 = PORT_PCR_MUX(1) | PORT_PCR_PE_MASK | PORT_PCR_PS_MASK;			// EXT.IO8 (GPIO)
	PORTC_PCR4 = PORT_PCR_MUX(1) | PORT_PCR_PE_MASK | PORT_PCR_PS_MASK;			// EXT.IO9 (GPIO)
	PORTC_PCR5 = PORT_PCR_MUX(2);												// SD.SCLK (SPI)
	PORTC_PCR6 = PORT_PCR_MUX(2);												// SD.MOSI (SPI)
	PORTC_PCR7 = PORT_PCR_MUX(2);												// SD.MISO (SPI)
	PORTC_PCR8 = PORT_PCR_MUX(5);												// LCD.AD7 (FLEXBUS)
	PORTC_PCR9 = PORT_PCR_MUX(5);												// LCD.AD6 (FLEXBUS)
	PORTC_PCR10 = PORT_PCR_MUX(5);												// LCD.AD5 (FLEXBUS)
	PORTC_PCR11 = PORT_PCR_MUX(5);												// LCD.RD# (FLEXBUS)

	GPIOC_PDDR = (1 << 3) | (1 << 4);
	GPIOC_PSOR = (1 << 3) | (1 << 4);

	// PORTD
	PORTD_PCR0 = PORT_PCR_MUX(1);												// LCD.RD# (FLEXBUS)
	PORTD_PCR1 = PORT_PCR_MUX(5);												// LCD.CS# (FLEXBUS)
	PORTD_PCR2 = PORT_PCR_MUX(5);												// LCD.AD4 (FLEXBUS)
	PORTD_PCR3 = PORT_PCR_MUX(5);												// LCD.AD3 (FLEXBUS)
	PORTD_PCR4 = PORT_PCR_MUX(5);												// LCD.AD2 (FLEXBUS)
	PORTD_PCR5 = PORT_PCR_MUX(5);												// LCD.AD1 (FLEXBUS)
	PORTD_PCR6 = PORT_PCR_MUX(5);												// LCD.AD0 (FLEXBUS)
	PORTD_PCR7 = PORT_PCR_MUX(1);												// SD.DET (GPIO)

	PORTE_PCR0 = PORT_PCR_MUX(1);												// EXT.IO10
	PORTE_PCR1 = PORT_PCR_MUX(1);												// EXT.IO11

	GPIOC_PDDR = (1 << 0);
	GPIOC_PSOR = (1 << 0);
}

// ----------------------------------------------------------------------------
// Initialize the Flexbus peripheral used to drive the display controller
static void Init_Flexbus(void) {

	// Enable the FLEXBUS clock
	SIM_SCGC7 |= SIM_SCGC7_FLEXBUS_MASK;

	// This may not be needed (page 298 of reference manual)
	SIM_SOPT2 |= SIM_SOPT2_FBSL(3);

	// Set up the chip select register
	FB_BASE_PTR->CS[0].CSAR = LCD_ADDRESS_CMD;
	// Set 16-bit port size, auto-ack and adjusted for FB_AD[15:0]
	FB_BASE_PTR->CS[0].CSCR = (FB_CSCR_PS(2) | FB_CSCR_AA_MASK | FB_CSCR_BLS_MASK);
	//fb->CS[ch].CSCR = (FB_CSCR_PS(3) | FB_CSCR_AA_MASK | FB_CSCR_BLS_MASK | FB_CSCR_WS(2) | FB_CSCR_ASET(2) | FB_CSCR_WRAH(2) | FB_CSCR_RDAH(2));
	FB_BASE_PTR->CS[0].CSMR = FB_CSMR_BAM(0x01) | FB_CSMR_V_MASK;

	// Assert LCD.RST#
	GPIOA_PCOR = (1 << 12);

	for (int i = 0; i < 12000; i++)
		__asm__("NOP");

	// Deassert LCD.RST#
	GPIOA_PSOR = (1 << 12);

	for (int i = 0; i < 12000; i++)
		__asm__("NOP");
}

// ----------------------------------------------------------------------------
// Initializes the FlexTimer PWM module used for backlight brightness
static void Init_FTM(FTM_MemMapPtr ftm, uint8_t ch) {

	// enable the correct clock gate
	if (FTM0_BASE_PTR == ftm) {
		SIM_SCGC6 |= SIM_SCGC6_FTM0_MASK;
	}
	else if (FTM1_BASE_PTR == ftm) {
		SIM_SCGC6 |= SIM_SCGC6_FTM1_MASK;
	}
	else if (FTM2_BASE_PTR == ftm) {
		SIM_SCGC6 |= SIM_SCGC6_FTM2_MASK;
	}

	// disable write protection
	ftm->MODE |= FTM_MODE_WPDIS_MASK;

	// ensure the ftm is off
	ftm->SC = 0;

	// enable the ftm registers
	ftm->MODE &= ~FTM_MODE_FTMEN_MASK;

	ftm->QDCTRL &= ~FTM_QDCTRL_QUADEN_MASK;

	ftm->CNTIN = 0;
	ftm->CNT = 0;

	// !!!! THIS MUST REMAIN AT THIS VALUE FOR 1kHz PWM !!!
	ftm->MOD = 30000;

	ftm->CONTROLS[ch].CnSC |= FTM_CnSC_ELSB_MASK;
	ftm->CONTROLS[ch].CnSC &= ~FTM_CnSC_ELSA_MASK;
	ftm->CONTROLS[ch].CnSC |= FTM_CnSC_MSB_MASK;

	// Initialize the brightness to be 50% of maximum
	ftm->CONTROLS[ch].CnV = (ftm->MOD / 2);

	ftm->SC = FTM_SC_PS(1) | FTM_SC_CLKS(1);
}

// ----------------------------------------------------------------------------
// Sets the modulo register for the FTM which ultimately determines the duty cycle
static void FTM_SetMod(FTM_MemMapPtr ftm, uint8_t ch, uint32_t value) {

	ftm->CONTROLS[ch].CnV = value;

}

// ----------------------------------------------------------------------------
// Initialize the I2C peripheral
static void Init_I2C(void) {

	// TSI.RST# low
	GPIOB_PCOR = (1 << 2);

	for (int i = 0; i < 12000; i++)
		__asm__("NOP");

	// TSI.RST# high
	GPIOB_PSOR = (1 << 2);

	for (int i = 0; i < 12000; i++)
		__asm__("NOP");

	// Initialize the i2c port
	GI2C1_Init();

}



// ----------------------------------------------------------------------------
// Display a message on the screen
static void DisplayMessage(const char *text, uint16_t len) {

	// Create a rect to contain the text
	tRectangle rect = { 0, 180, 240, 240 };
	// Set the forground colour for when we fill the rectangle
	GrContextForegroundSet(&g_sContext, ClrBlack);
	// Fill the rectangle behind where the text will be
	GrRectFill(&g_sContext, &rect);
	// Set an arbitrary foreground colour for our text
	GrContextForegroundSet(&g_sContext, 0xF79611);
	// Draw the string
	GrStringDrawCentered(&g_sContext, text, len, 120, 200, FALSE);

}

static void ClearScreen(void) {

	GrContextForegroundSet(&g_sContext, ClrBlack);
	// Create a full-screen rectangle
	tRectangle clientRect = {0, 0, 240, 240};
	// Fill the entire area
	GrRectFill(&g_sContext, &clientRect);

}


// ----------------------------------------------------------------------------
// Init the touch screen
static void TSI_Init(void) {

	// Buffer to hold data from the touch controller
	uint8_t data[1];
	// Message to send to the touch controller
	uint8_t tx[1] = { 0xA3 };

	// Try and read the device mode from the touch controller
	GI2C1_ReadAddress(0x38, tx, 1, data, 1);

	// Work out the current operating mode
	uint8_t mode = (data[0] & 0x03);

	//printf("mode: %d", mode);
}


// ----------------------------------------------------------------------------
// Poll the touch screen
static void TSI_Poll(void) {

	// Poll the I2C device
	uint8_t data[6];
	uint8_t tx[1] = { 0x01 };

	// Try and read the touch data from the touch controller
	GI2C1_ReadAddress(0x38, tx, 1, data, sizeof(data));

	// Validate the touch data
	g_lastNum = g_num;
	// Mask the LSB, as we're only interested in single touches
	g_num = (data[1] & 0x01);

	// Check what happened
	if ((g_lastNum == 0) && (g_num == 1)) {
		// Press down
		g_action = PRESS_DOWN;


	}
	else if ((g_num == 0) && (g_lastNum == 1)) {
		// Press up
		g_action = PRESS_UP;

		// Clear the screen
		ClearScreen();
	}
	else if ((g_num == 1) && (g_lastNum == 1)) {
		// Hold
		g_action = PRESS_HOLD;

		// Get the coordinates of the first point
		int16_t x = (((data[2] & 0x0F) << 8) | data[3]);
		int16_t y = (((data[4] & 0x0F) << 8) | data[5]);

		// Draw the point
		GrContextForegroundSet(&g_sContext, ClrRed);
		// Draw a point
		GrCircleFill(&g_sContext, x, y, 4);

		// Send the touch point out over the USB CDC device
		char str[32];
		sprintf(str, "X=%d,Y=%d\r\n", x, y);
		CDC1_SendString(str);

	}
	else {
		// No press
		g_action = PRESS_NONE;
	}

/*
	if ((g_num > 0) && (g_num < 6)) {

		// Get the coordinates of the first point
		int16_t x = (((data[2] & 0x0F) << 8) | data[3]);
		int16_t y = (((data[4] & 0x0F) << 8) | data[5]);

		printf("N:%d: X:%d, Y:%d [%d]\r", g_num, x, y, evt);
	}

	if (data[0] > 0)
	  printf("Gest:%d\r", data[0]);

	if (data[1] > 0) {

	  printf("No:%d\r", data[1]);

	  uint8_t event = ((data[2] & 0x3F) >> 6);

	  printf("Evt:%d\r", event);

	  // Reading the touch points
	  printf("X:%d,Y:%d [%d]\r", ((data[2] & 0x0F) << 8 | data[3]), ((data[4] & 0x0F) << 8 | data[5]), data[6]);

	  //fflush(stdout);
	}
	*/

}

/*lint -save  -e970 Disable MISRA rule (6.3) checking. */
int main(void)
/*lint -restore Enable MISRA rule (6.3) checking. */
{
  /* Write your local variable definition here */

  /*** Processor Expert internal initialization. DON'T REMOVE THIS CODE!!! ***/
  PE_low_level_init();
  /*** End of Processor Expert internal initialization.                    ***/

  // Enable interrupts
  SEI;

  /* Write your code here */
  /* For example: for(;;) { } */

  // Initialize the GPIO pin-mux registers
  Init_GPIO();

  // Initialize the Flexbus peripheral used to drive the display
  Init_Flexbus();

  // Init the I2C peripheral for the touch-screen
  Init_I2C();

  // Initialize the graphics subsystems
  gfx_initialize(&g_context, &g_st7789s_display);
  GrContextInit(&g_sContext, &g_sDisplay);
  GrContextForegroundSet(&g_sContext, ClrBlack);
  GrContextBackgroundSet(&g_sContext, ClrBlue);
  GrContextFontSet(&g_sContext, &g_sFontExOrbitronb16);

  // Wait a period of time before bringing the backlight on
  WAIT1_Waitms(50);

  // Initialize the FTM peripheral for the backlight PWM
  Init_FTM(FTM0_BASE_PTR, 2);

  // Init the touch screen
  TSI_Init();

  // Clear the screen
  ClearScreen();

  // Main loop
  for (;;) {

	  // Poll the touch screen periodically
	  if (g_poll) {
		  // Poll the touch screen
		  TSI_Poll();
		  // Clear the poll flag (set by the timer)
		  g_poll = FALSE;
	  }



  }

  /*** Don't write any code pass this line, or it will be deleted during code generation. ***/
  /*** RTOS startup code. Macro PEX_RTOS_START is defined by the RTOS component. DON'T MODIFY THIS CODE!!! ***/
  #ifdef PEX_RTOS_START
    PEX_RTOS_START();                  /* Startup of the selected RTOS. Macro is defined by the RTOS component. */
  #endif
  /*** End of RTOS startup code.  ***/
  /*** Processor Expert end of main routine. DON'T MODIFY THIS CODE!!! ***/
  for(;;){}
  /*** Processor Expert end of main routine. DON'T WRITE CODE BELOW!!! ***/
} /*** End of main routine. DO NOT MODIFY THIS TEXT!!! ***/

/* END main */
/*!
** @}
*/
/*
** ###################################################################
**
**     This file was created by Processor Expert 10.5 [05.21]
**     for the Freescale Kinetis series of microcontrollers.
**
** ###################################################################
*/
