#pragma once
#include "Scene.h"
#include "AccelerationTree.h"
#include "Geometry.h"
#include <thread>

class RayTracer
{
private:
	std::optional<Geometry::Intersection> IntersectRay(Ray&);

	Vector3 traceRay(Ray&, Vector3&, int,bool);

public:
	Scene scene;

	AccelerationTree ACTree;

	RayTracer(Scene&);

	void ParallelRegionsRender(std::string,bool);

	void ParallelBucketsRender(std::string,bool);

	void AABBRender(std::string);

	void RenderRegion( int, int, int, int,bool);

	void Render(std::string);

	void ACTreeRender(std::string);
};