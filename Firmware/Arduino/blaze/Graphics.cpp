
#include "Graphics.h"

// -----------------------------------------------------------------------------
Graphics::Graphics(void) {
	m_pContext = NULL;
	// Initialize the pen and brush colours
	m_pen = Colours::White();
	m_brush = Colours::Black();
	
	m_clipRegion.setX(0);
	m_clipRegion.setY(0);
	m_clipRegion.setWidth(240);
	m_clipRegion.setHeight(240);
}

// -----------------------------------------------------------------------------
Graphics::~Graphics(void) {
	delete m_pContext;
}

// -----------------------------------------------------------------------------
void Graphics::begin(Context *context) {
	// Set the initial context
	swapContext(context);
}

// -----------------------------------------------------------------------------
void Graphics::clear(void) {
	m_pContext->clear(m_brush);
}

// -----------------------------------------------------------------------------
void Graphics::setPen(Colour pen) {
	m_pen = pen;
}

// -----------------------------------------------------------------------------
void Graphics::setBrush(Colour brush) {
	m_brush = brush;
}

// -----------------------------------------------------------------------------
void Graphics::drawArc(Point centre, int32_t radius, int32_t start, int32_t sweep) {
	#ifdef ENABLED
	// Loop through the arc sweep
	for (int i = 0; i < sweep; i++) {
		float rads = ((start + i) * DEG_TO_RAD);

		float opp = sin(rads) * (float)radius;
		float adj = cos(rads) * (float)radius;

		int32_t x = (centre.x() + (int32_t)opp);
		int32_t y = (centre.y() - (int32_t)adj);

		// Draw the pixel at the specified location
		m_pContext->drawPixel(x, y, m_pen);
	}
	#endif
}

// -----------------------------------------------------------------------------
void Graphics::drawBezier(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2) {

	int sx = x2-x1, sy = y2-y1;
	long xx = x0-x1, yy = y0-y1, xy;         /* relative values for checks */
	double dx, dy, err, cur = xx*sy-yy*sx;                    /* curvature */

	if (sx*(long)sx+sy*(long)sy > xx*xx+yy*yy) { /* begin with longer part */
		x2 = x0; x0 = sx+x1; y2 = y0; y0 = sy+y1; cur = -cur;  /* swap P0 P2 */
	}

	if (cur != 0) {                                    /* no straight line */
		xx += sx; xx *= sx = x0 < x2 ? 1 : -1;           /* x step direction */
		yy += sy; yy *= sy = y0 < y2 ? 1 : -1;           /* y step direction */
		xy = 2*xx*yy; xx *= xx; yy *= yy;          /* differences 2nd degree */

		if (cur*sx*sy < 0) {                           /* negated curvature? */
			xx = -xx; yy = -yy; xy = -xy; cur = -cur;
		}

		dx = 4.0*sy*cur*(x1-x0)+xx-xy;             /* differences 1st degree */
		dy = 4.0*sx*cur*(y0-y1)+yy-xy;
		xx += xx; yy += yy; err = dx+dy+xy;                /* error 1st step */

		do {
			/* plot curve */
			m_pContext->drawPixel(x0, y0, m_pen);

			if (x0 == x2 && y0 == y2)
				return;  /* last pixel -> curve finished */

			y1 = 2*err < dx;                  /* save value for test of y step */

			if (2*err > dy) {
				x0 += sx;
				dx -= xy;
				err += dy += yy;
			} /* x step */

			if (y1) {
				y0 += sy;
				dy -= xy;
				err += dx += xx;
			} /* y step */

		} while (dy < dx );           /* gradient negates -> algorithm fails */
	}

	drawLine(x0, y0, x2, y2);
}

