#pragma once
#include "Geometry.h"
#include "Vector.h"
#include "Scene.h"

Vector3 barycentricCoordinates(const Vector3& intersectionPoint, Geometry::Triangle* triangle);

Vector3 barycentricNorm(const Vector3& baryCoords, Geometry::Intersection& isec,Scene& scene);

