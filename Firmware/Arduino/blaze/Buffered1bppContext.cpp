
#include "Buffered1bppContext.h"

// -----------------------------------------------------------------------------
uint32_t Buffered1bppContext::translateColor(uint32_t color) {
	return 0;
	/*
	return (((color & 0x00FF0000) >> 16) * 19661) +
		    ((color & 0x0000FF00) >> 8) * 38666) + 
			(color & 0x000000FF) * 7209)) / (65536 * 128));*/
}

// -----------------------------------------------------------------------------
void Buffered1bppContext::drawPixel(int16_t x, int16_t y, int32_t color) {
	
	uint8_t *ptr;
	
	//ptr = (uint8_t *)m_buffer;
	
	int32_t bytesPerRow;
	
}
