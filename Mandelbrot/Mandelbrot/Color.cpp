#include "Color.h"



Color::Color(): value(0)
{
}

Color::Color(int r, int g, int b)
{
	value = ((r & 0xFF) << 16 |
			 (g & 0xFF) <<  8 |
			  b & 0xFF);
}


Color::~Color()
{
}

int Color::R()
{
	return value >> 16 & 0xFF;
}

int Color::G()
{
	return value >> 8 & 0xFF;
}

int Color::B()
{
	return value & 0xFF;
}

int Color::operator[](int i)
{
	return value >> (16 - (i << 3)) & 0xFF;
}

void Color::setRGB(int r, int g, int b)
{
	value = ((r & 0xFF) << 16 |
			 (g & 0xFF) <<  8 |
			  b & 0xFF);
}
