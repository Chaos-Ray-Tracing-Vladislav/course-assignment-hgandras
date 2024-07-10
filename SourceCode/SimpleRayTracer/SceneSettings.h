#pragma once
#include <vector>
#include "Color.h"

struct Settings {
	Color bgCol;
	int width;
	int height;

	std::vector<float> matrix;
	std::vector<float> position;

	std::vector<float> vertices;
	std::vector<int> triangles;
};



