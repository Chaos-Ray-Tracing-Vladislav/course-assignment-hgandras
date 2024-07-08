#pragma once
#include "Camera.h"
#include "Geometry.h"
#include "Ray.h"
#include "Image.h"
#include "Vector.h"
#include <vector>
#include <algorithm>
#include <string>

struct Scene {
	Camera cam;
	Color bg;
	std::vector<Geometry::Triangle> geometry;

	//scene without geometry
	Scene():cam(Camera()), bg(Color(128, 128, 128))
	{
			
	}

	Scene(Camera& cam, std::vector<Geometry::Triangle> geometryList) :cam(cam), bg(Color(128, 128, 128))
	{
		geometry.reserve(geometryList.size());
		geometry.insert(geometry.begin(), geometryList.begin(),geometryList.end());
	}

	void Render(std::string imgName) {
		std::vector<Geometry::Intersection> intersections;
		for (int i = 0; i < cam.image.w * cam.image.h; i++)
		{
			int x = i % cam.image.w;
			int y = i / cam.image.w;
			Ray ray = cam.CastRay(x, y);
			intersections.reserve(geometry.size());

			for (int j = 0; j < geometry.size(); j++)
			{
				auto intersection = geometry[j].Intersect(ray);
				if (intersection)
				{
					intersections.push_back(intersection.value());
				}
			}

			//Sort intersections, and get material
			if (intersections.empty())
			{
				cam.image.setPixel(x, y, bg);
			}
			else {
				std::sort(intersections.begin(), intersections.end(), customSort);
				cam.image.setPixel(x, y, intersections[0].material.color);
			}
			intersections.clear();
		}
		cam.image.writePPM(imgName);
	}
private:
	struct
	{
		bool operator()(Geometry::Intersection a, Geometry::Intersection b) const { return a.t<b.t; }
	}
	customSort;


};
