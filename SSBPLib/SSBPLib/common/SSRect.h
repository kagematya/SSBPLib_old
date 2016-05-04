#pragma once

#include "SSPoint.h"
#include "SSSize.h"

namespace ss{


/**
 * 矩形クラス
 */
class SSRect{
public:
	SSPoint origin;
	SSSize  size;

public:
	SSRect();
	SSRect(float x, float y, float width, float height);
	SSRect(const SSRect& other);
	SSRect& operator= (const SSRect& other);
	void setRect(float x, float y, float width, float height);
	float getMinX() const; /// return the leftmost x-value of current rect
	float getMidX() const; /// return the midpoint x-value of current rect
	float getMaxX() const; /// return the rightmost x-value of current rect
	float getMinY() const; /// return the bottommost y-value of current rect
	float getMidY() const; /// return the midpoint y-value of current rect
	float getMaxY() const; /// return the topmost y-value of current rect
	bool equals(const SSRect& rect) const;
	bool containsPoint(const SSPoint& point) const;
	bool intersectsRect(const SSRect& rect) const;
};


} //namespace ss

