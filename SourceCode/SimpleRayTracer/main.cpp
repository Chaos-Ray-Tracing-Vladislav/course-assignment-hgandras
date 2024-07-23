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

void Task14()
{
	Settings settings;
	Scene scene0 = Scene::FromFile(SCENE_PATH + "\\Task14\\scene1.crtscene",settings);
	std::cout << "Scene read from file" << std::endl;
	RayTracer renderer0(scene0);

	const auto startBuckets{ std::chrono::high_resolution_clock::now() };
	renderer0.ParallelBucketsRender(PATH + "\\PPM\\BucketActree1.ppm",true);
	const auto endBuckets{ std::chrono::high_resolution_clock::now() };
	long elapsed = std::chrono::duration_cast<std::chrono::seconds>(endBuckets - startBuckets).count();

	std::cout << "Scene1 rendered using ACTree and bucket rendering in " << elapsed << " seconds" << std::endl;

	const auto startactree{ std::chrono::high_resolution_clock::now() };
	renderer0.ACTreeRender(PATH + "\\PPM\\ACTree1.ppm");
	const auto endactree{ std::chrono::high_resolution_clock::now() };
	elapsed = std::chrono::duration_cast<std::chrono::seconds>(endactree - startactree).count();

	std::cout << "Scene1 rendered using ACTree in " << elapsed << " seconds" << std::endl;

	const auto start{std::chrono::high_resolution_clock::now()};
	renderer0.Render(PATH + "\\PPM\\Sequential.ppm");
	const auto end{ std::chrono::high_resolution_clock::now() };
	elapsed = std::chrono::duration_cast<std::chrono::seconds>(end - start).count();
	 
	std::cout << "Scene1 rendered in sequential in " << elapsed << " seconds" << std::endl;
}



int main(){ Task14(); }

