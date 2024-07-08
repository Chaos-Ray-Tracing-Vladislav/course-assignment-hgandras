#include <iostream>
#include "Image.h"
#include "Vector.h"
#include "Camera.h"
#include "Geometry.h"
#include "Scene.h"

#define WIDTH 800
#define HEIGHT 800
#define FOV 60

void RandomImage()
{
	int n_small_x = 4;
	int n_small_y = 4;

	int stepX = WIDTH / n_small_x;
	int stepY = HEIGHT / n_small_y;
	Image image(WIDTH,HEIGHT);
	for (int small_i = 0; small_i < n_small_x*n_small_y; small_i++)
	{
		int small_x = small_i % n_small_x;
		int small_y = small_i / n_small_x;
		for (int i = 0; i < stepX*stepY; i++)
		{
			int x =small_x * stepX + i % stepX;
			int y =small_y * stepY + i / stepX;
			
			int r = small_i == 0 ? 0 : 255 / small_i;
			int g = small_i * 16;
			int b = small_i * small_i * 4;
			image.setPixel(x, y, Color(r, g, b));
		}
	}
	image.writePPM("Squares.ppm");
}

void CircleImage()
{

	float radius = 100;

	Image image(WIDTH,HEIGHT);
	Vector2 center(WIDTH / 2, HEIGHT / 2);
	Color circleCol(255, 0, 0);
	Color bg(128, 128, 128);

	for (int i = 0; i < WIDTH*HEIGHT; i++)
	{
		int x = i % WIDTH;
		int y = i / WIDTH;
		
		Vector2 pos(x, y);
		if ((pos - center).length() < radius)
			image.setPixel(x, y, circleCol);
		else
			image.setPixel(x, y, bg);
		
	}
	image.writePPM("Circle.ppm");
}

void rayImage()
{
	Image image(WIDTH,HEIGHT);
	Camera cam(image,Frame(),FOV);
	
	for (int i = 0; i < WIDTH*HEIGHT; i++)
	{
		int x = i % WIDTH;
		int y = i / WIDTH;
		Ray ray = cam.CastRay(x, y);
		Color col(abs(ray.dir.x*255), abs(ray.dir.y*255), abs(ray.dir.z*255));
		image.setPixel(x, y, col);
	}
	image.writePPM("Rays.ppm");
}

void Triangles()
{
	//Cross products
	std::cout << Cross(Vector3(3.5, 0, 0), Vector3(1.75, 3.5, 0)) << std::endl;
	std::cout << Cross(Vector3(3.0, -3, 1), Vector3(4, 9, 3)) << std::endl;

	//Parallelogram areas
	std::cout << Cross(Vector3(3,-3,1), Vector3(4,9,3)).length() << std::endl;
	std::cout << Cross(Vector3(3,-3,1), Vector3(-12,12,-4)).length() << std::endl;

	//Triangle normals
	std::cout << Geometry::Triangle(Vector3(-1.75, -1.75, -3), Vector3(1.75, -1.75, -3), Vector3(0, 1.75, -3)).normal() << std::endl;
	std::cout << Geometry::Triangle(Vector3(0,0,-1), Vector3(1,0,1), Vector3(-1,0,1)).normal() << std::endl;
	std::cout << Geometry::Triangle(Vector3(0.56,1.11,1.23), Vector3(0.44,-2.368,-0.54), Vector3(-1.56,0.15,-1.92)).normal() << std::endl;

	//Triangle area
	std::cout << Geometry::Triangle(Vector3(-1.75, -1.75, -3), Vector3(1.75, -1.75, -3), Vector3(0, 1.75, -3)).area() << std::endl;
	std::cout << Geometry::Triangle(Vector3(0, 0, -1), Vector3(1, 0, 1), Vector3(-1, 0, 1)).area() << std::endl;
	std::cout << Geometry::Triangle(Vector3(0.56, 1.11, 1.23), Vector3(0.44, -2.368, -0.54), Vector3(-1.56, 0.15, -1.92)).area() << std::endl;

}

void renderTriangle()
{
	Image image(WIDTH, HEIGHT);
	Camera cam(image, Frame(), FOV);
	Geometry::Triangle triangle(Vector3(-1.75, -1.75, -3), Vector3(1.75, -1.75, -3), Vector3(0, 1.75, -3));
	std::vector<Geometry::Triangle>geometry{ triangle };
	Scene scene(cam,geometry);
	scene.Render("Triangle.ppm");
}

void renderTriangles()
{
	Image image(WIDTH, HEIGHT);
	Camera cam(image, Frame(), FOV);

	Geometry::Triangle triangle1(Vector3(-1.75, -1.75, -3), Vector3(1.75, -1.75, -3), Vector3(0, 1.75, -3));
	Geometry::Triangle triangle2(Vector3(-1, -1, -2), Vector3(1, -1, -2), Vector3(0, 1, -2),Material::ColorMaterial(Color(0,255,0)));

	std::vector<Geometry::Triangle>geometry{ triangle1,triangle2 };
	Scene scene(cam, geometry);
	scene.Render("Triangles.ppm");
}

