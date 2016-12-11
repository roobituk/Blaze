
#include "common.h"

#include "lcd.h"

#include "gfx.h"


tContext g_sContext;


#define max_polygon_nodes									100

#define font_absent_char									'.'

// represents the portion of the lcd display to update
bounds_t gfx_invalid_bounds;

// initialize the graphical context
void gfx_initialize(lcd_context_t *context, const lcd_display_t *display) {

	context->size = sizeof(lcd_context_t);

	// initialize the clip region
	context->clip_region.left 	= 0;
	context->clip_region.top 	= 0;
	context->clip_region.right 	= (display->width - 1);
	context->clip_region.bottom = (display->height - 1);

	// initialize default colors
	context->foreground_color = color_white;
	context->background_color = color_red;

	// assign the display
	context->display = display;

	// initialize the display
	context->display->fn_initialize();

	context->display->fn_clear(color_black);

}



// draws a single picture element
void gfx_draw_pixel(lcd_context_t *context, int16_t x, int16_t y) {

}


// draws a horizontal line
void gfx_draw_horizontal_line(lcd_context_t *context, int16_t x1, int16_t x2, int16_t y) {

	int16_t temp;

	// if we are outside the clipping region, return
	if ((y < context->clip_region.top) || (y > context->clip_region.bottom))
		return;

	// swap x coords if necessary
	if (x1 > x2) {
		temp = x1;
		x1 = x2;
		x2 = temp;
	}

	// if the line is drawn outside the clipping region, return
	if ((x1 > context->clip_region.right) || (x2 < context->clip_region.left))
		return;

	// clip the line
	if (x1 < context->clip_region.left)
		x1 = context->clip_region.left;

	if (x2 > context->clip_region.right)
		x2 = context->clip_region.right;

	// draw the line
	context->display->fn_draw_horizontal_line(x1, x2, y, context->foreground_color);

}

// draws a vertical line
void gfx_draw_vertical_line(lcd_context_t *context, int16_t x, int16_t y1, int16_t y2) {

	int16_t temp;

	if ((x < context->clip_region.left) || (x > context->clip_region.right))
		return;

	// swap y coords if necessary
	if (y1 > y2) {
		temp = y1;
		y1 = y2;
		y2 = y1;
	}

	// if the line is drawn outside the clipping region, return
	if ((y1 > context->clip_region.bottom) || (y2 < context->clip_region.top))
		return;

	// clip the line
	if (y1 < context->clip_region.top)
		y1 = context->clip_region.top;

	if (y2 > context->clip_region.bottom)
		y2 = context->clip_region.bottom;

	// draw the line
	context->display->fn_draw_vertical_line(x, y1, y2, context->foreground_color);

}

// draws a line
void gfx_draw_line(lcd_context_t *context, int16_t x1, int16_t y1, int16_t x2, int16_t y2) {

	// for efficiency, we avoid using the bresenham algorithm if the line is
	// either vertical or horizontal. this algorithm only gets used for diagonals

	// check for horizontal line (same y value)
	if (y1 == y2) {
		gfx_draw_horizontal_line(context, x1, x2, y1);
		return;
	}

	// check for vertical line (same x value)
	if (x1 == x2) {
		gfx_draw_horizontal_line(context, x1, y1, y2);
		return;
	}

	// steep lines
	uint8_t steep = 0;

	// work out the gradients for both x and y
	int16_t delta_x = (x2 > x1) ? (x2 - x1) : (x1 - x2);
	int16_t delta_y = (y2 > y1) ? (y2 - y1) : (y1 - y2);

	// check for a steep line
	if (delta_y > delta_x)
		steep = 1;

	int16_t temp;

	if (steep) {
		temp = x1;
		x1 = y1;
		y1 = temp;
		temp = x2;
		x2 = y2;
		x2 = temp;
	}

	// if the starting x coordinate is larger than the end, swap the coordinates
	if (x1 > x2) {
		temp = x1;
		x1 = x2;
		x2 = x1;
		temp = y1;
		y1 = y2;
		y2 = temp;
	}

	// recompute the x and y deltas
	delta_x = (x2 - x1);
	delta_y = (y2 - y1);

	// initialize the error term of negative half x delta
	int16_t error = -delta_x / 2;

	// initialize the y step direction
	int16_t step_y = -1;

	// determine the y step direction
	if (y1 < y2)
		step_y = 1;

	// loop through all points on the x axis
	for (; x1 <= x2; x1++) {

		if (steep) {

			// draw the point, swap x and y
			context->display->fn_draw_pixel(y1, x1, context->foreground_color);
			//lcd_draw_pixel(y1, x1, context->foreground_color);

		}
		else {

			// draw the point, keeping x and y
			context->display->fn_draw_pixel(x1, y1, context->foreground_color);
			//lcd_draw_pixel(x1, y1, context->foreground_color);

		}

		// increase the error term by y delta
		error += delta_y;

		// if the error term is greater than zero
		if (error > 0) {
			// step in the y axis
			y1 += step_y;
			// decrease the error term by x delta
			error -= delta_x;
		}

	}

}

// draws the outline of a rectangle
void gfx_draw_rectangle(lcd_context_t *context, bounds_t_ptr bounds) {

	// draw a line across the top of the rectangle
	gfx_draw_horizontal_line(context, bounds->left, bounds->right, bounds->top);

	// if the rectangle is 1px tall, return
	if (bounds->top == bounds->bottom) return;

	// draw a line down the right of the rectangle
	gfx_draw_vertical_line(context, bounds->right, bounds->top, bounds->bottom);

	// if the rectangle is 1px wide, return
	if (bounds->left == bounds->right) return;

	// draw a line across the bottom of the rectangle
	gfx_draw_horizontal_line(context, bounds->left, bounds->right, bounds->bottom);

	// if the rectangle is 2px tall, return
	if ((bounds->left + 1) == bounds->right) return;

	// draw a line down the left of the rectangle
	gfx_draw_vertical_line(context, bounds->left, bounds->top, bounds->bottom);

}

