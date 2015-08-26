#include "SSPoint.h"
#include <float.h>
#include "SSUtil.h"
#include "SSSize.h"


namespace ss{


SSPoint::SSPoint(void) : x(0), y(0)
{
}

SSPoint::SSPoint(float x, float y) : x(x), y(y)
{
}

SSPoint::SSPoint(const SSPoint& other) : x(other.x), y(other.y)
{
}

SSPoint::SSPoint(const SSSize& size) : x(size.width), y(size.height)
{
}

SSPoint& SSPoint::operator= (const SSPoint& other)
{
	setPoint(other.x, other.y);
	return *this;
}

SSPoint& SSPoint::operator= (const SSSize& size)
{
	setPoint(size.width, size.height);
	return *this;
}

SSPoint SSPoint::operator+(const SSPoint& right) const
{
	return SSPoint(this->x + right.x, this->y + right.y);
}

SSPoint SSPoint::operator-(const SSPoint& right) const
{
	return SSPoint(this->x - right.x, this->y - right.y);
}

SSPoint SSPoint::operator-() const
{
	return SSPoint(-x, -y);
}

SSPoint SSPoint::operator*(float a) const
{
	return SSPoint(this->x * a, this->y * a);
}

SSPoint SSPoint::operator/(float a) const
{
	SS_ASSERT2(a, "SSPoint division by 0.");
	return SSPoint(this->x / a, this->y / a);
}

SSPoint& SSPoint::operator +=(const SSPoint &other){
	x += other.x;
	y += other.y;
	return *this;
}
SSPoint& SSPoint::operator -=(const SSPoint &other){
	x -= other.x;
	y -= other.y;
	return *this;
}

void SSPoint::setPoint(float x, float y)
{
	this->x = x;
	this->y = y;
}

bool SSPoint::equals(const SSPoint& target) const
{
	return (fabs(this->x - target.x) < FLT_EPSILON)
		&& (fabs(this->y - target.y) < FLT_EPSILON);
}

bool SSPoint::fuzzyEquals(const SSPoint& b, float var) const
{
	if (x - var <= b.x && b.x <= x + var)
	if (y - var <= b.y && b.y <= y + var)
		return true;
	return false;
}

float SSPoint::getAngle(const SSPoint& other) const
{
	SSPoint a2 = normalize();
	SSPoint b2 = other.normalize();
	float angle = atan2f(a2.cross(b2), a2.dot(b2));
	if (fabs(angle) < FLT_EPSILON) return 0.f;
	return angle;
}

//SSPoint SSPoint::rotateByAngle(const SSPoint& pivot, float angle) const
//{
//	return pivot + (*this - pivot).rotate(SSPoint::forAngle(angle));
//}



} //namespace ss