// -----------------------------------------------------------------------------
void Graphics::drawCircle(int32_t x, int32_t y, int32_t radius) {
	int32_t lA, lB, lD, lX1, lY1;

	// Initialize the variables that control the Bresenham circle drawing algorithm.
	lA = 0;
	lB = radius;
	lD = 3 - (2 * radius);

	// Loop until the A delta is greater than the B delta, meaning that the entire circle has been drawn.
	while(lA <= lB) {
		// Determine the row when subtracting the A delta.
		lY1 = y - lA;

		// See if this row is within the clipping region.
		if ((lY1 >= m_clipRegion.top()) && (lY1 <= m_clipRegion.bottom())) {
			// Determine the column when subtracting the B delta.
			lX1 = x - lB;

			// If this column is within the clipping region, then draw a pixel
			// at that position.
			if((lX1 >= m_clipRegion.left()) && (lX1 <= m_clipRegion.right())) {
				m_pContext->drawPixel(lX1, lY1, m_pen);
			}

			// Determine the column when adding the B delta.
			lX1 = x + lB;

			// If this column is within the clipping region, then draw a pixel
			// at that position.
			if((lX1 >= m_clipRegion.left()) && (lX1 <= m_clipRegion.right())) {
				m_pContext->drawPixel(lX1, lY1, m_pen);
			}
		}

		// Determine the row when adding the A delta.
		lY1 = y + lA;

		// See if this row is within the clipping region, and the A delta is
		// not zero (otherwise, it will be the same row as when the A delta was
		// subtracted).
		if ((lY1 >= m_clipRegion.top()) && (lY1 <= m_clipRegion.bottom()) && (lA != 0)) {
			// Determine the column when subtracting the B delta.
			lX1 = x - lB;

			// If this column is within the clipping region, then draw a pixel at that position.
			if((lX1 >= m_clipRegion.left()) && (lX1 <= m_clipRegion.right())) {
				m_pContext->drawPixel(lX1, lY1, m_pen);
			}

			// Determine the column when adding the B delta.
			lX1 = x + lB;

			// If this column is within the clipping region, then draw a pixel at that position.
			if((lX1 >= m_clipRegion.left()) && (lX1 <= m_clipRegion.right())) {
				m_pContext->drawPixel(lX1, lY1, m_pen);
			}
		}

		// Only draw the complementary pixels if the A and B deltas are
		// different (otherwise, they describe the same set of pixels).
		if(lA != lB) {
			// Determine the row when subtracting the B delta.
			lY1 = y - lB;

			// See if this row is within the clipping region.
			if((lY1 >= m_clipRegion.top()) && (lY1 <= m_clipRegion.bottom())) {
				// Determine the column when subtracting the a delta.
				lX1 = x - lA;

				// If this column is within the clipping region, then draw a
				// pixel at that position.
				if((lX1 >= m_clipRegion.left()) && (lX1 <= m_clipRegion.right())) {
					m_pContext->drawPixel(lX1, lY1, m_pen);
				}

				// Only draw the mirrored pixel if the A delta is non-zero
				// (otherwise, it will be the same pixel).
				if(lA != 0) {
					// Determine the column when adding the A delta.
					lX1 = x + lA;

					// If this column is within the clipping region, then draw
					// a pixel at that position.
					if((lX1 >= m_clipRegion.left()) && (lX1 <= m_clipRegion.right())) {
						m_pContext->drawPixel(lX1, lY1, m_pen);
					}
				}
			}

			// Determine the row when adding the B delta.
			lY1 = y + lB;

			// See if this row is within the clipping region.
			if((lY1 >= m_clipRegion.top()) && (lY1 <= m_clipRegion.bottom())) {
				// Determine the column when subtracting the A delta.
				lX1 = x - lA;

				// If this column is within the clipping region, then draw a
				// pixel at that position.
				if((lX1 >= m_clipRegion.left()) && (lX1 <= m_clipRegion.right())) {
					m_pContext->drawPixel(lX1, lY1, m_pen);
				}

				// Only draw the mirrored pixel if the A delta is non-zero
				// (otherwise, it will be the same pixel).
				if(lA != 0) {
					// Determine the column when adding the A delta.
					lX1 = x + lA;

					// If this column is within the clipping region, then draw
					// a pixel at that position.
					if((lX1 >= m_clipRegion.left()) && (lX1 <= m_clipRegion.right())) {
						m_pContext->drawPixel(lX1, lY1, m_pen);
					}
				}
			}
		}

		// See if the error term is negative.
		if(lD < 0) {
			// Since the error term is negative, adjust it based on a move in
			// only the A delta.
			lD += (4 * lA) + 6;
		}
		else {
			// Since the error term is non-negative, adjust it based on a move
			// in both the A and B deltas.
			lD += (4 * (lA - lB)) + 10;
			// Decrement the B delta.
			lB -= 1;
		}

		// Increment the A delta.
		lA++;
	}
}


