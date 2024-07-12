#pragma once
#include <vector>
#include "Color.h"
#include "Geometry.h"
#include "Light.h"

struct Settings {
	Color bgCol;
	int width;
	int height;

	std::vector<float> matrix;
	std::vector<float> position;

	std::vector<Geometry::Object> objects;

	std::vector<Light> lights;
	float k_ambient;
	Vector3 ambientColor;

	const float EPSILON = 1e-5;
};



