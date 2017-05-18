
#include "DisplayContext.h"

// Flexbus addresses
#define DISPLAY_ADDR_CMD						0x60000000
#define DISPLAY_ADDR_DAT						0x60010000

// -----------------------------------------------------------------------------
DisplayContext::DisplayContext(void) {

}

// -----------------------------------------------------------------------------
DisplayContext::~DisplayContext(void) {

}

// -----------------------------------------------------------------------------
void DisplayContext::begin(void) {
	// Initialize the flexbus
	
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
		__asm__("NOP");
		
	// Send the display initialization sequence
	//DelayMs(120);
	// Send a SLPOUT
	writeCommand(0x11);
	//DelayMs(120);
	
	writeCommand(0x29);

	for (int i = 0; i < 12000; i++)
		__asm__("NOP");
	
	// Send a MADCTL
	writeCommand(0x36);
	writeByte(0x00);
	
	// Send a CASET
	writeCommand(0x2A);
	writeByte(0x00);
	writeByte(0x00);
	writeByte(0x00);
	writeByte(0xEF);
	
	// Send a RASET
	writeCommand(0x2B);
	writeByte(0x00);
	writeByte(0x00);
	writeByte(0x00);
	writeByte(0xEF);
	
	// Send COLMOD for pixel format
	writeCommand(0x3A);
	writeByte(0x55);
	
	// Send a PORCTL
	writeCommand(0xB2);
	writeByte(0x46);
	writeByte(0x4A);
	writeByte(0x01);
	writeByte(0xDE);
	writeByte(0x33);
	
	writeCommand(0xB3);
	writeByte(0x10);
	writeByte(0x05);
	writeByte(0x0F);
	
	writeCommand(0xB4);
	writeByte(0x0B);
	
	writeCommand(0xB7);
	writeByte(0x35);
	
	writeCommand(0xBB);
	writeByte(0x28);
	
	writeCommand(0xBC);
	writeByte(0xEC);
	
	writeCommand(0xC0);
	writeByte(0x2C);
	
	writeCommand(0xC2);
	writeByte(0x01);
	
	writeCommand(0xC3);
	writeByte(0x1E);
	
	writeCommand(0xC4);
	writeByte(0x20);
	
	writeCommand(0xC6);
	writeByte(0x0C);
	
	writeCommand(0xD0);
	writeByte(0xA4);
	writeByte(0xA1);
	
	writeCommand(0xE0);
	writeByte(0xD0);
	writeByte(0x00);
	writeByte(0x00);
	writeByte(0x08);
	writeByte(0x07);
	writeByte(0x05);
	writeByte(0x29);
	writeByte(0x54);
	writeByte(0x41);
	writeByte(0x3C);
	writeByte(0x17);
	writeByte(0x15);
	writeByte(0x1A);
	writeByte(0x20);
	
	writeCommand(0xE1);
	writeByte(0xD0);
	writeByte(0x00);
	writeByte(0x00);
	writeByte(0x08);
	writeByte(0x07);
	writeByte(0x04);
	writeByte(0x29);
	writeByte(0x44);
	writeByte(0x42);
	writeByte(0x3B);
	writeByte(0x16);
	writeByte(0x15);
	writeByte(0x1B);
	writeByte(0x1F);
	
	// Send a DISPON
	writeCommand(0x29);
	
	
	fillRectangle(0, 0, 240, 240, 0x00FF0000);
}

// -----------------------------------------------------------------------------
void DisplayContext::clear(Colour color) {
	
}

// -----------------------------------------------------------------------------
void DisplayContext::drawPixel(int16_t x, int16_t y, Colour color) {
	// Get the display colour
	uint16_t c = translateColour(color);
	
	// Set the column and page address
	setColumnAndPageAddress(x, y, x + 1, y + 1);
	// Write the colour
	writeWord(c);
}

// -----------------------------------------------------------------------------
void DisplayContext::drawPixels(int16_t x, int16_t y, int32_t x0, int32_t count, int32_t bpp, const uint8_t *data, const uint8_t *palette) {
	
}

