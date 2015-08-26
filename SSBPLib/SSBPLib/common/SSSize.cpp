#include "SSSize.h"
#include <math.h>
#include <float.h>
#include "SSUtil.h"
#include "SSPoint.h"

namespace ss{

	
SSSize::SSSize(void) : width(0), height(0)
{
}

SSSize::SSSize(float width, float height) : width(width), height(height)
{
}

SSSize::SSSize(const SSSize& other) : width(other.width), height(other.height)
{
}

SSSize::SSSize(const SSPoint& point) : width(point.x), height(point.y)
{
}

SSSize& SSSize::operator= (const SSSize& other)
{
	setSize(other.width, other.height);
	return *this;
}

SSSize& SSSize::operator= (const SSPoint& point)
{
	setSize(point.x, point.y);
	return *this;
}

SSSize SSSize::operator+(const SSSize& right) const
{
	return SSSize(this->width + right.width, this->height + right.height);
}

SSSize SSSize::operator-(const SSSize& right) const
{
	return SSSize(this->width - right.width, this->height - right.height);
}

SSSize SSSize::operator*(float a) const
{
	return SSSize(this->width * a, this->height * a);
}

SSSize SSSize::operator/(float a) const
{
	SS_ASSERT2(a, "SSSize division by 0.");
	return SSSize(this->width / a, this->height / a);
}

void SSSize::setSize(float width, float height)
{
	this->width = width;
	this->height = height;
}

bool SSSize::equals(const SSSize& target) const
{
	return (fabs(this->width - target.width)  < FLT_EPSILON)
		&& (fabs(this->height - target.height) < FLT_EPSILON);
}



} //namespace ss
