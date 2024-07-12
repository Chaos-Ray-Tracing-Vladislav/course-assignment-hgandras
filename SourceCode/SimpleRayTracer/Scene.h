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
	Settings sceneSettings;
	
	//generated from scene settings. Might be moved to the raytracer
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
		Frame frame(Matrix3(sceneSettings.matrix).T(), Vector3(sceneSettings.position[0], sceneSettings.position[1], sceneSettings.position[2]));
		Camera cam(sceneSettings.width, sceneSettings.height, frame, 120);
		Image img(sceneSettings.width, sceneSettings.height);

		image = img;
		camera = cam;
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
		nlohmann::json geometry = data["objects"];
		std::vector<Geometry::Object> objects;
		std::vector<int> triangles;
		std::vector<float> vertices;
		for (int i = 0; i < geometry.size(); i++)
		{
			triangles.clear();
			vertices.clear();

			vertices.insert(vertices.begin(), geometry[i]["vertices"].begin(), geometry[i]["vertices"].end());
			triangles.insert(triangles.begin(), geometry[i]["triangles"].begin(), geometry[i]["triangles"].end());
			
			std::vector<Geometry::Triangle> faces;

			for (int triangleID = 0; triangleID < triangles.size(); triangleID += 3)
			{
				int ind0 = triangles[triangleID] * 3;
				Vector3 v0(vertices[ind0], vertices[ind0 + 1], vertices[ind0 + 2]);

				int ind1 = triangles[triangleID + 1] * 3;
				Vector3 v1(vertices[ind1], vertices[ind1 + 1], vertices[ind1 + 2]);

				int ind2 = triangles[triangleID + 2] * 3;
				Vector3 v2(vertices[ind2], vertices[ind2 + 1], vertices[ind2 + 2]);

				Geometry::Triangle tr(v0, v1, v2);
				faces.push_back(tr);
			}
			
			Material mat{ Vector3(0.0005,0,0) };
			objects.push_back(Geometry::Object{ faces,mat });
		}

		//Lights
		nlohmann::json lights = data["lights"];
		std::vector<Light> lightList;
		for (int i = 0; i < lights.size(); i++)
		{
			nlohmann::json light = lights[i];

			std::vector<float> posVec;
			posVec.insert(posVec.begin(), light["position"].begin(), light["position"].end());

			Light obj{ light["intensity"],Vector3(posVec[0],posVec[1],posVec[2])};
			lightList.push_back(obj);
		}

		//Final struct
		Settings sceneSettings{
			Color((int)(bgCol[0] * 255),(int)(bgCol[1] * 255),(int)(bgCol[2] * 255)),
			settings["image_settings"]["width"],
			settings["image_settings"]["height"],

			matrix,
			position,

			objects,

			lightList,

			//Ambient coefficient and ambient light
			0.1,  
			Vector3(1,1,1)
		};

		return Scene(sceneSettings);

	}
};
