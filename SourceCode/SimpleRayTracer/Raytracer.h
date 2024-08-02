#pragma once
#include "Scene.h"
#include <thread>

class RayTracer
{
private:
	std::optional<Geometry::Intersection> IntersectRay(Ray& ray);

	Vector3 barycentricCoordinates(const Vector3& intersectionPoint, Geometry::Triangle* triangle);

	Vector3 barycentricNorm(const Vector3& baryCoords, Geometry::Intersection& isec);

	Vector3 traceRay(Ray& out, Vector3& finalColor, int bounces);

public:
	Scene scene;

	RayTracer(Scene& scene);

	void ParallelRegionsRender(std::string imgName);

	void ParallelBucketsRender(std::string imgName);

	void AABBRender(std::string imgName);

	void RenderRegion( int, int, int, int);

	void Render(std::string imgName);
};