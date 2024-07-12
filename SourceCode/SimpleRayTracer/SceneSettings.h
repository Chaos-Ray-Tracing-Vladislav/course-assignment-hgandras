#pragma once
#include <vector>
#include "Color.h"
#include "Geometry.h"
#include "Light.h"

struct Settings {
	//General
	Color bgCol;
	int width;
	int height;

	//Camera
	std::vector<float> matrix;
	std::vector<float> position;

	//Objects
	std::vector<Geometry::Object> objects;

	//Lights
	std::vector<Light> lights;
	float k_ambient;
	Vector3 ambientColor;

	//Materials
	std::vector<Material> materials;

	//Other
	const float EPSILON = 1e-5;
};



