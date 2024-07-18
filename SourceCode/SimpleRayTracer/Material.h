#pragma once
#include "Color.h"
#include "Texture.h"

enum MaterialType
{
	DIFFUSE,
	REFLECTIVE,
	REFRACTIVE,
	CONSTANT
};

struct Material {
	MaterialType type = MaterialType::DIFFUSE;
	Vector3 albedo = Vector3::zero();
	bool smooth_shading = true;
	float ior = -1;
	Texture* texturePtr = nullptr; 
};



