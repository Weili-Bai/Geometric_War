#ifndef VEC2_H
#define VEC2_H
#include <cmath>
#include <iostream>
class Vec2
{
public:
	float x;
	float y;

	Vec2();
	Vec2(float, float);

	bool operator==(const Vec2&) const;
	bool operator!=(const Vec2&) const;

	Vec2 operator+(const Vec2&) const;
	Vec2 operator-(const Vec2&) const;
	Vec2 operator*(const float) const;
	Vec2 operator/(const float) const;

	Vec2& operator+=(const Vec2&);
	Vec2& operator-=(const Vec2&);
	Vec2& operator*=(const float);
	Vec2& operator/=(const float);

	float distance(const Vec2&) const;
	void normalize();
	friend std::ostream& operator << (std::ostream&, const Vec2 & obj);
};
#endif