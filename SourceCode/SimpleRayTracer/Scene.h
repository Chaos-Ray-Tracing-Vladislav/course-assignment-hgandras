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
			std::vector<Vector3> vertex_normals(vertices.size()/3,Vector3::zero());

			//Calculate 
			for(int triangleID = 0; triangleID < triangles.size(); triangleID += 3)
			{
				int ind0 = triangles[triangleID] * 3;
				Vector3 v0(vertices[ind0], vertices[ind0 + 1], vertices[ind0 + 2]);

				int ind1 = triangles[triangleID + 1] * 3;
				Vector3 v1(vertices[ind1], vertices[ind1 + 1], vertices[ind1 + 2]);

				int ind2 = triangles[triangleID + 2] * 3;
				Vector3 v2(vertices[ind2], vertices[ind2 + 1], vertices[ind2 + 2]);

				Geometry::Triangle tr(v0, v1, v2,ind0/3,ind1/3,ind2/3);
				faces.push_back(tr);

				vertex_normals[ind0/3] = vertex_normals[ind0/3] + tr.normal;
				vertex_normals[ind1/3] = vertex_normals[ind1/3] + tr.normal;
				vertex_normals[ind2/3] = vertex_normals[ind2/3] + tr.normal;
			}

			for (int i = 0; i < vertex_normals.size(); i++)
				vertex_normals[i] = vertex_normals[i].norm();
			
			objects.push_back(Geometry::Object{ faces,vertex_normals,geometry[i]["material_index"]});
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

		//Materials
		nlohmann::json materials = data["materials"];
		std::vector<Material> materialList;
		std::vector<float> albedo;
		MaterialType type;
		static std::unordered_map<std::string, MaterialType> const map{ {"diffuse",MaterialType::DIFFUSE} };
		for (int i = 0; i < materials.size(); i++)
		{
			albedo.clear();
			albedo.insert(albedo.begin(),materials[i]["albedo"].begin(),materials[i]["albedo"].end());
			type = map.find(materials[i]["type"])->second;
			Material material{ type,Vector3(albedo[0],albedo[1],albedo[2]),materials[i]["smooth_shading"]};
			materialList.push_back(material);
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
			Vector3(1,1,1),

			materialList
		};

		return Scene(sceneSettings);

	}
};
