#ifndef CONTEXT_H
#define CONTEXT_H

#include "Common.h"

/*!-----------------------------------------------------------------------------

*/
class Context {

public:
	Context(void);
	virtual ~Context(void);
	
	int16_t height() const;
	int16_t width() const;

	void setWidth(const int16_t width);
	void setHeight(const int16_t height);

	virtual void begin(void) = 0;
	virtual void clear(Colour Colour) = 0;
	virtual void drawPixel(int16_t x, int16_t y, Colour Colour) = 0;
	virtual void drawPixels(int16_t x, int16_t y, int32_t x0, int32_t count, int32_t bpp, const uint8_t *data, const uint8_t *palette) = 0;
	virtual void drawPixelsEx(int16_t x, int16_t y, int32_t x0, int32_t count, int32_t bpp, const uint8_t *data, const uint8_t *palette, uint32_t transparent) = 0;
	virtual void drawHorizontalLine(int16_t x1, int16_t x2, int16_t y, Colour Colour) = 0;
	virtual void drawVerticalLine(int16_t x, int16_t y, int16_t y2, Colour colour) = 0;
	virtual void fillRectangle(int16_t x, int16_t y, int16_t width, int16_t height, Colour Colour) = 0;
	virtual uint32_t translateColour(Colour Colour) = 0;
	virtual void flush(void) = 0;

protected:

	bool m_done;
	
private:

	int16_t m_width;
	int16_t m_height;

};

#endif
