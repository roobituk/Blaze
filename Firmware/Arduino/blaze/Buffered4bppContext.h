#ifndef BUFFERED_4BPP_CONTEXT_H
#define BUFFERED_4BPP_CONTEXT_H

#include "BufferedContext.h"

/*!-----------------------------------------------------------------------------

*/
class Buffered4bppContext : public BufferedContext {

public:
	Buffered4bppContext(void);
	virtual ~Buffered4bppContext(void);

	void begin(void);
	void clear(Colour color);
	void drawPixel(int16_t x, int16_t y, Colour color);
	void drawPixels(int16_t x, int16_t y, int32_t x0, int32_t count, int32_t bpp, const uint8_t *data, const uint8_t *palete);
	void drawHorizontalLine(int16_t x, int16_t x2, int16_t y, Colour color);
	void drawVerticalLine(int16_t x, int16_t y, int16_t y2, Colour colour);
	void fillRectangle(int16_t x, int16_t y, int16_t width, int16_t height, Colour color);
	uint32_t translateColour(Colour color);
	void flush(void);

};

#endif