/*!-----------------------------------------------------------------------------

*/
void Graphics::drawEllipse(int16_t x0, int16_t y0, int16_t x1, int16_t y1) {
	#ifdef ENABLED
	int a = abs(x1-x0), b = abs(y1-y0), b1 = b&1; /* values of diameter */
	long dx = 4*(1-a)*b*b, dy = 4*(b1+1)*a*a; /* error increment */
	long err = dx+dy+b1*a*a, e2; /* error of 1.step */

	if (x0 > x1) {
		x0 = x1;
		x1 += a;
	} /* if called with swapped points */

	if (y0 > y1)
		y0 = y1; /* .. exchange them */

	y0 += (b+1)/2;
	y1 = y0-b1;   /* starting pixel */

	a *= 8*a;
	b1 = 8*b*b;

	do {
		m_pContext->drawPixel(x1, y0, m_pen);
		m_pContext->drawPixel(x0, y0, m_pen);
		m_pContext->drawPixel(x0, y1, m_pen);
		m_pContext->drawPixel(x1, y1, m_pen);

		e2 = 2*err;
		if (e2 <= dy) {
			y0++;
			y1--;
			err += dy += a;
		}  /* y step */

		if (e2 >= dx || 2*err > dy) {
			x0++;
			x1--;
			err += dx += b1;
		} /* x step */

	} while (x0 <= x1);

	/* too early stop of flat ellipses a=1 */
	while (y0-y1 < b) {
		m_pContext->drawPixel(x0-1, y0, m_pen);
		m_pContext->drawPixel(x1+1, y0++, m_pen);
		m_pContext->drawPixel(x0-1, y1, m_pen);
		m_pContext->drawPixel(x1+1, y1--, m_pen);
	}
	#endif
}

// -----------------------------------------------------------------------------
void Graphics::drawEllipse(Rectangle rect) {
	drawEllipse(rect.left(), rect.top(), rect.right(), rect.bottom());
}

// -----------------------------------------------------------------------------
void Graphics::drawHorizontalLine(int16_t x1, int16_t x2, int16_t y) {
	int32_t temp;
	
	// Check the Y clip region
	if ((y < m_clipRegion.top()) || (y > m_clipRegion.bottom()))
		return;
	
	// Swap x coordinates if necessary
	if (x1 > x2) {
		temp = x1;
		x1 = x2;
		x2 = temp;
	}
	
	// Check the X clip region
	if ((x1 < m_clipRegion.left()) || (x2 > m_clipRegion.right()))
		return;
	
	// Clip the coordinates to the clip region
	if (x1 < m_clipRegion.left())
		x1 = m_clipRegion.left();
	
	if (x2 > m_clipRegion.right())
		x2 = m_clipRegion.right();
	
	// Draw the line
	m_pContext->drawHorizontalLine(x1, x2, y, m_pen);
}

// -----------------------------------------------------------------------------
void Graphics::drawImage(int16_t x, int16_t y, Image img) {
	drawImage(x, y, img, Colours::White(), false);
}

// -----------------------------------------------------------------------------
void Graphics::drawImage(int16_t x, int16_t y, Image img, Colour transparent) {
	drawImage(x, y, img, transparent, true);
}

