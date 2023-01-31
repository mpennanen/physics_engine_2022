#pragma once


class vec2
{
public:
	float x;
	float y;

	vec2(float _x, float _y)
	{
		x = _x;
		y = _y;
	};

	vec2()
	{
		x = -9000;
		y = -9000;
	};

};

class vec3
{
public:
	float x;
	float y;
	float z;
	vec3(float _x, float _y, float _z)
	{
		x = _x;
		y = _y;
		z = _z;
	};
	vec3()
	{
		x = -9000;
		y = -9000;
		z = -9000;
	};

};