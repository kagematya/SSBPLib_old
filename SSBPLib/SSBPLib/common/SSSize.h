#pragma once


namespace ss{
class SSPoint;


/**
 * サイズクラス
 */
class SSSize{
public:
	float width;
	float height;

public:
	SSSize();
	SSSize(float width, float height);
	SSSize(const SSSize& other);
	SSSize(const SSPoint& point);
	SSSize& operator= (const SSSize& other);
	SSSize& operator= (const SSPoint& point);
	SSSize operator+(const SSSize& right) const;
	SSSize operator-(const SSSize& right) const;
	SSSize operator*(float a) const;
	SSSize operator/(float a) const;
	void setSize(float width, float height);
	bool equals(const SSSize& target) const;
};


} //namespace ss