// -----------------------------------------------------------------------------
void Graphics::drawImage(int16_t x, int16_t y, Image img, Colour transparent, bool useTransparency) {
	#ifdef ENABLED
	uint32_t ulByte, ulBits, ulMatch, ulSize, ulIdx, ulCount, ulNum;
	int32_t lBPP, lWidth, lHeight, lX0, lX1, lX2, lXMask;
	const uint8_t *pucPalette;
	uint32_t pulBWPalette[2];

	// Get the image format from the image data.
	lBPP = *pucImage++;

	// Get the image width from the image data.
	lWidth = *(unsigned short *)pucImage;
	pucImage += 2;

	// Get the image height from the image data.
	lHeight = *(unsigned short *)pucImage;
	pucImage += 2;

	// Return without doing anything if the entire image lies outside the
	// current clipping region.
	if((x > pContext->sClipRegion.sXMax) || ((x + lWidth - 1) < pContext->sClipRegion.sXMin) || (y > pContext->sClipRegion.sYMax) || ((y + lHeight - 1) < pContext->sClipRegion.sYMin)) {
		return;
	}

	// Get the starting X offset within the image based on the current clipping
	// region.
	if(x < pContext->sClipRegion.sXMin) {
		lX0 = pContext->sClipRegion.sXMin - x;
	}
	else {
		lX0 = 0;
	}

	// Get the ending X offset within the image based on the current clipping
	// region.
	if((x + lWidth - 1) > pContext->sClipRegion.sXMax) {
		lX2 = pContext->sClipRegion.sXMax - x;
	}
	else {
		lX2 = lWidth - 1;
	}

	// Reduce the height of the image, if required, based on the current
	// clipping region.
	if ((y + lHeight - 1) > pContext->sClipRegion.sYMax) {
		lHeight = pContext->sClipRegion.sYMax - y + 1;
	}

	// Determine the color palette for the image based on the image format.
	if ((lBPP & 0x7f) == IMAGE_FMT_1BPP_UNCOMP) {
		//
		// Construct a local "black & white" palette based on the foreground
		// and background colors of the drawing context.
		pulBWPalette[0] = m_brush.value();
		pulBWPalette[1] = m_pen.value();

		// Set the palette pointer to the local "black & white" palette.
		pucPalette = (uint8_t *)pulBWPalette;
	}
	else {
		// For 4 and 8 BPP images, the palette is contained at the start of the
		// image data.
		pucPalette = pucImage + 1;
		pucImage += (pucImage[0] * 3) + 4;
	}

	// See if the image is compressed.
	if (!(lBPP & 0x80)) {
		// The image is not compressed.  See if the top portion of the image
		// lies above the clipping region.
		if(y < pContext->sClipRegion.sYMin) {
			// Determine the number of rows that lie above the clipping region.
			lX1 = pContext->sClipRegion.sYMin - y;

			// Skip past the data for the rows that lie above the clipping
			// region.
			pucImage += (((lWidth * lBPP) + 7) / 8) * lX1;
			// Decrement the image height by the number of skipped rows.
			lHeight -= lX1;
			// Increment the starting Y coordinate by the number of skipped rows.
			y += lX1;
		}

		// Determine the starting offset for the first source pixel within the byte.
		switch(lBPP) {
			case 1:
			{
				lXMask = lX0 & 7;
				break;
			}
			case 4:
			{
				lXMask = lX0 & 1;
				break;
			}
			default:
			{
				lXMask = 0;
				break;
			}
		}

		// Loop while there are more rows to draw.
		while(lHeight--)
		{
			// Draw this row of image pixels.
			if (useTransparency) {
				m_pContext->drawPixelsEx(x + lX0, y, lXMask, lX2 - lX0 + 1, lBPP, pucImage + ((lX0 * lBPP) / 8), pucPalette, transparent);
			}
			else {
				m_pContext->drawPixels(x + lX0, y, lXMask, lX2 - lX0 + 1, lBPP, pucImage + ((lX0 * lBPP) / 8), pucPalette);
			}

			// Skip past the data for this row.
			pucImage += ((lWidth * lBPP) + 7) / 8;
			// Increment the Y coordinate.
			y++;
		}
	}
	else {
		// The image is compressed.  Clear the compressed flag in the format
		// specifier so that the bits per pixel remains.
		lBPP &= 0x7f;

		// Reset the dictionary used to uncompress the image.
		for (ulBits = 0; ulBits < sizeof(g_pucDictionary); ulBits += 4) {
			*(uint32_t *)(g_pucDictionary + ulBits) = 0;
		}

		// Determine the number of bytes of data to decompress.
		ulCount = (((lWidth * lBPP) + 7) / 8) * lHeight;

		// Initialize the pointer into the dictionary.
		ulIdx = 0;

		// Start off with no encoding byte.
		ulBits = 0;
		ulByte = 0;

		// Start from the upper left corner of the image.
		lX1 = 0;

		// Loop while there are more rows or more data in the image.
		while(lHeight && ulCount) {
			// See if an encoding byte needs to be read.
			if(ulBits == 0) {
				// Read the encoding byte, which indicates if each of the
				// following eight bytes are encoded or literal.
				ulByte = *pucImage++;
				ulBits = 8;
			}

			// See if the next byte is encoded or literal.
			if(ulByte & (1 << (ulBits - 1))) {
				// This byte is encoded, so extract the location and size of
				// the encoded data within the dictionary.
				ulMatch = *pucImage >> 3;
				ulSize = (*pucImage++ & 7) + 2;

				// Decrement the count of bytes to decode by the number of
				// copied bytes.
				ulCount -= ulSize;
			}
			else {
				// This byte is a literal, so copy it into the dictionary.
				g_pucDictionary[ulIdx++] = *pucImage++;

				// Decrement the count of bytes to decode.
				ulCount--;

				// Clear any previous encoded data information.
				ulMatch = 0;
				ulSize = 0;
			}

			// Loop while there are bytes to copy for the encoded data, or
			// once for literal data.
			while(ulSize || !(ulByte & (1 << (ulBits - 1)))) {
				// Set the encoded data bit for this data so that this loop
				// will only be executed once for literal data.
				ulByte |= 1 << (ulBits - 1);

				// Loop while there is more encoded data to copy and there is
				// additional space in the dictionary (before the buffer
				// wraps).
				while (ulSize && (ulIdx != sizeof(g_pucDictionary))) {
					// Copy this byte.
					g_pucDictionary[ulIdx] = g_pucDictionary[(ulIdx + ulMatch) % sizeof(g_pucDictionary)];

					// Increment the dictionary pointer.
					ulIdx++;
					// Decrement the encoded data size.
					ulSize--;
				}

				// See if the dictionary pointer is about to wrap, or if there
				// is no more data to decompress.
				if ((ulIdx == sizeof(g_pucDictionary)) || !ulCount) {
					// Loop through the data in the dictionary buffer.
					for(ulIdx = 0; (ulIdx < sizeof(g_pucDictionary)) && lHeight; ) {
						//
						// Compute the number of pixels that remain in the
						// dictionary buffer.
						//
						ulNum = ((sizeof(g_pucDictionary) - ulIdx) * 8) / lBPP;

						// See if any of the pixels in the dictionary buffer
						// are within the clipping region.
						if ((lY >= pContext->sClipRegion.sYMin) && ((lX1 + ulNum) >= lX0) && (lX1 <= lX2)) {
							// Skip some pixels at the start of the scan line
							// if required to stay within the clipping region.
							if(lX1 < lX0) {
								ulIdx += ((lX0 - lX1) * lBPP) / 8;
								lX1 = lX0;
							}

							// Shorten the scan line if required to stay within
							// the clipping region.
							if(ulNum > (lX2 - lX1 + 1)) {
								ulNum = lX2 - lX1 + 1;
							}

							// Determine the starting offset for the first
							// source pixel within the byte.
							switch(lBPP) {
								case 1:
								{
									lXMask = lX1 & 7;
									break;
								}
								case 4:
								{
									lXMask = lX1 & 1;
									break;
								}
								default:
								{
									lXMask = 0;
									break;
								}
							}

							// Draw this row of image pixels.
							if (useTransparency) {
								m_pContext->drawPixelsEx(x + lX1, y, lXMask, ulNum, lBPP, g_pucDictionary + ulIdx, pucPalette, transparent);
							}
							else {
								m_pContext->drawPixels(x + lX1, y, lXMask, ulNum, lBPP, g_pucDictionary + ulIdx, pucPalette);
							}
						}

						// Move the X coordinate back to the start of the first
						// data byte in this portion of the dictionary buffer.
						lX1 = ((lX1 * lBPP) & ~7) / lBPP;

						// See if the remainder of this scan line resides
						// within the dictionary buffer.
						if(((((lWidth - lX1) * lBPP) + 7) / 8) > (sizeof(g_pucDictionary) - ulIdx))	{
							// There is more to this scan line than is in the
							// dictionary buffer at this point, so move the
							// X coordinate by by the number of pixels in the
							// dictionary buffer.
							lX1 += (((sizeof(g_pucDictionary) - ulIdx) * 8) / lBPP);
							// The entire dictionary buffer has been scanned.
							ulIdx = sizeof(g_pucDictionary);
						}
						else {
							// The remainder of this scan line resides in the
							// dictionary buffer, so skip past it.
							ulIdx += (((lWidth - lX1) * lBPP) + 7) / 8;

							// Move to the start of the next scan line.
							lX1 = 0;
							lY++;

							// There is one less scan line to process.
							lHeight--;
						}
					}

					// Start over from the beginning of the dictionary buffer.
					ulIdx = 0;
				}
			}

			// Advance to the next bit in the encoding byte.
			ulBits--;
		}
	}
	#endif
}

