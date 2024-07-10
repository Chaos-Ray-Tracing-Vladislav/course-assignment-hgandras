#pragma once

class Color {
public:
	int r, g, b, a;

	Color() : r(0), g(0), b(0), a(1)
	{

	}

	Color(int r, int g, int  b) : r(r), g(g), b(b), a(1)
	{

	}

	Color(int r,int g,int b,int a) : r(r), g(g), b(b), a(a)
	{

	}
};
