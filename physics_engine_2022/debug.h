#pragma once
#include <iostream>

void debugDouble(std::string name, double value)
{
	std::cout << name << ": " << value << std::endl;
}

void debugVec2(std::string name, Vec2 value)
{
	std::cout << name << ": " << value.x << ", " << value.y << std::endl;
}

void debugString(std::string str)
{
	std::cout << str << std::endl;
}