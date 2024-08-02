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
	Scene scene0 = Scene::FromFile(SCENE_PATH + "\\Task15\\sceneFinal.crtscene",settings);
	Geometry::Translate(scene0.sceneSettings.objects[1],Vector3(-30,0,-2));
	Geometry::Translate(scene0.sceneSettings.objects[2], Vector3(-15, 0, -2));
	Geometry::Translate(scene0.sceneSettings.objects[3], Vector3(0, 0, -2));
	Geometry::Translate(scene0.sceneSettings.objects[4], Vector3(15, 0, -2));
	scene0.sceneSettings.active_lights = {1,0,0,0};
	std::cout << "Scene generated" << std::endl;
	RayTracer renderer0(scene0);

	int frame = 0;
	int maxFrames = 60;
	for (int i = 0; i < maxFrames; i++)
	{
		if(i == maxFrames/2)
			renderer0.scene.sceneSettings.active_lights = { 1,1,0,0 };
		renderer0.scene.camera.Truck(15.f/maxFrames);
		renderer0.ParallelBucketsRender(PATH + std::format("\\PPM\\FinalAnimation\\frame{}.ppm",frame), true);
		std::cout << std::format("Frame {} rendered", frame) << std::endl;
		frame++;
	}
	

	for (int i = 0; i < maxFrames; i++)
	{
		if (i == maxFrames / 2)
			renderer0.scene.sceneSettings.active_lights = { 1,1,1,0 };
		renderer0.scene.camera.Truck(15.f / maxFrames);
		renderer0.ParallelBucketsRender(PATH + std::format("\\PPM\\FinalAnimation\\frame{}.ppm", frame), true);
		std::cout << std::format("Frame {} rendered", frame) << std::endl;
		frame++;
	}

	for (int i = 0; i < maxFrames; i++)
	{
		if (i == maxFrames / 2)
			renderer0.scene.sceneSettings.active_lights = { 1,1,1,1 };
		renderer0.scene.camera.Truck(15.f / maxFrames);
		renderer0.ParallelBucketsRender(PATH + std::format("\\PPM\\FinalAnimation\\frame{}.ppm", frame), true);
		std::cout << std::format("Frame {} rendered", frame) << std::endl;
		frame++;
	}

	for (int i = 0; i < 20; i++)
	{
		renderer0.scene.camera.Pan(2.5);
		renderer0.scene.camera.Truck(-0.2);
		renderer0.ParallelBucketsRender(PATH + std::format("\\PPM\\FinalAnimation\\frame{}.ppm", frame), true);
		std::cout << std::format("Frame {} rendered", frame) << std::endl;
		frame++;
	}

	for (int i = 0; i < 20; i++)
	{
		renderer0.scene.camera.Dolly(0.2);
		renderer0.ParallelBucketsRender(PATH + std::format("\\PPM\\FinalAnimation\\frame{}.ppm", frame), true);
		std::cout << std::format("Frame {} rendered", frame) << std::endl;
		frame++;
	}
}



int main(){ Task14(); }

