#pragma once
#include "Color.h"

enum MaterialType
{
	DIFFUSE,
	REFLECTIVE,
	REFRACTIVE,
	CONSTANT
};

struct Material {
	MaterialType type;
	Vector3 albedo;
	bool smooth_shading;
	float ior;
};