// -----------------------------------------------------------------------------
void Graphics::drawLine(int16_t x1, int16_t y1, int16_t x2, int16_t y2) {
	// If the X coordinates are the same, draw a vertical line
	if (x1 == x2) {
		drawVerticalLine(x1, y1, y2);
		return;
	}
	
	// If the Y coordinates are the same, draw a horizontal line
	if (y1 == y2) {
		drawHorizontalLine(x1, x2, y1);
		return;
	}
	
	bool steep = false;
	
	// Check for a steep line
	if (((y2 > y1) ? (y2 - y1) : (y1 - y2)) > ((x2 > x1) ? (x2 - x1) : (x1 - x2))) {
		steep = true;
	}
	
	int16_t temp;

	// Swap the X and Y coordinates for a steep line
	if (steep) {
		temp = x1;
		x1 = y1;
		y1 = temp;
		temp = x2;
		x2 = y2;
		y2 = temp;
	}
	
	if (x1 > x2) {
		temp = x1;
		x1 = x2;
		x2 = temp;
		temp = y1;
		y1 = y2;
		y2 = temp;
	}
	
	// Work out the delta X and Y
	int32_t deltaX = x2 - x1;
	int32_t deltaY = (y2 > y1) ? (y2 - y1) : (y1 - y2);
	
	// Initialize the error term
	int32_t error = -deltaX / 2;
	
	int32_t stepY = -1;
	
	if (y1 < y2) {
		stepY = 1;
	}
	
	// Loop through the points in the line
	for (; x1 <= x2; x1++) {
		if (steep) {
			m_pContext->drawPixel(y1, x1, m_pen);
		}
		else {
			m_pContext->drawPixel(x1, y1, m_pen);
		}
		
		error += deltaY;
		
		if (error > 0) {
			y1 += stepY;
			error -= deltaX;
		}
	}
}

