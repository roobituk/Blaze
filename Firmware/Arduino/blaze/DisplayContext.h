#ifndef DISPLAY_CONTEXT_H
#define DISPLAY_CONTEXT_H

#include "Context.h"

/*!-----------------------------------------------------------------------------

*/
class DisplayContext : public Context {
	
public:
	DisplayContext(void);
	virtual ~DisplayContext(void);

	void begin(void);
	void clear(Colour color);
	void drawPixel(int16_t x, int16_t y, Colour color);
	void drawPixels(int16_t x, int16_t y, int32_t x0, int32_t count, int32_t bpp, const uint8_t *data, const uint8_t *palette);
	void drawPixelsEx(int16_t x, int16_t y, int32_t x0, int32_t count, int32_t bpp, const uint8_t *data, const uint8_t *palette, uint32_t transparent);
	void drawHorizontalLine(int16_t x, int16_t x2, int16_t y, Colour color);
	void drawVerticalLine(int16_t x, int16_t y, int16_t y2, Colour colour);
	void fillRectangle(int16_t x, int16_t y, int16_t width, int16_t height, Colour color);
	uint32_t translateColour(Colour color);
	void flush(void);
	
private:
	
	void setColumnAndPageAddress(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
	
	void writeCommand(uint8_t command);
	void writeByte(uint8_t value);
	void writeWord(uint16_t value);
	
};



#endif
