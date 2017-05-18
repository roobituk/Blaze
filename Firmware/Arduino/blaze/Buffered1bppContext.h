
#ifndef BUFFERED_1BPP_CONTEXT_H
#define BUFFERED_1BPP_CONTEXT_H

#include "BufferedContext.h"

class Buffered1bppContext : public BufferedContext {

public:
	void begin(void);
	void clear(int32_t color);
	void drawPixel(int16_t x, int16_t y, int32_t color);
	void drawPixels(int16_t x, int16_t y, int32_t x0, int32_t count, int32_t bpp, const uint8_t *data, const uint8_t *palete);
	void drawHorizontalLine(int16_t x, int16_t x2, int16_t y, int32_t color);
	void drawVerticalLine(int16_t x, int16_t y, int16_t y2, int32_t colour);
	void fillRectangle(int16_t x, int16_t y, int16_t width, int16_t height, int32_t color);
	uint32_t translateColor(uint32_t color);
	void flush(void);
};

#endif
