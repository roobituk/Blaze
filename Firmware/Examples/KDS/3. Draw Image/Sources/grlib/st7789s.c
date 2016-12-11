// Driver for the ST7789s controller

#include "common.h"

#include "lcd.h"

#include "st7789s.h"

#include "gfx.h"



#define LCD_ADDRESS_CMD															0x60000000
#define LCD_ADDRESS_DAT															0x60010000

#define st7789s_width										(240)
#define st7789s_height										(240)


// driver for the ILI9341 display controller
void st7789s_delay(uint32_t value) {

	while (value != 0) {
		value--;
	}

}


void st7789s_write_command(uint8_t command) {

	*((uint16_t *)LCD_ADDRESS_CMD) = command;


}

void st7789s_write_byte(uint8_t value) {

	*((uint16_t *)LCD_ADDRESS_DAT) = value;

}

void st7789s_write_word(uint16_t value) {

	*((uint16_t *)LCD_ADDRESS_DAT) = (value >> 8);
	*((uint16_t *)LCD_ADDRESS_DAT) = value;

}


void st7789s_set_column_page(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2) {

	uint16_t temp;

	// swap left and right if necessary
	if (x1 > x2) {
		temp = x1;
		x1 = x2;
		x2 = x1;
	}

	// swap top and bottom if necessary
	if (y1 > y2) {
		temp = y1;
		y1 = y2;
		y2 = temp;
	}

	// set the column address
	st7789s_write_command(0x2A);
	// start column
	st7789s_write_byte((x1 >> 8));
	st7789s_write_byte(x1);
	// end column
	st7789s_write_byte((x2 >> 8));
	st7789s_write_byte(x2);

	// set the page address
	st7789s_write_command(0x2B);
	// start page
	st7789s_write_byte((y1 >> 8));
	st7789s_write_byte(y1);
	// end page
	st7789s_write_byte((y2 >> 8));
	st7789s_write_byte(y2);

	// memory write
	st7789s_write_command(0x2C);

	// now pixel data...

}




// initializes the ili9341 controller
void st7789s_initialize(void) {

	st7789s_write_command(0x11);

	st7789s_write_command(0x29);


	st7789s_delay(1000000);

	st7789s_write_command(0x36);
	st7789s_write_byte(0x00);

	st7789s_write_command(0x35);
	st7789s_write_byte(0x00);

	st7789s_write_command(0x2a);
	st7789s_write_byte(0x00);
	st7789s_write_byte(0x00);
	st7789s_write_byte(0x00);
	st7789s_write_byte(0xef);

	st7789s_write_command(0x2b);
	st7789s_write_byte(0x00);
	st7789s_write_byte(0x00);
	st7789s_write_byte(0x00);
	st7789s_write_byte(0xef);

	st7789s_write_command(0x3A);
	st7789s_write_byte(0x55);

	st7789s_write_command(0xb2);
	st7789s_write_byte(0x46);
	st7789s_write_byte(0x4a);
	st7789s_write_byte(0x01);
	st7789s_write_byte(0xde);
	st7789s_write_byte(0x33);

	st7789s_write_command(0xb3);
	st7789s_write_byte(0x10);
	st7789s_write_byte(0x05);
	st7789s_write_byte(0x0f);

	st7789s_write_command(0xb4);
	st7789s_write_byte(0x0b);

	st7789s_write_command(0xb7);
	st7789s_write_byte(0x35);

	st7789s_write_command(0xbb);
	st7789s_write_byte(0x28);

	st7789s_write_command(0xbc);
	st7789s_write_byte(0xec);

	st7789s_write_command(0xc0);
	st7789s_write_byte(0x2c);

	st7789s_write_command(0xc2);
	st7789s_write_byte(0x01);

	st7789s_write_command(0xc3);
	st7789s_write_byte(0x1e);

	st7789s_write_command(0xc4);
	st7789s_write_byte(0x20);

	st7789s_write_command(0xc6);
	st7789s_write_byte(0x0C);

	st7789s_write_command(0xd0);
	st7789s_write_byte(0xa4);
	st7789s_write_byte(0xa1);

	st7789s_write_command(0xe0);
	st7789s_write_byte(0xd0);
	st7789s_write_byte(0x00);
	st7789s_write_byte(0x00);
	st7789s_write_byte(0x08);
	st7789s_write_byte(0x07);
	st7789s_write_byte(0x05);
	st7789s_write_byte(0x29);
	st7789s_write_byte(0x54);
	st7789s_write_byte(0x41);
	st7789s_write_byte(0x3c);
	st7789s_write_byte(0x17);
	st7789s_write_byte(0x15);
	st7789s_write_byte(0x1a);
	st7789s_write_byte(0x20);

	st7789s_write_command(0xe1);
	st7789s_write_byte(0xd0);
	st7789s_write_byte(0x00);
	st7789s_write_byte(0x00);
	st7789s_write_byte(0x08);
	st7789s_write_byte(0x07);
	st7789s_write_byte(0x04);
	st7789s_write_byte(0x29);
	st7789s_write_byte(0x44);
	st7789s_write_byte(0x42);
	st7789s_write_byte(0x3b);
	st7789s_write_byte(0x16);
	st7789s_write_byte(0x15);
	st7789s_write_byte(0x1b);
	st7789s_write_byte(0x1f);

	st7789s_write_command(0x29);

}

