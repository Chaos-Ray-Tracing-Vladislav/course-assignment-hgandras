#pragma once
#include "Color.h"

enum MaterialType
{
	DIFFUSE
};

struct Material {
	MaterialType type;
	Vector3 albedo;
	bool smooth_shading;
};