// -----------------------------------------------------------------------------
void DisplayContext::drawPixelsEx(int16_t x, int16_t y, int32_t x0, int32_t count, int32_t bpp, const uint8_t *data, const uint8_t *palette, uint32_t transparent) {
	int32_t lStart, lLen, lIndex, lStartX0, lOn, lOff, lNumBytes, lBit, lDraw;
	uint32_t ulMask;
	uint8_t ucPixel;
	uint8_t bSkip;

	// What format are we dealing with?
	switch(bpp) {
		// Two colour bitmap.
		case 1: {
			// How many bytes do we need to read to cover the line of data we've been passed.
			lNumBytes = (count + x0 + 7) / 8;

			// Where must we end the line of pixels?
			lLen = count;

			// Set our mask to allow us to make either foreground or background
			// pixels transparent.
			ulMask = transparent ? 0xFF : 0;

			// Loop through the bytes in the pixel data.
			lBit = x0;
			x0 = 0;
			for(lIndex = 0; lIndex < lNumBytes; ) {
				// Count the number of off pixels from this position in the
				// glyph image.
				for(lOff = 0; lIndex < lNumBytes; )
				{
					// Get the number of zero pixels at this position.
					//count = NUM_LEADING_ZEROS(((data[lIndex] ^ ulMask) << (24 + lBit)));

					// If there were more than 8, then it is a "false" result
					// since it counted beyond the end of the current byte.
					// Therefore, simply limit it to the number of pixels
					// remaining in this byte.
					if (count > 8) {
						count = 8 - lBit;
					}

					// Increment the number of off pixels.
					lOff += count;
					// Increment the bit position within the byte.
					lBit += count;
					// See if the end of the byte has been reached.
					if(lBit == 8) {
						// Advance to the next byte and continue counting off
						// pixels.
						lBit = 0;
						lIndex++;
					}
					else {
						// Since the end of the byte was not reached, there
						// must be an on pixel.  Therefore, stop counting off
						// pixels.
						break;
					}
				}

				// Count the number of on pixels from this position in the
				// glyph image.
				for (lOn = 0; lIndex < lNumBytes; ) {
					// Get the number of one pixels at this location (by
					// inverting the data and counting the number of zeros).
					//count = NUM_LEADING_ZEROS(~(((data[lIndex] ^ ulMask) << (24 + lBit))));

					// If there were more than 8, then it is a "false" result
					// since it counted beyond the end of the current byte.
					// Therefore, simply limit it to the number of pixels
					// remaining in this byte.
					if (count > 8) {
						count = 8 - lBit;
					}

					// Increment the number of on pixels.
					lOn += count;
					// Increment the bit position within the byte.
					lBit += count;

					// See if the end of the byte has been reached.
					if(lBit == 8) {
						// Advance to the next byte and continue counting on
						// pixels.
						lBit = 0;
						lIndex++;
					}
					else {
						// Since the end of the byte was not reached, there
						// must be an off pixel.  Therefore, stop counting on
						// pixels.
						break;
					}
				}

				// At this point, we have the next off and on run lengths
				// determined so draw the on run if it is non-zero length
				// and falls within the range we need to draw.
				if(lOn && (lOff < lLen)) {
					lDraw = ((lOff + lOn) > lLen) ? (x + lLen) : (x + lOff + lOn);
					drawHorizontalLine(x + lOff, lDraw - 1, y, *(uint32_t *)(palette + (transparent ? 0 : 4)));
				}

				// Move right past these two runs.
				x += (lOff + lOn);
				lLen -= (lOff + lOn);
			}
		}
		break;

		// 4 bits per pixel (16 color) bitmap.
		case 4:
		{
			// Are we starting by drawing or skipping pixels?
			ucPixel = (data[0] >> (x0 ? 0 : 4)) & 0x0F;
			bSkip = (ucPixel == (uint8_t)transparent) ? true : false;
			lStart = 0;
			lStartX0 = x0;
			lBit = x0;
			lLen = bSkip ? 0 : 1;

			// Scan all pixels in the line of data provided.
			for(lIndex = 1; lIndex < count; lIndex++) {
				// Toggle the sub-byte pixel indicator;
				x0 = 1 - x0;

				// Read the next pixel.
				ucPixel = (data[(lIndex + lBit) / 2] >> (x0 ? 0 : 4)) & 0x0F;

				// Is this pixel a transparent one?
				if(ucPixel != (uint8_t)transparent) {
					// It's not transparent.  Have we just ended a run of
					// transparent pixels?
					if(bSkip) {
						// We are currently skipping pixels so this starts a new run.
						lStart = lIndex;
						lStartX0 = x0;
						lLen = 1;
						bSkip = false;
					}
					else {
						//
						// We were already in the middle of a run of non-
						// transparent pixels so increment the run length.
						//
						lLen++;
					}
				}
				else {
					// Pixel is transparent.  Do we have a run to draw?
					if(!bSkip) {
						// Yes - draw what we have.
						drawPixels(x + lStart, y, lStartX0, lLen, 4, &data[(lStart + lBit) / 2], palette);

						// Reset for the transparent run.
						lLen = 0;
						bSkip = true;
					}
				}
			}

			// If we drop out of the pixel loop with a run not drawn, draw it here.
			if(!bSkip && lLen) {
				drawPixels(x + lStart, y, lStartX0, lLen, 4, &data[(lStart + lBit) / 2], palette);
			}
		}
		break;

		//
		// 8 bit per pixel (256 color) bitmap.
		//
		case 8:
		{
			// Are we starting by drawing or skipping pixels?
			bSkip = (data[0] == (uint8_t)transparent) ? true : false;
			lStart = 0;
			lLen = bSkip ? 0 : 1;

			// Scan all pixels in the line of data provided.
			for(lIndex = 1; lIndex < count; lIndex++) {
				// Is this pixel a transparent one?
				if(data[lIndex] != (uint8_t)transparent) {
					// It's not transparent.  Have we just ended a run of transparent pixels?
					if(bSkip) {
						// We are currently skipping pixels so this starts a new run.
						lStart = lIndex;
						lLen = 1;
						bSkip = false;
					}
					else {
						// We were already in the middle of a run of non-
						// transparent pixels so increment the run length.
						lLen++;
					}
				}
				else {
					// Pixel is transparent.  Do we have a run to draw?
					if(!bSkip) {
						// Yes - draw what we have.
						drawPixels(x + lStart, y, 0, lLen, 8, &data[lStart], palette);

						// Reset for the transparent run.
						lLen = 0;
						bSkip = true;
					}
				}
			}

			// If we drop out of the pixel loop with a run not drawn, draw it here.
			if(!bSkip && lLen){
				drawPixels(x + lStart, y, x0, lLen, 8, &data[lStart], palette);
			}
		}
		break;
	}
}

