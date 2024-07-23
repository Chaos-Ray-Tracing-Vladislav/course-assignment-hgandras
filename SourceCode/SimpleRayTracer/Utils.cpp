#include "Utils.h"

Vector3 barycentricCoordinates(const Vector3& intersectionPoint, Geometry::Triangle* triangle)
{
	float u = Geometry::Triangle::Area(triangle->v1, intersectionPoint, triangle->v3) / triangle->area;
	float v = Geometry::Triangle::Area(triangle->v2, intersectionPoint, triangle->v1) / triangle->area;

	return Vector3(u, v, 1 - u - v);
}

Vector3 barycentricNorm(const Vector3& baryCoords, Geometry::Intersection& isec, Scene& scene)
{
	Vector3 n1 = baryCoords.z * scene.sceneSettings.objects[isec.objectID].vertex_normals[isec.triangle->vertexIDs.x];
	Vector3 n2 = baryCoords.x * scene.sceneSettings.objects[isec.objectID].vertex_normals[isec.triangle->vertexIDs.y];
	Vector3 n3 = baryCoords.y * scene.sceneSettings.objects[isec.objectID].vertex_normals[isec.triangle->vertexIDs.z];

	return n1 + n2 + n3;
}