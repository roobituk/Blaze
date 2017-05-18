
#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "Common.h"
#include "Colours.h"
#include "Context.h"

/*!-----------------------------------------------------------------------------

*/
class Graphics {

public:
	Graphics(void);
	virtual ~Graphics(void);

	Colour pen() const { return m_pen; }
	Colour brush() const { return m_brush; }
	//Font font() const { return m_font; }
	
	Rectangle clipRegion() const { return m_clipRegion; }

	void begin(Context *context);
	
	void clear(void);

	void setPen(Colour pen);
	void setBrush(Colour brush);
	//void setFont(Font &font);

	void drawLine(int16_t x1, int16_t y1, int16_t x2, int16_t y2);
	void drawLine(Point p1, Point p2);
	void drawCircle(int32_t x, int32_t y, int32_t radius);
	void drawCircle(Point centre, int32_t radius);
	void drawPolygon(Point *points, int32_t count, bool closed = true);
	void drawImage(int16_t x, int16_t y, Image img);
	void drawImage(int16_t x, int16_t y, Image img, Colour transparent);
	void drawArc(Point centre, int32_t radius, int32_t start, int32_t sweep);
	void drawBezier(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2);
	void drawEllipse(int16_t x0, int16_t y0, int16_t x1, int16_t y1);
	void drawEllipse(Rectangle rect);
	void drawString(const char *text, int16_t x, int16_t y);
	void drawRectangle(int16_t x, int16_t y, int16_t width, int16_t height);
	void drawRectangle(Rectangle rect);
	void fillRectangle(Rectangle rect);
	void fillCircle(int32_t x, int32_t y, int32_t radius);
	void fillEllipse(int16_t x0, int16_t y0, int16_t x1, int16_t y1);
	void fillEllipse(Rectangle rect);
	void fillPolygon(Point *points, int32_t count);
	
	void swapContext(Context *context);
	
private:
	
	Context *m_pContext;
	
	Colour m_pen;
	Colour m_brush;
	
	Rectangle m_clipRegion;

	void drawHorizontalLine(int16_t x1, int16_t x2, int16_t y);
	void drawVerticalLine(int16_t x, int16_t y1, int16_t y2);

	void drawImage(int16_t x, int16_t y, Image img, Colour transparent, bool useTransparency);

};

#endif