// -----------------------------------------------------------------------------
void DisplayContext::drawHorizontalLine(int16_t x1, int16_t x2, int16_t y, Colour color) {
	// Get the display colour
	uint16_t c = translateColour(color);
	
	// Set the column and page address
	setColumnAndPageAddress(x1, y, x2, y);
	// Write the colours
	for (; x1 <= x2; x1++) {
		writeWord(c);
	}
}

// -----------------------------------------------------------------------------
void DisplayContext::drawVerticalLine(int16_t x, int16_t y1, int16_t y2, Colour color) {
	// Get the display colour
	uint16_t c = translateColour(color);
	
	// Set the column and page address
	setColumnAndPageAddress(x, y1, x, y2);
	// Write the colours
	for (; y1 <= y2; y1++) {
		writeWord(c);
	}
}

// -----------------------------------------------------------------------------
void DisplayContext::fillRectangle(int16_t x, int16_t y, int16_t width, int16_t height, Colour color) {
	// Get the display colour
	uint16_t c = translateColour(color);
	
	// Set the column and page address
	setColumnAndPageAddress(x, y, x + width, y + height);
	// Write the colours
	for (uint16_t y = 0; y < height; y++) {
		for (uint16_t x = 0; x < width; x++) {
			writeWord(c);
		}
	}
}

// -----------------------------------------------------------------------------
uint32_t DisplayContext::translateColour(Colour color) {
	return (((color.value() & 0x00F80000) >> 8) | ((color.value() & 0x0000FC00) >> 5) | ((color.value() & 0x000000F8) >> 3));
}

// -----------------------------------------------------------------------------
void DisplayContext::flush(void) {
	
}

// -----------------------------------------------------------------------------
void DisplayContext::setColumnAndPageAddress(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2) {
	uint16_t temp;
	
	// Swap the coordinates if necessary
	if (x1 > x2) {
		temp = x1;
		x1 = x2;
		x2 = temp;
	}
	
	if (y1 > y2) {
		temp = y1;
		y1 = y2;
		y2 = temp;
	}
	
	writeCommand(0x2A);
	writeWord(x1);
	writeWord(x2);
	
	writeCommand(0x2B);
	writeWord(y1);
	writeWord(y2);
	
	writeCommand(0x2C);
}

// -----------------------------------------------------------------------------
void DisplayContext::writeCommand(uint8_t command) {
	*((uint16_t *)DISPLAY_ADDR_CMD) = command;
}

// -----------------------------------------------------------------------------
void DisplayContext::writeByte(uint8_t value) {
	*((uint16_t *)DISPLAY_ADDR_DAT) = value;
}

// -----------------------------------------------------------------------------
void DisplayContext::writeWord(uint16_t value) {
	*((uint16_t *)DISPLAY_ADDR_DAT) = (value >> 8);
	*((uint16_t *)DISPLAY_ADDR_DAT) = value;
}


