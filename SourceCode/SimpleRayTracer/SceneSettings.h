#pragma once
#include <vector>
#include "Color.h"
#include "Geometry.h"
#include "Light.h"
#include "Texture.h"

struct Settings {

	//General
	Vector3 bgCol;
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

	//Textures
	std::vector<ColorTexture> cTextures;
	std::vector<EdgeTexture> edgeTextures;
	std::vector<CheckerTexture> checkerTextures;
	std::vector<BitmapTexture> bmapTextures;

	//Other
	const float EPSILON = 1e-5;
	const int MAX_BOUNCE = 5;
};