// -----------------------------------------------------------------------------
void Graphics::drawLine(Point p1, Point p2) {
	drawLine(p1.x(), p1.y(), p2.x(), p2.y());
}

// -----------------------------------------------------------------------------
void Graphics::drawPolygon(Point *pPoints, int32_t count, bool closed)
{
	unsigned long ulIdx;

	// Draw the lines between points
	for(int16_t x = 0; x < (count - 1); x++) {
		drawLine(pPoints[x].x(), pPoints[x].y(), pPoints[x + 1].x(), pPoints[x + 1].y());
	}

	// Draw a closing line between first and last point?
	if(closed) {
		drawLine(pPoints[0].x(), pPoints[0].y(), pPoints[count - 1].x(), pPoints[count - 1].y());
	}
}

// -----------------------------------------------------------------------------
void Graphics::drawRectangle(int16_t x, int16_t y, int16_t width, int16_t height) {
	// Line across the top
	drawHorizontalLine(x, x + width, y);
	
	// Line down the right
	drawVerticalLine(x + width, y, y + height);
	
	// Line across the bottom
	drawHorizontalLine(x, x + width, y + height);
	
	// Line down the left
	drawVerticalLine(x, y, y + height);
}

// -----------------------------------------------------------------------------
void Graphics::drawRectangle(Rectangle rect) {
	drawRectangle(rect.left(), rect.top(), rect.width(), rect.height());
}

// -----------------------------------------------------------------------------
void Graphics::drawString(const char *text, int16_t x, int16_t y) {

}


// -----------------------------------------------------------------------------
void Graphics::drawVerticalLine(int16_t x, int16_t y1, int16_t y2) {
	int32_t temp;

	// Check the Y clip region
	if ((y1 < m_clipRegion.top()) || (y2 > m_clipRegion.bottom()))
		return;

	// Swap x coordinates if necessary
	if (y1 > y2) {
		temp = y1;
		y1 = y2;
		y2 = temp;
	}

	// Check the X clip region
	if ((x < m_clipRegion.left()) || (x > m_clipRegion.right()))
		return;

	// Clip the coordinates to the clip region
	if (y1 < m_clipRegion.top())
		y1 = m_clipRegion.top();

	if (y2 > m_clipRegion.bottom())
		y2 = m_clipRegion.bottom();

	// Draw the line
	m_pContext->drawVerticalLine(x, y1, y2, m_pen);
}

