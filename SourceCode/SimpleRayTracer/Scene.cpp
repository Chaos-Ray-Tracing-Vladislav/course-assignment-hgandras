#include "Scene.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


//scene without geometry
Scene::Scene() :camera(Camera()){}

//At the moment does nothing, just here so the code compiles.
Scene::Scene(Camera cam, std::vector<Geometry::Triangle> geometry){}

Scene::Scene(Settings sceneSettings) : sceneSettings(sceneSettings)
{
	Frame frame(Matrix3(sceneSettings.matrix).T(), Vector3(sceneSettings.position[0], sceneSettings.position[1], sceneSettings.position[2]));
	Camera cam(sceneSettings.width, sceneSettings.height, frame, 120);
	Image img(sceneSettings.width, sceneSettings.height);

	image = img;
	camera = cam;
}

Scene Scene::FromFile(std::string path, Settings &sceneSettings)
{
	Geometry::AABB aabb;
	Image image;
	Camera camera;

	std::ifstream f(path);
	nlohmann::json data = nlohmann::json::parse(f);

	//Image settings
	nlohmann::json settings = data["settings"];
	std::vector<float> bgCol;
	bgCol.insert(bgCol.begin(), settings["background_color"].begin(), settings["background_color"].end());

	//Camera settings
	nlohmann::json camSettingsJson = data["camera"];
	std::vector<float> matrix;
	std::vector<float> position;

	matrix.insert(matrix.begin(), camSettingsJson["matrix"].begin(), camSettingsJson["matrix"].end());
	position.insert(position.begin(), camSettingsJson["position"].begin(), camSettingsJson["position"].end());

	//Objects
	nlohmann::json geometry = data["objects"];
	std::vector<int> triangles;
	std::vector<float> vertices;
	std::vector<float> uvs;

	float maxX = -std::numeric_limits<float>::max();
	float maxY = -std::numeric_limits<float>::max();
	float maxZ = -std::numeric_limits<float>::max();

	float minX = std::numeric_limits<float>::max();
	float minY = std::numeric_limits<float>::max();
	float minZ = std::numeric_limits<float>::max();

	for (int i = 0; i < geometry.size(); i++)
	{
		triangles.clear();
		vertices.clear();
		uvs.clear();

		vertices.insert(vertices.begin(), geometry[i]["vertices"].begin(), geometry[i]["vertices"].end());
		triangles.insert(triangles.begin(), geometry[i]["triangles"].begin(), geometry[i]["triangles"].end());
		uvs.insert(uvs.begin(), geometry[i]["uvs"].begin(), geometry[i]["uvs"].end());

		std::vector<Geometry::Triangle> faces;
		std::vector<Vector3> vertex_normals(vertices.size() / 3, Vector3::zero());

		//Calculate triangles
		for (int triangleID = 0; triangleID < triangles.size(); triangleID += 3)
		{
			int ind0 = triangles[triangleID] * 3;
			Vector3 v0(vertices[ind0], vertices[ind0 + 1], vertices[ind0 + 2]);
			 
			int ind1 = triangles[triangleID + 1] * 3;
			Vector3 v1(vertices[ind1], vertices[ind1 + 1], vertices[ind1 + 2]);
			
			int ind2 = triangles[triangleID + 2] * 3;
			Vector3 v2(vertices[ind2], vertices[ind2 + 1], vertices[ind2 + 2]);
			
			Vector2 uv1, uv2, uv3;

			if (uvs.size() == 0)
			{
				uv1 = Vector2::zero();
				uv2 = Vector2::zero();
				uv3 = Vector2::zero();
			}
			else
			{
				uv1 = Vector2(uvs[ind0], uvs[ind0 + 1]);
				uv2 = Vector2(uvs[ind1], uvs[ind1 + 1]);
				uv3 = Vector2(uvs[ind2], uvs[ind2 + 1]);
			}

			Geometry::Triangle tr(v0, v1, v2, Vector3(ind0 / 3, ind1 / 3, ind2 / 3),uv1,uv2,uv3);
			faces.push_back(tr);

			vertex_normals[ind0 / 3] = vertex_normals[ind0 / 3] + tr.normal;
			vertex_normals[ind1 / 3] = vertex_normals[ind1 / 3] + tr.normal;
			vertex_normals[ind2 / 3] = vertex_normals[ind2 / 3] + tr.normal;

		}

		for (int i = 0; i < vertex_normals.size(); i++)
			vertex_normals[i] = vertex_normals[i].norm();

		sceneSettings.objects.push_back(Geometry::Object{ faces,vertex_normals,geometry[i]["material_index"] });

		//AABB
		for (int i = 0; i < vertices.size(); i += 3)
		{
			float x = vertices[i];
			float y = vertices[i + 1];
			float z = vertices[i + 2];

			maxX = std::max(maxX, x);
			maxY = std::max(maxY, y);
			maxZ = std::max(maxZ, z);

			minX = std::min(minX, x);
			minY = std::min(minY, y);
			minZ = std::min(minZ, z);
		}
	}

	aabb.max = Vector3(maxX, maxY, maxZ);
	aabb.min = Vector3(minX, minY, minZ);

	//Lights
	nlohmann::json lights = data["lights"];
	for (int i = 0; i < lights.size(); i++)
	{
		nlohmann::json light = lights[i];

		std::vector<float> posVec;
		posVec.insert(posVec.begin(), light["position"].begin(), light["position"].end());

		Light obj{ light["intensity"],Vector3(posVec[0],posVec[1],posVec[2]) };
		sceneSettings.lights.push_back(obj);
	}

#if TEXTURES
	//Textures TODO: REFACTOR THIS I THINK
	nlohmann::json texturesData = data["textures"];
	std::map<std::string,Texture*> texturePtrs;
	static std::unordered_map<std::string, TextureType> const textureMap
	{
		{"albedo",TextureType::ALBEDO},
		{"edges",TextureType::EDGES},
		{"checker",TextureType::CHECKER},
		{"bitmap",TextureType::BITMAP}
	};

	for (int i = 0; i < texturesData.size(); i++)
	{
		TextureType tType = textureMap.find(texturesData[i]["type"])->second;
		std::string name = texturesData[i]["name"];
		switch (tType)
		{
			case TextureType::ALBEDO:
			{
				Vector3 albedo(texturesData[i]["albedo"][0], texturesData[i]["albedo"][1], texturesData[i]["albedo"][2]);
				ColorTexture cTexture{ name,tType,albedo };
				sceneSettings.cTextures.push_back(cTexture);
				texturePtrs.insert({ name,&sceneSettings.cTextures.back()});
				break;
			}

			case TextureType::EDGES:
			{
				Vector3 edgeColor(texturesData[i]["edge_color"][0], texturesData[i]["edge_color"][1], texturesData[i]["edge_color"][2]);
				Vector3 innerColor(texturesData[i]["inner_color"][0], texturesData[i]["inner_color"][1], texturesData[i]["inner_color"][2]);
				EdgeTexture eTexture{ name,tType,edgeColor,innerColor,texturesData[i]["edge_width"] };
				sceneSettings.edgeTextures.push_back(eTexture);
				texturePtrs.insert({ name,&sceneSettings.edgeTextures.back() });
				break;
			}

			case TextureType::CHECKER:
			{
				Vector3 colA(texturesData[i]["color_A"][0], texturesData[i]["color_A"][1], texturesData[i]["color_A"][2]);
				Vector3 colB(texturesData[i]["color_B"][0], texturesData[i]["color_B"][1], texturesData[i]["color_B"][2]);
				CheckerTexture cTexture{ name,tType,colA,colB,texturesData[i]["square_size"] };
				sceneSettings.checkerTextures.push_back(cTexture);
				texturePtrs.insert({ name,&sceneSettings.checkerTextures.back() });
				break;
			}

			case TextureType::BITMAP:
			{
				BitmapTexture bmTexture{name, tType};
				std::string file_path = texturesData[i]["file_path"];
				std::string full_path = "C:/Gazsi/Prog/ChaosCamp/RayTracing/course-assignment-hgandras/SourceCode/Scenes/Task12" + file_path;
				unsigned char* image = stbi_load(full_path.c_str(), &bmTexture.w, &bmTexture.h, &bmTexture.c,0);
				bmTexture.image = image;
				sceneSettings.bmapTextures.push_back(bmTexture);
				texturePtrs.insert({ name,&sceneSettings.bmapTextures.back() });
				break;
			}
		}
		
	}
#endif
	//Materials
	nlohmann::json materialsData = data["materials"];
	std::vector<float> albedo;
	MaterialType mType;

	static std::unordered_map<std::string, MaterialType> const materialMap
	{
		{"diffuse",MaterialType::DIFFUSE},
		{"reflective",MaterialType::REFLECTIVE},
		{"refractive",MaterialType::REFRACTIVE},
		{"constant",MaterialType::CONSTANT}
	};

	for (int i = 0; i < materialsData.size(); i++)
	{
		Material material;
		albedo.clear();
		mType = materialMap.find(materialsData[i]["type"])->second;
#if !TEXTURES
		if (mType == MaterialType::DIFFUSE || mType == MaterialType::REFLECTIVE || mType == MaterialType::CONSTANT)
		{
			albedo.insert(albedo.begin(), materialsData[i]["albedo"].begin(), materialsData[i]["albedo"].end());
			material = { mType,Vector3(albedo[0],albedo[1],albedo[2]),materialsData[i]["smooth_shading"],-1 };
		}
		else if (mType == MaterialType::REFRACTIVE)
		{
			material = { mType, Vector3(-1,-1,-1), materialsData[i]["smooth_shading"],materialsData[i]["ior"] };
		}
#else
		Texture* textureID = texturePtrs.find(materialsData[i]["albedo"])->second;
		material = { mType, Vector3::zero(),materialsData[i]["smooth_shading"],-1,textureID };
#endif
		sceneSettings.materials.push_back(material);
	}


	//AABB
	
	

	

	//Final struct
	sceneSettings.bgCol = Vector3(bgCol[0], bgCol[1], bgCol[2]);
	sceneSettings.width = settings["image_settings"]["width"];
	sceneSettings.height = settings["image_settings"]["height"];
	sceneSettings.bucket_size = settings["image_settings"]["bucket_size"];

	sceneSettings.matrix = matrix;
	sceneSettings.position = position;

	//Ambient coefficient and ambient light
	sceneSettings.k_ambient = 0.1;
	sceneSettings.ambientColor = Vector3(1, 1, 1);
	sceneSettings.aabb = aabb;

	return Scene(sceneSettings);
}