#pragma once
#include "Vector.h"

struct Ray
{
	Vector3 origin;
	Vector3 dir; //Always should be normalized

	Ray()
	{
		origin = Vector3::zero();
		dir = Vector3::zero();
	}

	Ray(Vector3 origin, Vector3 direction) : dir(direction.norm()), origin(origin)
	{

	}
};