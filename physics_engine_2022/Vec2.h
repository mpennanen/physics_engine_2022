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

	void rotate(Vec2 vec, double a)
	{
		x = cos(a) * vec.x - sin(a) * vec.y;
		y = sin(a) * vec.x + cos(a) * vec.y;
	}

	Vec2 normalized() {
		double length = sqrt(x * x + y * y);
		Vec2 _result(x, y);
		if (length > 0)
		{
			_result.x /= length;
			_result.y /= length;
		}
		return _result;
	}
	double length() const {
		return sqrt(x * x + y * y);
	}
};

double dot(const Vec2& v1, const Vec2& v2) {
	return v1.x * v2.x + v1.y * v2.y;
}