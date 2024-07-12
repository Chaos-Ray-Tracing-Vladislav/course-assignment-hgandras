#pragma once
#include "Scene.h"

class RayTracer
{
private:
	std::optional<Geometry::Intersection> IntersectRay(Ray ray, std::vector<Geometry::Object> objects)
	{
		Geometry::Intersection closestIntersection;
		float minT;
		Material intersectedMaterial;
		closestIntersection.t = FLT_MAX;
		bool intersected=false;

		for(auto& object : scene.sceneSettings.objects)
		{
			for(auto& triangle : object.triangles)
			{
				float t = triangle.Intersect(ray);
				if(t>=0 && t < closestIntersection.t)
				{
					minT = t;
					intersected = true;
					intersectedMaterial = object.material;

					closestIntersection.material = object.material;
					closestIntersection.normal = triangle.normal;
					closestIntersection.t = minT;
				}
			}
		}

		if (intersected) {
			return closestIntersection;
		}
		return {};
	}

public:
	Scene scene;

	RayTracer(Scene scene): scene(scene){}

	void Render(std::string imgName) {
		for (int i = 0; i < scene.image.w * scene.image.h; i++)
		{
			int x = i % scene.image.w;
			int y = i / scene.image.w;
			Ray ray = scene.camera.CastRay(x, y);

			auto intersection = IntersectRay(ray, scene.sceneSettings.objects);
			
			if(!intersection)
			{
				scene.image.setPixel(x, y, scene.sceneSettings.bgCol);
			}
			else 
			{
				Vector3 intersectionPoint = intersection.value().t * ray.dir + ray.origin;
				float reflectedIntensity = 0.f;
				Vector3 in;
				for (int i = 0; i < scene.sceneSettings.lights.size(); i++)
				{
					Light light = scene.sceneSettings.lights[i];
					in = (light.position - intersectionPoint).norm();
					Ray shadowRay(intersectionPoint + scene.sceneSettings.EPSILON*in, in);
					auto shadow = IntersectRay(shadowRay, scene.sceneSettings.objects);
					if(!shadow)
						reflectedIntensity = reflectedIntensity + light.intensity  * std::max(0.0f, Dot(in, intersection.value().normal));
				}
				Vector3 finalColor = 255*Min(Vector3(1.f,1.f,1.f), 1/PI * intersection.value().material.albedo * reflectedIntensity);
				scene.image.setPixel(x, y, Color((int)finalColor.x,(int)finalColor.y,(int)finalColor.z));
			}
			
		}
		scene.image.writePPM(imgName);
	}
};