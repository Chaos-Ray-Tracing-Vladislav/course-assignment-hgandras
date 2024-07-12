#include <iostream>
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

void Task8()
{

	Scene scene0 = Scene::FromFile(SCENE_PATH + "\\Task8\\scene0.crtscene");
	Material mat0{ Vector3(0.5,0,0) };
	scene0.sceneSettings.objects[0].material = mat0;
	scene0.sceneSettings.objects[1].material = mat0;
	RayTracer renderer0(scene0);
	renderer0.Render(PATH + "\\PPM\\scene0.ppm");
	std::cout << "Scene0 rendered" << std::endl;

	Scene scene1 = Scene::FromFile(SCENE_PATH + "\\Task8\\scene1.crtscene");
	Material mat1{ Vector3(0.5,0,0) };
	scene1.sceneSettings.objects[0].material = mat1;
	scene1.sceneSettings.objects[1].material = mat1;
	RayTracer renderer1(scene1);
	renderer1.Render(PATH + "\\PPM\\scene1.ppm");
	std::cout << "Scene1 rendered" << std::endl;

	Scene scene2 = Scene::FromFile(SCENE_PATH + "\\Task8\\scene2.crtscene");
	Material mat2{ Vector3(0.5,0,0) };
	scene2.sceneSettings.objects[0].material = mat2;
	scene2.sceneSettings.objects[1].material = mat2;
	RayTracer renderer2(scene2);
	renderer2.Render(PATH + "\\PPM\\scene2.ppm");
	std::cout << "Scene2 rendered" << std::endl;

	Scene scene3 = Scene::FromFile(SCENE_PATH + "\\Task8\\scene3.crtscene");
	Material mat3{ Vector3(0.5,0,0) };
	scene3.sceneSettings.objects[0].material = mat3;
	scene3.sceneSettings.objects[1].material = mat3;
	RayTracer renderer3(scene3);
	renderer3.Render(PATH + "\\PPM\\scene3.ppm");
	std::cout << "Scene3 rendered" << std::endl;
}

int main()
{
	Task8();
}

