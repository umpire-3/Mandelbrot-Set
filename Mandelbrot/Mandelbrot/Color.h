#pragma once
class Color
{
	int value;
public:
	Color();
	Color(int Red, int Green, int Blue);
	~Color();
	int R();
	int G();
	int B();
	int operator[](int i);
	void setRGB(int Red, int Green, int Blue);
};