void st7789s_draw_pixel(int16_t x, int16_t y, color_t color) {

	// set the column and page addresses
	st7789s_set_column_page(x, y, x + 1, y + 1);
	// write the pixel color
	st7789s_write_word(color);

}

void st7789s_draw_horizontal_line(int16_t x1, int16_t x2, int16_t y, color_t color) {

	// set the column and page addresses
	st7789s_set_column_page(x1, y, x2, y + 1);
	// write each pixel in the line
	for (; x1 <= x2; x1++) {
		st7789s_write_word(color);
	}

}

void st7789s_draw_vertical_line(int16_t x, int16_t y1, int16_t y2, color_t color) {

	// set the column and page addresses
	st7789s_set_column_page(x, y1, x, y2);
	// write each pixel in the line
	for (; y1 <= y2; y1++) {
		st7789s_write_word(color);
	}

}

void st7789s_fill_rectangle(bounds_t_ptr bounds, color_t color) {

	int16_t rows = (bounds->right - bounds->left);
	int16_t cols = (bounds->bottom - bounds->top);

	// set the column and page addresses
	st7789s_set_column_page(bounds->left, bounds->top, bounds->right - 1, bounds->bottom - 1);
	// write each pixel in the display area
	for (uint16_t col = 0; col < cols; col++) {
		for (uint16_t page = 0; page < rows; page++) {
			st7789s_write_word(color);
		}
	}

}

void st7789s_clear(color_t color) {

	// set the column and page addresses
	st7789s_set_column_page(0, 0, (st7789s_width - 1), (st7789s_height - 1));
	// write each pixel in the display area
	for (uint16_t col = 0; col < st7789s_width; col++) {
		for (uint16_t page = 0; page < st7789s_height; page++) {
			st7789s_write_word(color);
		}
	}

}

// translates the ARGB32 color into a display color
color_t st7789s_translate_color(color_t color) {

	return color_to_565(color);

}

// initialize the lcd_display struct containing information about our display
const lcd_display_t g_st7789s_display = {

	sizeof(lcd_display_t),
	240,
	240,
	st7789s_initialize,
	st7789s_draw_pixel,
	st7789s_draw_horizontal_line,
	st7789s_draw_vertical_line,
	st7789s_fill_rectangle,
	st7789s_clear,
	st7789s_translate_color
};








/* -----------------------------------------------------------------------------
GRLIB SUPPORT - TO BE REMOVED
------------------------------------------------------------------------------*/



uint32_t lcd_hw_translateColor(void *buffer, uint32_t color) {

	return color_to_565(color);

}

void lcd_hw_drawPixel(void *buffer, int32_t x, int32_t y, uint32_t color) {

	// set the column and page addresses
	st7789s_set_column_page(x, y, x + 1, y + 1);
	// write the pixel color
	st7789s_write_word(color);

}

