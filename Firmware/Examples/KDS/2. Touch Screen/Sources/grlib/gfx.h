
#ifndef __gfx_h
#define __gfx_h

#include "lcd.h"

#include "grlib.h"


extern tContext g_sContext;
extern const tDisplay g_sDisplay;

// Offscreen buffer context
extern tContext g_sOffscreenContext;
extern tDisplay g_sOffscreenDisplay;
extern uint8_t g_pucOffscreenImage[];

// Color palette
extern uint32_t g_pulPalette[];



// color definitions (in alphabetical order)
#define color_black											(0x00000000)
#define color_blue											(0x000000FF)
#define color_chartreuse									(0x00AFCB1F)
#define color_dodger_blue									(0x001E90FF)
#define color_green											(0x00008000)
#define color_hot_pink										(0x00FF69B4)
#define color_lime											(0x0000FF00)
#define color_lime_green									(0x0032CD32)
#define color_red											(0x00FF0000)
#define color_white											(0x00FFFFFF)
#define color_yellow										(0x00FFFF00)



// initializes the graphical context
void gfx_initialize(lcd_context_t *context, const lcd_display_t *display);

// draws a single picture element
void gfx_draw_pixel(lcd_context_t *context, int16_t x, int16_t y);

// draws a line
void gfx_draw_line(lcd_context_t *context, int16_t x1, int16_t y1, int16_t x2, int16_t y2);

// draws the outline of a rectangle
void gfx_draw_rectangle(lcd_context_t *context, bounds_t_ptr bounds);

// fills the interior of a rectangle
void gfx_fill_rectangle(lcd_context_t *context, bounds_t_ptr bounds);

// draws the outline of an ellipse
void gfx_draw_ellipse(lcd_context_t *context, bounds_t_ptr bounds);

// fills the interior of an ellipse
void gfx_fill_ellipse(void);

// draws a polygon
//void gfx_draw_polygon(lcd_context_t *context, const point_t *points, uint32_t count, bool closed);

// fills the interior of a polygon
//void gfx_fill_polygon(lcd_context_t *context, const point_t *points, uint32_t count);

// draws a text string
void gfx_draw_string(lcd_context_t *context, const char *str, int32_t length, int32_t x, int32_t y, uint32_t opaque);

// draws a bitmap image
void gfx_draw_image(void);

// clears the entire display area
void gfx_clear(lcd_context_t *context);


// gets the foreground color
void gfx_set_foreground_color(lcd_context_t *context, color_t color);

//color_t gfx_get_foreground_color(lcd_context_t *context);

// sets the background color
void gfx_set_background_color(lcd_context_t *context, color_t color);

//color_t gfx_get_background_color(lcd_context_t *context);


int32_t gfx_get_string_width(lcd_context_t *context, const char *str, int32_t length);




#endif