void Graphics::fillCircle(int32_t x, int32_t y, int32_t radius) {
    int32_t lA, lB, lD, lX1, lX2, lY1;

    // Initialize the variables that control the Bresenham circle drawing
    // algorithm.
    lA = 0;
    lB = radius;
    lD = 3 - (2 * radius);

    // Loop until the A delta is greater than the B delta, meaning that the
    // entire circle has been filled.
    while(lA <= lB) {
        // Determine the row when subtracting the A delta.
        lY1 = y - lA;

        // See if this row is within the clipping region.
        if((lY1 >= m_clipRegion.top()) && (lY1 <= m_clipRegion.bottom())) {
            // Determine the column when subtracting the B delta, and move it
            // to the left edge of the clipping region if it is to the left of
            // the clipping region.
            lX1 = x - lB;
            if(lX1 < m_clipRegion.left()) {
                lX1 = m_clipRegion.left();
            }

            // Determine the column when adding the B delta, and move it to the
            // right edge of the clipping region if it is to the right of the
            // clipping region.
            lX2 = x + lB;
            if(lX2 > m_clipRegion.right()) {
                lX2 = m_clipRegion.right();
            }

            // Draw a horizontal line if this portion of the circle is within
            // the clipping region.
            if(lX1 <= lX2) {
            	m_pContext->drawHorizontalLine(lX1, lX2, lY1, m_pen);
            }
        }

        // Determine the row when adding the A delta.
        lY1 = y + lA;

        // See if this row is within the clipping region, and the A delta is
        // not zero (otherwise, this describes the same row of the circle).
        if((lY1 >= m_clipRegion.top()) && (lY1 <= m_clipRegion.bottom()) && (lA != 0)) {
            // Determine the column when subtracting the B delta, and move it
            // to the left edge of the clipping region if it is to the left of
            // the clipping region.
            lX1 = x - lB;
            if(lX1 < m_clipRegion.left()) {
                lX1 = m_clipRegion.left();
            }

            // Determine the column when adding the B delta, and move it to the
            // right edge of the clipping region if it is to the right of the
            // clipping region.
            lX2 = x + lB;
            if(lX2 > m_clipRegion.right()) {
                lX2 = m_clipRegion.right();
            }

            // Draw a horizontal line if this portion of the circle is within
            // the clipping region.
            if(lX1 <= lX2) {
            	m_pContext->drawHorizontalLine(lX1, lX2, lY1, m_pen);
            }
        }

        // Only draw the complementary lines if the B delta is about to change
        // and the A and B delta are different (otherwise, they describe the
        // same set of pixels).
        if((lD >= 0) && (lA != lB)) {
            // Determine the row when subtracting the B delta.
            lY1 = y - lB;

            // See if this row is within the clipping region.
            if((lY1 >= m_clipRegion.top()) && (lY1 <= m_clipRegion.bottom())) {
                // Determine the column when subtracting the A delta, and move
                // it to the left edge of the clipping regino if it is to the
                // left of the clipping region.
                lX1 = x - lA;

                if(lX1 < m_clipRegion.left()) {
                    lX1 = m_clipRegion.left();
                }

                // Determine the column when adding the A delta, and move it to
                // the right edge of the clipping region if it is to the right
                // of the clipping region.
                lX2 = x + lA;

                if(lX2 > m_clipRegion.right()) {
                    lX2 = m_clipRegion.right();
                }

                // Draw a horizontal line if this portion of the circle is
                // within the clipping region.
                if(lX1 <= lX2) {
                	m_pContext->drawHorizontalLine(lX1, lX2, lY1, m_pen);
                }
            }

            // Determine the row when adding the B delta.
            lY1 = y + lB;

            // See if this row is within the clipping region.
            if((lY1 >= m_clipRegion.top()) && (lY1 <= m_clipRegion.bottom())) {
                // Determine the column when subtracting the A delta, and move
                // it to the left edge of the clipping region if it is to the
                // left of the clipping region.
                lX1 = x - lA;

                if(lX1 < m_clipRegion.left()) {
                    lX1 = m_clipRegion.left();
                }

                // Determine the column when adding the A delta, and move it to
                // the right edge of the clipping region if it is to the right
                // of the clipping region.
                lX2 = x + lA;

                if(lX2 > m_clipRegion.right()) {
                    lX2 = m_clipRegion.right();
                }

                // Draw a horizontal line if this portion of the circle is
                // within the clipping region.
                if(lX1 <= lX2) {
                	m_pContext->drawHorizontalLine(lX1, lX2, lY1, m_pen);
                }
            }
        }

        // See if the error term is negative.
        if(lD < 0) {
            //
            // Since the error term is negative, adjust it based on a move in
            // only the A delta.
            //
            lD += (4 * lA) + 6;
        }
        else {
            // Since the error term is non-negative, adjust it based on a move
            // in both the A and B deltas.
            lD += (4 * (lA - lB)) + 10;
            // Decrement the B delta.
            lB -= 1;
        }

        // Increment the A delta.
        lA++;
    }
}

