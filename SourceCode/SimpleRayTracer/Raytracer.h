#pragma once
#include "Scene.h"

class RayTracer
{
private:
	std::optional<Geometry::Intersection> IntersectRay(Ray ray, std::vector<Geometry::Object> objects)
	{
		Geometry::Intersection closestIntersection;
		float minT;
		closestIntersection.t = FLT_MAX;
		bool intersected=false;
		int objID = 0;

		for(auto& object : scene.sceneSettings.objects)
		{
			for(auto& triangle : object.triangles)
			{
				float t = triangle.Intersect(ray);
				if(t>=0 && t < closestIntersection.t)
				{
					minT = t;
					intersected = true;

					closestIntersection.material = &scene.sceneSettings.materials[object.materialID];
					closestIntersection.triangle = &triangle;
					closestIntersection.t = minT;
					closestIntersection.objectID = objID;
				}
			}
			objID++;
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
				Geometry::Intersection isec = intersection.value();
				Vector3 finalColor = Vector3::zero();
				Vector3 intersectionPoint = isec.t * ray.dir + ray.origin;
				Vector3 Lin;

				//TODO: If shading is not smooth, dont calculate the barycentric coordinates. Also refactor the 
				//whole function, looks ugly:(

				float u = Geometry::Triangle::Area(isec.triangle->v1, intersectionPoint, isec.triangle->v3)/isec.triangle->area;
				float v = Geometry::Triangle::Area(isec.triangle->v2, intersectionPoint, isec.triangle->v1)/isec.triangle->area;

				Vector3 baryCoords(u,v, 1-u-v);

				//If there are no lights, color by barycentric coordinates
				if(scene.sceneSettings.lights.size() == 0)
				{
					finalColor = 255 * baryCoords;
				}
				else {
					for (Light light : scene.sceneSettings.lights)
					{
						Lin = (light.position - intersectionPoint);
						float sphereArea = 4 * Lin.length() * Lin.length() * PI;
						Lin = Lin.norm();

						Vector3 n1 = baryCoords.z * scene.sceneSettings.objects[isec.objectID].vertex_normals[isec.triangle->id_v1];
						Vector3 n2 = baryCoords.x * scene.sceneSettings.objects[isec.objectID].vertex_normals[isec.triangle->id_v2];
						Vector3 n3 = baryCoords.y * scene.sceneSettings.objects[isec.objectID].vertex_normals[isec.triangle->id_v3];
						Vector3 norm = isec.material->smooth_shading ? n1 + n2 + n3 : isec.triangle->normal;

						Ray shadowRay(intersectionPoint + scene.sceneSettings.EPSILON * Lin, Lin);
						auto shadow = IntersectRay(shadowRay, scene.sceneSettings.objects);
						if (!shadow)
						{
							finalColor = finalColor + isec.material->albedo * light.intensity / sphereArea * std::max(0.0f, Dot(Lin, norm));
							if (isec.material->type == MaterialType::REFLECTIVE)
							{
								for (int bounce = 0; bounce < scene.sceneSettings.MAX_BOUNCE; bounce++)
								{
									Vector3 Rin = Reflect(ray.dir, norm);
									Ray reflected = Ray(intersectionPoint + scene.sceneSettings.EPSILON * Rin, Rin);
									auto bounceIntersection = IntersectRay(reflected,scene.sceneSettings.objects);
									if (!bounceIntersection)
										break;
									finalColor = finalColor + bounceIntersection.value().material->albedo * light.intensity / sphereArea * std::max(0.0f, Dot(Lin, norm));

								}
							}
						}
					}
					finalColor = 255 * Min(Vector3(1.f, 1.f, 1.f),  finalColor);
				}
				
				scene.image.setPixel(x, y, Color((int)finalColor.x,(int)finalColor.y,(int)finalColor.z));
			}
		}
		scene.image.writePPM(imgName);
	}
};