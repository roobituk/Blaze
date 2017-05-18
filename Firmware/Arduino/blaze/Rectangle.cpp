#include "Common.h"

// -----------------------------------------------------------------------------
Rectangle::Rectangle(void) {

}

// -----------------------------------------------------------------------------
Rectangle::Rectangle(int16_t x, int16_t y, int16_t width, int16_t height) {
	m_x = x;
	m_y = y;
	m_width = width;
	m_height = height;
}

// -----------------------------------------------------------------------------
Rectangle::~Rectangle(void) {

}

// -----------------------------------------------------------------------------
int16_t Rectangle::x() const {
	return m_x;
}

// -----------------------------------------------------------------------------
int16_t Rectangle::y() const {
	return m_x;
}

// -----------------------------------------------------------------------------
int16_t Rectangle::width() const {
	return m_x;
}

// -----------------------------------------------------------------------------
int16_t Rectangle::height() const {
	return m_x;
}

// -----------------------------------------------------------------------------
int16_t Rectangle::left() const {
	return m_x;
}

// -----------------------------------------------------------------------------
int16_t Rectangle::right() const {
	return (m_x + m_width);
}

// -----------------------------------------------------------------------------
Point Rectangle::centre() const {
	return Point(m_x + (m_width / 2), m_y + (m_height / 2));
}

// -----------------------------------------------------------------------------
int16_t Rectangle::top() const {
	return m_y;
}

// -----------------------------------------------------------------------------
Point Rectangle::topLeft() const {
	return Point(m_x, m_y);
}

// -----------------------------------------------------------------------------
Point Rectangle::topRight() const {
	return Point(m_x + m_width, m_y);
}

// -----------------------------------------------------------------------------
int16_t Rectangle::bottom() const {
	return (m_y + m_height);
}

// -----------------------------------------------------------------------------
Point Rectangle::bottomLeft() const {
	return Point(m_x, m_y + m_height);
}

// -----------------------------------------------------------------------------
Point Rectangle::bottomRight() const {
	return Point(m_x + m_width, m_y + m_height);
}

// -----------------------------------------------------------------------------
void Rectangle::setX(int16_t x) {
	m_x = x;
}

// -----------------------------------------------------------------------------
void Rectangle::setY(int16_t y) {
	m_y = y;
}

// -----------------------------------------------------------------------------
void Rectangle::setWidth(int16_t width) {
	m_width = width;
}

// -----------------------------------------------------------------------------
void Rectangle::setHeight(int16_t height) {
	m_height = height;
}

// -----------------------------------------------------------------------------
bool Rectangle::contains(Point pt) const {
	if ((pt.x() < left() || pt.x() > right()) && (pt.y() < top() || pt.y() > bottom()))
		return false;

	return true;
}

