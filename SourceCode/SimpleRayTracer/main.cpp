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

void Task11()
{

	Scene scene1 = Scene::FromFile(SCENE_PATH + "\\Task11\\scene1.crtscene");
	RayTracer renderer1(scene1);
	renderer1.Render(PATH + "\\PPM\\scene1.ppm");
	std::cout << "Scene1 rendered" << std::endl;

	Scene scene2 = Scene::FromFile(SCENE_PATH + "\\Task11\\scene2.crtscene");
	RayTracer renderer2(scene2);
	renderer2.Render(PATH + "\\PPM\\scene2.ppm");
	std::cout << "Scene2 rendered" << std::endl;

	Scene scene3 = Scene::FromFile(SCENE_PATH + "\\Task11\\scene3.crtscene");
	RayTracer renderer3(scene3);
	renderer3.Render(PATH + "\\PPM\\scene3.ppm");
	std::cout << "Scene3 rendered" << std::endl;

	Scene scene4 = Scene::FromFile(SCENE_PATH + "\\Task11\\scene4.crtscene");
	RayTracer renderer4(scene4);
	renderer4.Render(PATH + "\\PPM\\scene4.ppm");
	std::cout << "Scene4 rendered" << std::endl;

	Scene scene5 = Scene::FromFile(SCENE_PATH + "\\Task11\\scene5.crtscene");
	RayTracer renderer5(scene5);
	renderer5.Render(PATH + "\\PPM\\scene5.ppm");
	std::cout << "Scene5 rendered" << std::endl;

	Scene scene6 = Scene::FromFile(SCENE_PATH + "\\Task11\\scene6.crtscene");
	RayTracer renderer6(scene6);
	renderer6.Render(PATH + "\\PPM\\scene6.ppm");
	std::cout << "Scene6 rendered" << std::endl;

	Scene scene7 = Scene::FromFile(SCENE_PATH + "\\Task11\\scene7.crtscene");
	RayTracer renderer7(scene7);
	renderer7.Render(PATH + "\\PPM\\scene7.ppm");
	std::cout << "Scene7 rendered" << std::endl;

	Scene scene8 = Scene::FromFile(SCENE_PATH + "\\Task11\\scene8.crtscene");
	RayTracer renderer8(scene8);
	renderer8.Render(PATH + "\\PPM\\scene8.ppm");
	std::cout << "Scene8 rendered" << std::endl; 
}

int main()
{
	Task11();
}

