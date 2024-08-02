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

void Task12()
{
	Settings settings;
	Scene scene0 = Scene::FromFile(SCENE_PATH + "\\Task12\\scene4.crtscene",settings);
	RayTracer renderer0(scene0);
	renderer0.Render(PATH + "\\PPM\\scene4.ppm");
	std::cout << "Scene1 rendered" << std::endl;
}

int main(){ Task12(); }

