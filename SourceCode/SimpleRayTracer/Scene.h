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

//TODO: Make scene settings be the same as the json sepcification, and move everything else to the scene like objects, and 
//complete materials. This will hopefully make the code a bit more readable.
struct Scene {
	Settings sceneSettings;
	
	//generated from scene settings. Might be moved to the raytracer
	Camera camera;
	Image image;

	//scene without geometry
	Scene();

	//At the moment does nothing, just here so the code compiles.
	Scene(Camera cam, std::vector<Geometry::Triangle> geometry);

	Scene(Settings sceneSettings);

	static Scene FromFile(std::string path);
};