void lcd_hw_drawPixels(void *buffer, int32_t x, int32_t y, int32_t offset, int32_t count, int32_t bpp, const uint8_t *pixels, const uint8_t *palette) {

	uint32_t b;

    // set the column and page addresses
	st7789s_set_column_page(x, y, x + count, y + 1);

    // Determine how to interpret the pixel data based on the number of bits per pixel.
    switch(bpp) {
        // The pixel data is in 1 bit per pixel format.
        case 1: {
            // Loop while there are more pixels to draw.
            while (count) {
                // Get the next byte of image data.
                b = *pixels++;

                // Loop through the pixels in this byte of image data.
                for(; (offset < 8) && count; offset++, count--) {
                    // Draw this pixel in the appropriate color.
					st7789s_write_word(((uint32_t *)palette)[(b >> (7 - offset)) & 1]);
                }

                // Start at the beginning of the next byte of image data.
                offset = 0;
            }

            // The image data has been drawn.
            break;
        }

        // The pixel data is in 4 bit per pixel format.
        case 4: {
            // Loop while there are more pixels to draw.  "Duff's device" is
            // used to jump into the middle of the loop if the first nibble of
            // the pixel data should not be used.  Duff's device makes use of
            // the fact that a case statement is legal anywhere within a
            // sub-block of a switch statement.  See
            // http://en.wikipedia.org/wiki/Duff's_device for detailed
            // information about Duff's device.
            switch(offset & 1) {
                case 0:
                    while(count) {
                        // Get the upper nibble of the next byte of pixel data
                        // and extract the corresponding entry from the
                        // palette.
                        b = (*pixels >> 4) * 3;
                        b = (*(unsigned long *)(palette + b) & 0x00ffffff);

                        // Translate this palette entry and write it to the screen.
						st7789s_write_word(lcd_hw_translateColor(NULL, b));
                        //WriteData(DPYCOLORTRANSLATE(b));

                        // Decrement the count of pixels to draw.
                        count--;

                        // See if there is another pixel to draw.
                        if(count) {
                case 1:
                            // Get the lower nibble of the next byte of pixel
                            // data and extract the corresponding entry from
                            // the palette.
                            b = (*pixels++ & 15) * 3;
                            b = (*(uint32_t *)(palette + b) & 0x00ffffff);

                            // Translate this palette entry and write it to the
                            // screen.
							st7789s_write_word(lcd_hw_translateColor(NULL, b));
                            //WriteData(DPYCOLORTRANSLATE(b));

                            // Decrement the count of pixels to draw.
                            count--;
                        }
                    }
            }

            // The image data has been drawn.
            break;
        }

        // The pixel data is in 8 bit per pixel format.
        case 8: {
            // Loop while there are more pixels to draw.
            while(count--) {
                // Get the next byte of pixel data and extract the
                // corresponding entry from the palette.
                b = *pixels++ * 3;
                b = *(uint32_t *)(palette + b) & 0x00ffffff;

                // Translate this palette entry and write it to the screen.
				st7789s_write_word(lcd_hw_translateColor(NULL, b));
                //WriteData(DPYCOLORTRANSLATE(b));
            }

            // The image data has been drawn.
            break;
        }
    }

}

void lcd_hw_drawHorizontalLine(void *buffer, int32_t x1, int32_t x2, int32_t y, uint32_t color) {

	// set the column and page addresses
	st7789s_set_column_page(x1, y, x2, y + 1);
	// write each pixel in the line
	for (; x1 <= x2; x1++) {
		st7789s_write_word(color);
	}

}

void lcd_hw_drawVerticalLine(void *buffer, int32_t x, int32_t y1, int32_t y2, uint32_t color) {

	// set the column and page addresses
	st7789s_set_column_page(x, y1, x, y2);
	// write each pixel in the line
	for (; y1 <= y2; y1++) {
		st7789s_write_word(color);
	}

}


void lcd_hw_fillRectangle(void *buffer, const tRectangle *rectangle, uint32_t color) {


	int16_t rows = (rectangle->sXMax - rectangle->sXMin);
	int16_t cols = (rectangle->sYMax - rectangle->sYMin);

	// set the column and page addresses
	st7789s_set_column_page(rectangle->sXMin, rectangle->sYMin, rectangle->sXMax - 1, rectangle->sYMax - 1);
	// write each pixel in the display area
	for (uint16_t col = 0; col < cols; col++) {
		for (uint16_t page = 0; page < rows; page++) {
			st7789s_write_word(color);
		}
	}

}


void lcd_hw_flush(void *buffer) {

	// Do nothing

}

const tDisplay g_sDisplay = {
	sizeof(tDisplay),
	0,
	240,
	240,
	lcd_hw_drawPixel,
	lcd_hw_drawPixels,
	lcd_hw_drawHorizontalLine,
	lcd_hw_drawVerticalLine,
	lcd_hw_fillRectangle,
	lcd_hw_translateColor,
	lcd_hw_flush
};
