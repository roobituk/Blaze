
#ifndef __lcd_h
#define __lcd_h


#include "gpio.h"


// macros defining the flexbus addresses for writing and reading
#define lcd_address_wr										0x60000000
#define lcd_address_rd										0x60010000

// macros defining the width and height of the display
#define lcd_width											240
#define lcd_height											240


#define lcd_rst												11
#define lcd_dc												17
#define lcd_cs												18


// lcd cs# control macros
#define lcd_cs_h											gpio_set_bit(GPIOB,lcd_cs)
#define lcd_cs_l											gpio_clr_bit(GPIOB,lcd_cs)

// lcd dc# control macros
#define lcd_dc_h											gpio_set_bit(GPIOB,lcd_dc)
#define lcd_dc_l											gpio_clr_bit(GPIOB,lcd_dc)

// lcd rst# control macros
#define lcd_rst_h											gpio_set_bit(GPIOC,lcd_rst)
#define lcd_rst_l											gpio_clr_bit(GPIOC,lcd_rst)





// macro converts r, g and b components into a 32ARGB number
#define rgb_to_color(r,g,b)									((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3)

// macro converts r, g and b components into an RGB565
#define rgb_to_565(r,g,b)									(((r / 8) << 11) + ((g / 4) << 5) + (b / 8))

// macro maps ARGB32 to a RGB565
#define color_to_565(color)									(((color & 0x00F80000) >> 8) | ((color & 0x0000FC00) >> 5) | ((color & 0x000000F8) >> 3))



// indicates that the font is a wide version (font_wide_t instead of font_t)
#define font_wide											0x40
// indicates that the font is an extended version (font_ex_t instead of font_t)
#define font_ex												0x80


// indicates the font is stored uncompressed
#define font_format_uncompressed							0x00
// indicates the font is stored using pixel-based rle
#define font_format_pixel_rle								0x01
// indicates the font_ex is stored uncompressed
#define font_format_ex_uncompressed							(font_format_uncompressed | font_ex)
// indicates the font_ex is stored using pixel-based rle
#define font_format_pixel_rle								(font_format_pixel_rle | font_ex)
// indicates the font_wide is stored uncompressed
#define font_format_wide_uncompressed						(font_format_uncompressed | font_wide)
// indicates the font_wide is stored using pixel-based rle
#define font_format_wide_pixel_rle							(font_format_pixel_rle | font_wide)
// indicates the font is stored in offline storage (file system, etc)
#define font_format_wrapped									0x20




#define image_format_1bpp_uncompressed						0x01

#define image_format_4bpp_uncompressed						0x04

#define image_format_8bpp_uncompressed						0x08

#define image_format_1bpp_compressed						0x81

#define image_format 4bpp_compressed						0x84

#define image_format_8bpp_compressed						0x88




typedef struct {

	// format of the font (FONT_FMT_UNCOMPRESSED or FONT_FMT_PIXEL_RLE)
	uint8_t format;

	// maximum width of a character in the font
	uint8_t max_width;

	// height of the character cell
	uint8_t height;

	// offset between the top of the character cell and the baseline of the glyph
	uint8_t baseline;

	// offset within 'data' to the data for each character in the font
	uint16_t offset[96];

	// pointer to the data for the font
	const uint8_t *data;

} font_t, *font_t_ptr;


typedef struct {

	// format of the font (FONT_FMT_UNCOMPRESSED or FONT_FMT_PIXEL_RLE)
	uint8_t format;

	// maximum width of a character in the font
	uint8_t max_width;

	// height of the character cell
	uint8_t height;

	// offset between the top of the character cell and the baseline of the glyph
	uint8_t baseline;

	// codepoint number representing the first character encoded in the font
	uint8_t first_char;
	// codepoint number representing the last character encoded in the font
	uint8_t last_char;

	// offset within 'data' to the data for each character in the font
	uint16_t offset;

	// pointer to the data for the font
	uint8_t data;

} font_ex_t, *font_ex_t_ptr;



typedef struct {

	// format of the font (FONT_FMT_UNCOMPRESSED or FONT_FMT_PIXEL_RLE)
	uint8_t format;

	// maximum width of a character in the font
	uint8_t max_width;

	// height of the character cell
	uint8_t height;

	// offset between the top of the character cell and the baseline of the glyph
	uint8_t baseline;

	// the codepage that is used to find characters in this font
	uint16_t codepage;

	// number of blocks of characters described by this font
	uint16_t block_count;

} font_wide_t, *font_wide_t_ptr;


typedef struct {

	// first codepoint in this block of characters
	uint32_t start;

	// number of characters encoded in this block
	uint32_t count;

	// offset from the beginning of the font_wide_t header to the glyph offset table
	uint32_t glyph_table_offset;

} font_block_t, *font_block_t_ptr;



typedef struct {

	// offset of each glyph in the block relative to the first entry in this table
	uint32_t glyph_offset[1];

} font_offset_table_t, *font_offset_table_t_ptr;


// font_access_funcs_t;

// font_wrapper_t;





typedef struct {

	void (*fn_write_command)(uint8_t cmd);

	void (*fn_write_byte)(uint8_t val);

	void (*fn_write_word)(uint8_t val);

	uint8_t (*fn_read_byte)(void);

	uint16_t (*fn_read_word)(void);

} lcd_controller_t, *lcd_controller_t_ptr;



// struct contains information about the lcd display
typedef struct {

	// size of this lcd_display_t struct
	int32_t size;

	// width of the display in pixels
	uint16_t width;
	// height of the display in pixels
	uint16_t height;

	// pointer to a function used to initialize the display
	void (*fn_initialize)(void);

	// pointer to a function used to draw a single picture element
	void (*fn_draw_pixel)(int16_t x, int16_t y, color_t color);

	// pointer to a function used to draw a horizontal line
	void (*fn_draw_horizontal_line)(int16_t x1, int16_t x2, int16_t y, color_t color);

	// pointer to a function used to draw a vertical line
	void (*fn_draw_vertical_line)(int16_t x, int16_t y1, int16_t y2, color_t color);

	// pointer to a function used to fill a rectangle
	void (*fn_fill_rectangle)(bounds_t_ptr bounds, color_t color);

	// clears the entire contents of the display to the specified color
	void (*fn_clear)(color_t color);

	// pointer to a function which converts a 24-bit RGB color to a display color
	color_t (*fn_translate_color)(color_t color);

} lcd_display_t;


// struct defines a context for drawing onto a display
typedef struct {

	// size of this lcd_context_t struct
	int32_t size;

	// screen onto which drawing operations are performed
	const lcd_display_t *display;

	// clipping region
	bounds_t clip_region;

	// foreground color
	color_t foreground_color;

	// background color
	color_t background_color;

} lcd_context_t;


#endif
