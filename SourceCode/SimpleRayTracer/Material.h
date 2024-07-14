#pragma once
#include "Color.h"

enum MaterialType
{
	DIFFUSE,
	REFLECTIVE,
	REFRACTIVE
};

struct Material {
	MaterialType type;
	bool smooth_shading;
};

struct Diffuse : Material
{
	Vector3 albedo;
};

struct Refractive : Material
{
	float ior;
};

struct Reflective {
	Vector3 albedo;
};

struct Texture : Material
{
	int textureID;
};

