#include <iostream>
#include <chrono>
#include "Image.h"
#include "Vector.h"
#include "Camera.h"
#include "Geometry.h"
#include "Scene.h"
#include "Raytracer.h"

#define WIDTH 400
#define HEIGHT 400
#define FOV 60

const std::string PATH = "C:\\Gazsi\\Prog\\ChaosCamp\\RayTracing\\course-assignment-hgandras\\Images";
const std::string SCENE_PATH = "C:\\Gazsi\\Prog\\ChaosCamp\\RayTracing\\course-assignment-hgandras\\SourceCode\\Scenes\\";

void Task12()
{
	Settings settings;
	Scene scene0 = Scene::FromFile(SCENE_PATH + "\\Task13\\scene0.crtscene",settings);
	std::cout << "Scene read from file" << std::endl;
	RayTracer renderer0(scene0);

	const auto startBuckets{ std::chrono::high_resolution_clock::now() };
	renderer0.ParallelBucketsRender(PATH + "\\PPM\\sceneBuckets.ppm");
	const auto endBuckets{ std::chrono::high_resolution_clock::now() };
	long elapsed = std::chrono::duration_cast<std::chrono::seconds>(endBuckets - startBuckets).count();

	std::cout << "Scene0 rendered in parallel buckets in " << elapsed << " seconds" << std::endl;

	const auto startAABB{ std::chrono::high_resolution_clock::now() };
	renderer0.AABBRender(PATH + "\\PPM\\sceneAABB.ppm");
	const auto endAABB{ std::chrono::high_resolution_clock::now() };
	elapsed = std::chrono::duration_cast<std::chrono::seconds>(endAABB - startAABB).count();

	std::cout << "Scene0 rendered AABB in " << elapsed << " seconds" << std::endl;

	const auto startParallel{ std::chrono::high_resolution_clock::now() };
	renderer0.ParallelRegionsRender(PATH + "\\PPM\\sceneRegions.ppm");
	const auto endParallel{ std::chrono::high_resolution_clock::now() };
	elapsed = std::chrono::duration_cast<std::chrono::seconds>(endParallel - startParallel).count();

	std::cout << "Scene0 rendered in parallel regions in " << elapsed << " seconds" << std::endl;

	const auto start{std::chrono::high_resolution_clock::now()};
	renderer0.Render(PATH + "\\PPM\\sceneSequential.ppm");
	const auto end{ std::chrono::high_resolution_clock::now() };
	elapsed = std::chrono::duration_cast<std::chrono::seconds>(end - start).count();

	std::cout << "Scene0 rendered in sequential in " << elapsed << " seconds" << std::endl;
}



int main(){ Task12(); }

