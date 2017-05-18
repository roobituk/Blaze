
#include "Buffered4bppContext.h"

// -----------------------------------------------------------------------------
Buffered4bppContext::Buffered4bppContext(void) {

}

// -----------------------------------------------------------------------------
Buffered4bppContext::~Buffered4bppContext(void) {
	if (m_pBuffer) {
		free(m_pBuffer);
	}
}

// -----------------------------------------------------------------------------
void Buffered4bppContext::begin(void) {
	// Allocate the memory for the off-screen buffer
	m_pBuffer = (uint8_t *)malloc((240 * 240) / 2);

}

// -----------------------------------------------------------------------------
void Buffered4bppContext::clear(Colour color) {

}

// -----------------------------------------------------------------------------
void Buffered4bppContext::drawPixel(int16_t x, int16_t y, Colour color) {

}

// -----------------------------------------------------------------------------
void Buffered4bppContext::drawPixels(int16_t x, int16_t y, int32_t x0, int32_t count, int32_t bpp, const uint8_t *data, const uint8_t *palete) {

}

// -----------------------------------------------------------------------------
void Buffered4bppContext::drawHorizontalLine(int16_t x, int16_t x2, int16_t y, Colour color) {

}

// -----------------------------------------------------------------------------
void Buffered4bppContext::drawVerticalLine(int16_t x, int16_t y, int16_t y2, Colour colour) {

}

// -----------------------------------------------------------------------------
void Buffered4bppContext::fillRectangle(int16_t x, int16_t y, int16_t width, int16_t height, Colour color) {

}

// -----------------------------------------------------------------------------
uint32_t Buffered4bppContext::translateColour(Colour color) {

}

// -----------------------------------------------------------------------------
void Buffered4bppContext::flush(void) {

}

