#include "Vec2.h"
Vec2::Vec2() {
	x = 0;
	y = 0;
}
Vec2::Vec2(float a, float b) {
	x = a;
	y = b;
}
bool Vec2::operator==(const Vec2& obj) const {
	return x == obj.x && y == obj.y;
}
bool Vec2::operator!=(const Vec2& obj) const {
	return x != obj.x || y != obj.y;
}

Vec2 Vec2::operator+(const Vec2& obj) const {
	Vec2 result;
	result.x = x + obj.x;
	result.y = y + obj.y;
	return result;
}
Vec2 Vec2::operator-(const Vec2& obj) const {
	Vec2 result;
	result.x = x - obj.x;
	result.y = y - obj.y;
	return result;
}
Vec2 Vec2::operator*(float a) const {
	Vec2 result;
	result.x = x * a;
	result.y = y * a;
	return result;
}
Vec2 Vec2::operator/(float a) const {
	Vec2 result;
	result.x = x / a;
	result.y = y / a;
	return result;
}
Vec2& Vec2::operator+=(const Vec2& obj) {
	x += obj.x;
	y += obj.y;
	return *this;
}
Vec2& Vec2::operator-=(const Vec2& obj) {
	x -= obj.x;
	y -= obj.y;
	return *this;
}
Vec2& Vec2::operator*=(const float f) {
	x *= f;
	y *= f;
	return *this;
}
Vec2& Vec2::operator/=(const float f) {
	x /= f;
	y /= f;
	return *this;
}
float Vec2::distance(const Vec2& obj) const {
	return sqrt((obj.x - x) * (obj.x - x) + (obj.y - y) * (obj.y - y));
}
void Vec2::normalize() {
	float hysq = x * x + y * y;
	hysq = sqrt(hysq);
	x /= hysq;
	y /= hysq;
}
std::ostream& operator<<(std::ostream& COUT, const Vec2& obj) {
	COUT << "x: " << obj.x << ' ' << " y: " << obj.y << std::endl;
	return COUT;
}