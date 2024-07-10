#pragma once
#include "Camera.h"
#include "Geometry.h"
#include "Ray.h"
#include "Image.h"
#include "Vector.h"
#include "nlohmann/json.hpp"
#include "SceneSettings.h"

#include <vector>
#include <algorithm>
#include <string>

struct Scene {
private:
	std::vector<Geometry::Triangle> geometry;
public:
	Settings sceneSettings;
	Camera camera;
	Image image;

	//scene without geometry
	Scene():camera(Camera())
	{
			
	}

	//At the moment does nothing, just here so the code compiles.
	Scene(Camera cam, std::vector<Geometry::Triangle> geometry)
	{

	}

	Scene(Settings sceneSettings) : sceneSettings(sceneSettings)							
	{
		Frame frame(Matrix3(sceneSettings.matrix), Vector3(sceneSettings.position[0], sceneSettings.position[1], sceneSettings.position[2]));
		Camera cam(sceneSettings.width, sceneSettings.height, frame, 90);
		Image img(sceneSettings.width, sceneSettings.height);

		image = img;
		camera = cam;

		//Precalculate triangles
		for (int i = 0; i < sceneSettings.triangles.size() ; i+=3)
		{
			int ind0 = sceneSettings.triangles[i] * 3;
			Vector3 v0(sceneSettings.vertices[ind0], sceneSettings.vertices[ind0 + 1], sceneSettings.vertices[ind0 + 2]);

			int ind1 = sceneSettings.triangles[i + 1] * 3;
			Vector3 v1(sceneSettings.vertices[ind1], sceneSettings.vertices[ind1 + 1], sceneSettings.vertices[ind1 + 2]);

			int ind2 = sceneSettings.triangles[i + 2] * 3;
			Vector3 v2(sceneSettings.vertices[ind2], sceneSettings.vertices[ind2 + 1], sceneSettings.vertices[ind2 + 2]);

			geometry.push_back(Geometry::Triangle(v0, v1, v2));
		}
	}

	void Render(std::string imgName) {
		for (int i = 0; i < image.w * image.h; i++)
		{
			Geometry::Intersection closestIntersection;
			float closestT = FLT_MAX;
			bool intersected = false;
			int x = i % image.w;
			int y = i / image.w;
			Ray ray = camera.CastRay(x, y);

			for (int j = 0; j < geometry.size(); j++)
			{
				
				auto intersection = geometry[j].Intersect(ray);
				if (intersection && intersection.value().t<closestT)
				{
					closestIntersection = intersection.value();
					closestT = closestIntersection.t;
					intersected = true;
				}
			}

			if (!intersected)
			{
				image.setPixel(x, y, sceneSettings.bgCol);
			}
			else {
				image.setPixel(x, y, closestIntersection.material.color);
			}
		}
		image.writePPM(imgName);
	}

	static Scene FromFile(std::string path)
	{

		Image image;
		Camera camera;

		std::ifstream f(path);
		nlohmann::json data = nlohmann::json::parse(f);

		//Image settings
		nlohmann::json settings = data["settings"];
		std::vector<float> bgCol;
		bgCol.insert(bgCol.begin(),settings["background_color"].begin(),settings["background_color"].end());

		//Camera settings
		nlohmann::json camSettingsJson = data["camera"];
		std::vector<float> matrix;
		std::vector<float> position;

		matrix.insert(matrix.begin(), camSettingsJson["matrix"].begin(), camSettingsJson["matrix"].end());
		position.insert(position.begin(), camSettingsJson["position"].begin(), camSettingsJson["position"].end());
		
		//Objects
		nlohmann::json geometry = data["objects"][0];
		
		std::vector<float> vertices;
		vertices.insert(vertices.begin(), geometry["vertices"].begin(), geometry["vertices"].end());

		std::vector<int> triangles;
		triangles.insert(triangles.begin(), geometry["triangles"].begin(), geometry["triangles"].end());

		//Final struct
		Settings sceneSettings{ 
			Color((int)(bgCol[0]*255),(int)(bgCol[1]*255),(int)(bgCol[2]*255)),
			settings["image_settings"]["width"],
			settings["image_settings"]["height"],

			matrix,
			position,

			vertices,
			triangles 
		};

		return Scene(sceneSettings);

	}
};