// -----------------------------------------------------------------------------
void Graphics::fillEllipse(int16_t x0, int16_t y0, int16_t x1, int16_t y1) {
	#ifdef ENABLED
	int a = abs(x1-x0), b = abs(y1-y0), b1 = b&1; /* values of diameter */
	long dx = 4*(1-a)*b*b, dy = 4*(b1+1)*a*a; /* error increment */
	long err = dx+dy+b1*a*a, e2; /* error of 1.step */

	if (x0 > x1) {
		x0 = x1;
		x1 += a;
	} /* if called with swapped points */

	if (y0 > y1)
		y0 = y1; /* .. exchange them */

	y0 += (b+1)/2;
	y1 = y0-b1;   /* starting pixel */
	a *= 8*a;
	b1 = 8*b*b;

	do {
		m_pContext->drawHorizontalLine(x0, x1, y0, m_brush);
		m_pContext->drawHorizontalLine(x0, x1, y1, m_brush);

	   e2 = 2*err;
	   if (e2 <= dy) {
		   y0++;
		   y1--;
		   err += dy += a;
	   }  /* y step */

	   if (e2 >= dx || 2*err > dy) {
		   x0++;
		   x1--;
		   err += dx += b1;
	   } /* x step */
	} while (x0 <= x1);
	#endif
}

// -----------------------------------------------------------------------------
void Graphics::fillEllipse(Rectangle rect) {
	fillEllipse(rect.left(), rect.top(), rect.right(), rect.bottom());
}

// -----------------------------------------------------------------------------
void Graphics::fillPolygon(Point *points, int32_t count)
{
	unsigned long ulIdx, ulIdx2;
	unsigned long ulNumNodes;
	short sY;
	short pNodeList[100];
	short sSwap;

	// Loop through the rows of the clipping area.
	for (sY = m_clipRegion.top(); sY <= m_clipRegion.bottom(); sY++)
	{
		// Build a list of nodes.
  		ulNumNodes = 0;

		// Search for X coordinates of polygons on the current row.
		for(ulIdx = 0; ulIdx < count; ulIdx++) {
			// Get the index of next point and wrap around points count.
			ulIdx2 = (ulIdx + 1) % count;

			// Check if the polygon line exists on the same row.
			if(((points[ulIdx].y() <  sY) && (points[ulIdx2].y() >= sY)) || ((points[ulIdx].y() >= sY) && (points[ulIdx2].y() <  sY))) {
				// Make sure nodes list isn't overflowed.
				if(ulNumNodes >= 100)
					break;

				// Calculate the intersection point X coordinate
				// of the polygon edge.
				pNodeList[ulNumNodes++] =
					(points[ulIdx].x() + (sY - points[ulIdx].y()) *
					(points[ulIdx2].x() - points[ulIdx].x()) /
					(points[ulIdx2].y() - points[ulIdx].y()));
			}
		}

		// Sort the nodes, via a simple “Bubble” sort.
		ulIdx = 0;

		while(ulIdx + 1 < ulNumNodes) {
			if(pNodeList[ulIdx] > pNodeList[ulIdx + 1]) {
				sSwap = pNodeList[ulIdx];
				pNodeList[ulIdx] = pNodeList[ulIdx + 1];
				pNodeList[ulIdx + 1] = sSwap;

				if(ulIdx) {
					ulIdx--;
				}
			}
			else {
				ulIdx++;
			}
		}

		// Fill the pixels between node pairs.
		for(ulIdx = 0; ulIdx < ulNumNodes; ulIdx += 2) {
			// Break when lines go out of clipping region.
			if(pNodeList[ulIdx] > m_clipRegion.right()) break;

			// Skip when line ends before clipping region.
			if(pNodeList[ulIdx + 1] < m_clipRegion.left()) continue;

			// Clip the line from left.
			if(pNodeList[ulIdx] < m_clipRegion.left()) {
				pNodeList[ulIdx] = m_clipRegion.left();
			}

			// Clip the line from right.
			if(pNodeList[ulIdx + 1] > m_clipRegion.right()) {
				pNodeList[ulIdx + 1] = m_clipRegion.right();
			}

			// Call the low level horizontal line drawing routine.
			m_pContext->drawHorizontalLine(pNodeList[ulIdx], pNodeList[ulIdx + 1], sY, m_brush);
		}
	}
}

// -----------------------------------------------------------------------------
void Graphics::fillRectangle(Rectangle rect) {
	// Swap coordinates if necessary
	m_pContext->fillRectangle(rect.left(), rect.top(), rect.right(), rect.bottom(), m_brush);
}

// -----------------------------------------------------------------------------
void Graphics::swapContext(Context *context) {
	m_pContext = context;
	// Check if we need to initialize the context
	//m_pContext->begin();
}