// fills the interior of a rectangle
void gfx_fill_rectangle(lcd_context_t *context, bounds_t_ptr bounds) {

	bounds_t temp;

	// TODO check to see if we're outside the clip region

	// swap y coords if necessary
	if (bounds->top > bounds->bottom) {
		temp.top = bounds->bottom;
		temp.bottom = bounds->top;
	}
	else {
		temp.top = bounds->top;
		temp.bottom = bounds->bottom;
	}

	// swap x coords if necessary
	if (bounds->left > bounds->right) {
		temp.left = bounds->right;
		temp.right = bounds->left;
	}
	else {
		temp.left = bounds->left;
		temp.right = bounds->right;
	}

	context->display->fn_fill_rectangle(&temp, context->background_color);

}

// draws the outline of an ellipse
void gfx_draw_ellipse(lcd_context_t *context, bounds_t_ptr bounds) {

}

void gfx_fill_ellipse(void) {

}

/*
void gfx_draw_polygon(lcd_context_t *context, const point_t *points, uint32_t count, bool closed) {

	// iterate through all the points and draw them
	for (uint32_t i = 0; i < (count - 1); i++) {
		// draw the first point
		gfx_draw_line(context, points[i].x, points[i].y, points[i+1].x, points[i+1].y);
	}

	// should we close the polygon?
	if (closed) {
		// draw a closing line between the first and last points
		gfx_draw_line(context, points[0].x, points[0].y, points[count-1].x, points[count-1].y);
	}

}

void gfx_fill_polygon(lcd_context_t *context, const point_t *points, uint32_t count) {

	uint32_t idx1, idx2;
	uint32_t nodes;
	int16_t y;
	int16_t nodes_list[max_polygon_nodes];
	int16_t swap;

	// loop through the rows of the clipping area
	for (y = context->clip_region->top; y <= context->clip_region->bottom; y++) {

		// build a list of nodes
		nodes = 0;

		// search for x coordinates of polygon on the current row
		for (idx1 = 0; idx1 < count; idx1++) {

			// get the index of the next point and wrap around points count
			idx2 = (idx1 + 1) % count;

			// check if the polygon line exists on the same row
			if (((points[idx1].y < y) && (points[idx2].y >= y)) || ((points[idx1].y >= y) && (points[idx2].y < y))) {

				// make sure we don't overflow the node list
				if (count >= max_polygon_nodes) break;

				// calculate the intersection point x coordinate of the polygon edge
				node_list[nodes++] = (points[idx].y + (y - points[idx].y) * (points[idx2].x - points[idx].y) / (points[idx2].y - points[idx].y));

			}
		}

		// bubble sort the nodes
		idx = 0;

		while (idx + 1 < nodes) {

			if (nodes_list[idx] > nodes_list[idx+1]) {
				swap = nodes_list[idx];
				nodes_list[idx] = nodes_list[idx+1];
				nodes_list[idx+1] = swap;

				if (idx) {
					idx--;
				}
			}
			else {
				idx++;
			}

		}

		// fill the pixels between the node pairs
		for (idx = 0; idx < nodes; idx += 2) {



		}

	}

}

void gfx_draw_string(lcd_context_t *context, const char *str, int32_t length, int32_t x, int32_t y, uint32_t opaque) {


	const uint8_t *data;
	const uint8_t *glyphs;
	const uint16_t *offset;
	uint8_t first, last, absent;

	lcd_context_t ctx;

	// copy the current context into a local, modifiable variable
	ctx = *context;

	// check for font_ex_t
	if (context->font->format & font_ex) {

		// get the font as a font_ex_t
		font_ex_t_ptr font = (font_ex_t_ptr)(context->font);

		glyphs = font->data;
		offset = font->offset;
		first = font->first;
		last = font->last;

		// check for the default replacement character
		if ((font_absent_char >= first) && (font_absent_char <= last)) {
			// use the standard one
			absent = font_absent_char;
		}
		else {
			// use the first character instead
			absent = font->first;
		}

	}
	else {

		// normal font
		glyphs = context->font->data;
		offset = context->font->offset;
		first = 32;
		last = 126;
		absent = font_absent_char;

	}

	// loop through the characters in the string
	while (*str && length--) {

		// stop if the string has gone past the clipping region


	}

}
*/
void gfx_draw_image(void) {

}

// clears the display back to the background color
void gfx_clear(lcd_context_t *context) {

	context->display->fn_clear(context->background_color);

}

// sets the foreground color
void gfx_set_foreground_color(lcd_context_t *context, color_t color) {

	// set the foreground color
	context->foreground_color = context->display->fn_translate_color(color);
	//context->foreground_color = color;

}

/*
color_t gfx_get_foreground_color(lcd_context_t *context) {

	return context->foreground_color;

}
*/

// sets the background color
void gfx_set_background_color(lcd_context_t *context, color_t color) {

	// set the background color
	context->background_color = context->display->fn_translate_color(color);
	//context->background_color = color;
}

/*
color_t gfx_get_background_color(lcd_context_t *context) {

	return context->background_color;

}
*/

// sets the clip region
void gfx_set_clip_region(lcd_context_t *context, bounds_t_ptr bounds) {

	// update the clip region
	context->clip_region = *bounds;

}


void gfx_update_invalid_bounds(void) {

}