void renderShape()
{
	Image image(WIDTH, HEIGHT);
	Camera cam(image, Frame(Matrix3::Identity(),Vector3(0,0,-1)), FOV);

	Material::ColorMaterial m1(Color(255, 0, 0));
	Material::ColorMaterial m2(Color(0, 255, 0));
	Material::ColorMaterial m3(Color(0, 0,255));

	
	Geometry::Triangle t11(Vector3(0,0,-3), Vector3(0.2,-1,-3), Vector3(-0.2, -1, -3), m1);
	Geometry::Triangle t12(Vector3(0, 0, -2.9), Vector3(0.15, -0.9, -2.9), Vector3(-0.15, -0.9, -2.9), m2);
	Geometry::Triangle t13(Vector3(0, 0, -2.8), Vector3(0.1, -0.8, -2.8), Vector3(-0.1, -0.8, -2.8), m3);

	Geometry::Triangle t21(Vector3(0, 0, -3), Vector3(0.2, 1, -3), Vector3(-0.2, 1, -3), m1);
	Geometry::Triangle t22(Vector3(0, 0, -2.9), Vector3(0.15, 0.9, -2.9), Vector3(-0.15, 0.9, -2.9), m2);
	Geometry::Triangle t23(Vector3(0, 0, -2.8), Vector3(0.1, 0.8, -2.8), Vector3(-0.1, 0.8, -2.8), m3);

	Geometry::Triangle t31(Vector3(0, 0, -3), Vector3(-1,0.2, -3), Vector3(-1,-0.2, -3), m1);
	Geometry::Triangle t32(Vector3(0, 0, -2.9), Vector3(-0.9, 0.15, -2.9), Vector3( -0.9, -0.15, -2.9), m2);
	Geometry::Triangle t33(Vector3(0, 0, -2.8), Vector3(-0.8, 0.1, -2.8), Vector3( -0.8, -0.1, -2.8), m3);

	Geometry::Triangle t41(Vector3(0, 0, -3), Vector3(1, 0.2, -3), Vector3(1, -0.2, -3), m1);
	Geometry::Triangle t42(Vector3(0, 0, -2.9), Vector3(0.9, 0.15, -2.9), Vector3(0.9, -0.15, -2.9), m2);
	Geometry::Triangle t43(Vector3(0, 0, -2.8), Vector3(0.8, 0.1, -2.8), Vector3(0.8, -0.1, -2.8), m3);

	std::vector<Geometry::Triangle>geometry{ t11,t12,t13,t21,t22,t23,t31,t32,t33,t41,t42,t43 };
	Scene scene(cam, geometry);
	scene.Render("Mill.ppm");
}

void renderShapeMovedCamera()
{
	Image image(WIDTH, HEIGHT);
	Camera cam(image, Frame(Matrix3::Identity(), Vector3(-3, 0, -1)), FOV);
	cam.Pan(60);

	Material::ColorMaterial m1(Color(255, 0, 0));
	Material::ColorMaterial m2(Color(0, 255, 0));
	Material::ColorMaterial m3(Color(0, 0, 255));

	Geometry::Triangle t11(Vector3(0, 0, -3), Vector3(0.2, -1, -3), Vector3(-0.2, -1, -3), m1);
	Geometry::Triangle t12(Vector3(0, 0, -2.9), Vector3(0.15, -0.9, -2.9), Vector3(-0.15, -0.9, -2.9), m2);
	Geometry::Triangle t13(Vector3(0, 0, -2.8), Vector3(0.1, -0.8, -2.8), Vector3(-0.1, -0.8, -2.8), m3);

	Geometry::Triangle t21(Vector3(0, 0, -3), Vector3(0.2, 1, -3), Vector3(-0.2, 1, -3), m1);
	Geometry::Triangle t22(Vector3(0, 0, -2.9), Vector3(0.15, 0.9, -2.9), Vector3(-0.15, 0.9, -2.9), m2);
	Geometry::Triangle t23(Vector3(0, 0, -2.8), Vector3(0.1, 0.8, -2.8), Vector3(-0.1, 0.8, -2.8), m3);

	Geometry::Triangle t31(Vector3(0, 0, -3), Vector3(-1, 0.2, -3), Vector3(-1, -0.2, -3), m1);
	Geometry::Triangle t32(Vector3(0, 0, -2.9), Vector3(-0.9, 0.15, -2.9), Vector3(-0.9, -0.15, -2.9), m2);
	Geometry::Triangle t33(Vector3(0, 0, -2.8), Vector3(-0.8, 0.1, -2.8), Vector3(-0.8, -0.1, -2.8), m3);

	Geometry::Triangle t41(Vector3(0, 0, -3), Vector3(1, 0.2, -3), Vector3(1, -0.2, -3), m1);
	Geometry::Triangle t42(Vector3(0, 0, -2.9), Vector3(0.9, 0.15, -2.9), Vector3(0.9, -0.15, -2.9), m2);
	Geometry::Triangle t43(Vector3(0, 0, -2.8), Vector3(0.8, 0.1, -2.8), Vector3(0.8, -0.1, -2.8), m3);

	std::vector<Geometry::Triangle>geometry{ t11,t12,t13,t21,t22,t23,t31,t32,t33,t41,t42,t43 };
	Scene scene(cam, geometry);
	scene.Render("MillPan.ppm");
}



int main()
{
	//Task2
	RandomImage();
	CircleImage();

	//Task3
	rayImage();

	//Task4
	Triangles();

	//Task5
	renderTriangle();
	renderTriangles();
	renderShape();

	//Task6
	renderShapeMovedCamera();
}

