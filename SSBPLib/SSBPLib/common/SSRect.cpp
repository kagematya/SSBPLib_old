#include "SSRect.h"
#include "SSUtil.h"


namespace ss{


SSRect::SSRect(void)
{
	setRect(0.0f, 0.0f, 0.0f, 0.0f);
}

SSRect::SSRect(float x, float y, float width, float height)
{
	setRect(x, y, width, height);
}

SSRect::SSRect(const SSRect& other)
{
	setRect(other.origin.x, other.origin.y, other.size.width, other.size.height);
}

SSRect& SSRect::operator= (const SSRect& other)
{
	setRect(other.origin.x, other.origin.y, other.size.width, other.size.height);
	return *this;
}

void SSRect::setRect(float x, float y, float width, float height)
{
	// CGRect can support width<0 or height<0
	SS_ASSERT2(width >= 0.0f && height >= 0.0f, "width and height of Rect must not less than 0.");

	origin.x = x;
	origin.y = y;

	size.width = width;
	size.height = height;
}

bool SSRect::equals(const SSRect& rect) const
{
	return (origin.equals(rect.origin) &&
		size.equals(rect.size));
}

float SSRect::getMaxX() const
{
	return (float)(origin.x + size.width);
}

float SSRect::getMidX() const
{
	return (float)(origin.x + size.width / 2.0);
}

float SSRect::getMinX() const
{
	return origin.x;
}

float SSRect::getMaxY() const
{
	return origin.y + size.height;
}

float SSRect::getMidY() const
{
	return (float)(origin.y + size.height / 2.0);
}

float SSRect::getMinY() const
{
	return origin.y;
}

bool SSRect::containsPoint(const SSPoint& point) const
{
	bool bRet = false;

	if (point.x >= getMinX() && point.x <= getMaxX()
		&& point.y >= getMinY() && point.y <= getMaxY())
	{
		bRet = true;
	}

	return bRet;
}

bool SSRect::intersectsRect(const SSRect& rect) const
{
	return !(getMaxX() < rect.getMinX() ||
		rect.getMaxX() <      getMinX() ||
		getMaxY() < rect.getMinY() ||
		rect.getMaxY() <      getMinY());
}


} //namespace ss
