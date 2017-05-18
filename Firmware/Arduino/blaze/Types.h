#ifndef TYPES_H
#define TYPES_H

// -----------------------------------------------------------------------------
class Point {

public:
	Point(void);
	Point(int16_t x, int16_t y);
	virtual ~Point(void);

	int16_t x() const;
	int16_t y() const;

	void setX(int16_t x);
	void setY(int16_t y);

	bool isNull() const;

private:
	int16_t m_x;
	int16_t m_y;

};

// -----------------------------------------------------------------------------
class Rectangle {

public:
	Rectangle(void);
	Rectangle(int16_t x, int16_t y, int16_t width, int16_t height);
	virtual ~Rectangle(void);

	int16_t x() const;
	int16_t y() const;
	int16_t width() const;
	int16_t height() const;

	int16_t left() const;
	int16_t right() const;
	Point centre() const;
	int16_t top() const;
	Point topLeft() const;
	Point topRight() const;
	int16_t bottom() const;
	Point bottomLeft() const;
	Point bottomRight() const;

	void setX(int16_t x);
	void setY(int16_t y);
	void setWidth(int16_t width);
	void setHeight(int16_t height);

	bool contains(Point pt) const;

private:
	int16_t m_x;
	int16_t m_y;
	int16_t m_width;
	int16_t m_height;
	
};

// -----------------------------------------------------------------------------
class Colour {

public:
	Colour(void);
	Colour(int32_t colour);
	virtual ~Colour();
	
	int32_t value() const;

	uint16_t to16bit() const;

	static Colour fromArgb(uint8_t a, uint8_t r, uint8_t g, uint8_t b);

private:
	int32_t m_value;

};

class Image {


};

#endif