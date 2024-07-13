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

	Scene scene0 = Scene::FromFile(SCENE_PATH + "\\Task9\\scene0.crtscene");
	RayTracer renderer0(scene0);
	renderer0.Render(PATH + "\\PPM\\scene0.ppm");
	std::cout << "Scene0 rendered" << std::endl;

	Scene scene1 = Scene::FromFile(SCENE_PATH + "\\Task9\\scene1.crtscene");
	RayTracer renderer1(scene1);
	renderer1.Render(PATH + "\\PPM\\scene1.ppm");
	std::cout << "Scene1 rendered" << std::endl;

	Scene scene2 = Scene::FromFile(SCENE_PATH + "\\Task9\\scene2.crtscene");
	RayTracer renderer2(scene2);
	renderer2.Render(PATH + "\\PPM\\scene2.ppm");
	std::cout << "Scene2 rendered" << std::endl;

	Scene scene3 = Scene::FromFile(SCENE_PATH + "\\Task9\\scene3.crtscene");
	RayTracer renderer3(scene3);
	renderer3.Render(PATH + "\\PPM\\scene3.ppm");
	std::cout << "Scene3 rendered" << std::endl;

	Scene scene4 = Scene::FromFile(SCENE_PATH + "\\Task9\\scene4.crtscene");
	RayTracer renderer4(scene4);
	renderer4.Render(PATH + "\\PPM\\scene4.ppm");
	std::cout << "Scene4 rendered" << std::endl;

	Scene scene5 = Scene::FromFile(SCENE_PATH + "\\Task9\\scene5.crtscene");
	RayTracer renderer5(scene5);
	renderer5.Render(PATH + "\\PPM\\scene5.ppm");
	std::cout << "Scene5 rendered" << std::endl;
}

int main()
{
	Task8();
}

