#pragma once
#include "Color.h"
#include <string>
#include <vector>
#include <stdexcept>
#include <fstream>
#include <format>
#include "assert.h"

class Image {
public:
	int w, h;
	std::vector<Color> image;

	Image();

	Image(int w, int h);

	void setPixel(int x, int y, Color value);

	Color getPixel(int x, int y);

	void clear();

	void writePPM(std::string name);
};