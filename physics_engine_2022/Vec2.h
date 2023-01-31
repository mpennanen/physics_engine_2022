#pragma once
#include <math.h>

class Vec2
{
public:
	double x;
	double y;
	Vec2(double _x, double _y)
	{
		x = _x;
		y = _y;
	}

	Vec2 operator+(const Vec2& other) const {
		return Vec2(x + other.x, y + other.y);
	}

	Vec2& operator+=(const Vec2& other) {
		x += other.x;
		y += other.y;
		return *this;
	}

	Vec2 operator-(const Vec2& other) {
		return { x - other.x, y - other.y };
	}

	Vec2& operator-=(const Vec2& other) {

		x - other.x;
		y - other.y;
		return *this;
	}

	Vec2 operator*(const double& other) {
		return { x * other, y * other };
	}

	Vec2 normalized() {
		double length = sqrt(x * x + y * y);
		return Vec2(x / length, y / length);
	}
	double length() const {
		return sqrt(x * x + y * y);
	}
};

double dot(const Vec2& v1, const Vec2& v2) {
	return v1.x * v2.x + v1.y * v2.y;
}