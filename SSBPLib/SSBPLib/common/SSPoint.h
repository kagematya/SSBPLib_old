#pragma once

#include <math.h>

namespace ss{
class SSSize;


/**
 * 座標クラス
 */
class SSPoint{
public:
	float x;
	float y;

public:
	SSPoint();
	SSPoint(float x, float y);
	SSPoint(const SSPoint& other);
	SSPoint(const SSSize& size);
	SSPoint& operator= (const SSPoint& other);
	SSPoint& operator= (const SSSize& size);
	SSPoint operator+(const SSPoint& right) const;
	SSPoint operator-(const SSPoint& right) const;
	SSPoint operator-() const;
	SSPoint operator*(float a) const;
	SSPoint operator/(float a) const;
	SSPoint& operator+= (const SSPoint& other);
	SSPoint& operator-= (const SSPoint& other);
	
	void setPoint(float x, float y);
	bool equals(const SSPoint& target) const;
	bool fuzzyEquals(const SSPoint& target, float variance) const;
	inline float getLength() const {
		return sqrtf(x*x + y*y);
	};
	inline float getLengthSq() const {
		return dot(*this); //x*x + y*y;
	};
	inline float getDistanceSq(const SSPoint& other) const {
		return (*this - other).getLengthSq();
	};
	inline float getDistance(const SSPoint& other) const {
		return (*this - other).getLength();
	};
	inline float getAngle() const {
		return atan2f(y, x);
	};
	float getAngle(const SSPoint& other) const;
	inline float dot(const SSPoint& other) const {
		return x*other.x + y*other.y;
	};
	inline float cross(const SSPoint& other) const {
		return x*other.y - y*other.x;
	};
	inline SSPoint getPerp() const {
		return SSPoint(-y, x);
	};
	inline SSPoint getRPerp() const {
		return SSPoint(y, -x);
	};
	inline SSPoint project(const SSPoint& other) const {
		return other * (dot(other) / other.dot(other));
	};
	//inline SSPoint rotate(const SSPoint& other) const {
	//	return SSPoint(x*other.x - y*other.y, x*other.y + y*other.x);
	//};
	//inline SSPoint unrotate(const SSPoint& other) const {
	//	return SSPoint(x*other.x + y*other.y, y*other.x - x*other.y);
	//};
	inline SSPoint normalize() const {
		float length = getLength();
		if (length == 0.) return SSPoint(1.f, 0);
		return *this / getLength();
	};
	inline SSPoint lerp(const SSPoint& other, float alpha) const {
		return *this * (1.f - alpha) + other * alpha;
	};
	//SSPoint rotateByAngle(const SSPoint& pivot, float angle) const;

	//static inline SSPoint forAngle(const float a){
	//	return SSPoint(cosf(a), sinf(a));
	//}

	// 回転させる
	void rotate(float angleRadian){
		float tx = x;
		x = tx * cos(angleRadian) - y * sin(angleRadian);
		y = tx * sin(angleRadian) + y * cos(angleRadian);
	}
	// 基準点を中心として回転させる
	void rotate(float angleRadian, const SSPoint &offset){
		*this -= offset;
		rotate(angleRadian);
		*this += offset;
	}
};



} //namespace ss
