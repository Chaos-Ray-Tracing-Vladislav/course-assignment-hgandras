#pragma once
#include "Raytracer.h"
#include "Threadpool.h"

std::optional<Geometry::Intersection> RayTracer::IntersectRay(Ray& ray)
{
	Geometry::Intersection closestIntersection;
	float minT;
	closestIntersection.t = FLT_MAX;
	bool intersected = false;
	int objID = 0;

	for (auto& object : scene.sceneSettings.objects)
	{
		for (auto& triangle : object.triangles)
		{
			float t = triangle.Intersect(ray);
			if (t >= 0 && t < closestIntersection.t)
			{
				minT = t;
				intersected = true;

				closestIntersection.material = &scene.sceneSettings.materials[object.materialID];
				closestIntersection.triangle = &triangle;
				closestIntersection.t = minT;
				closestIntersection.objectID = objID;
				Vector3 intersectionPoint = minT * ray.dir + ray.origin;
				Vector3 barycentricCoords = barycentricCoordinates(intersectionPoint, closestIntersection.triangle);
				closestIntersection.normal = closestIntersection.material->smooth_shading ? barycentricNorm(barycentricCoords, closestIntersection) : closestIntersection.triangle->normal;
				closestIntersection.barycentric = barycentricCoords;

				//Only take 1st 2 coords of UV, since 3rd is always 0
				closestIntersection.uv = barycentricCoords.x * triangle.uv2 + barycentricCoords.y * triangle.uv3 + barycentricCoords.z * triangle.uv1;
 			}
		}
		objID++;
	}

	if (intersected) {
		return closestIntersection;
	}
	return {};
}

Vector3 RayTracer::barycentricCoordinates(const Vector3& intersectionPoint, Geometry::Triangle* triangle)
{
	float u = Geometry::Triangle::Area(triangle->v1, intersectionPoint, triangle->v3) / triangle->area;
	float v = Geometry::Triangle::Area(triangle->v2, intersectionPoint, triangle->v1) / triangle->area;

	return Vector3(u, v, 1 - u - v);
}

Vector3 RayTracer::barycentricNorm(const Vector3& baryCoords, Geometry::Intersection& isec)
{
	Vector3 n1 = baryCoords.z * scene.sceneSettings.objects[isec.objectID].vertex_normals[isec.triangle->vertexIDs.x];
	Vector3 n2 = baryCoords.x * scene.sceneSettings.objects[isec.objectID].vertex_normals[isec.triangle->vertexIDs.y];
	Vector3 n3 = baryCoords.y * scene.sceneSettings.objects[isec.objectID].vertex_normals[isec.triangle->vertexIDs.z];

	return n1 + n2 + n3;
}

Vector3 RayTracer::traceRay(Ray& out, Vector3& finalColor, int bounces)
{
	auto intersection = IntersectRay(out);

	if (!intersection)
		return scene.sceneSettings.bgCol;

	Geometry::Intersection isec = intersection.value();

	if (isec.material->type == MaterialType::CONSTANT)
		return isec.material->albedo;

	Vector3 intersectionPoint = isec.t * out.dir + out.origin;
	if (isec.material->type == MaterialType::DIFFUSE) {
		Vector3 Lin; //Incoming light direction
		for (Light light : scene.sceneSettings.lights)
		{
			Lin = light.position - intersectionPoint;
			float sphereArea = 4 * Lin.length() * Lin.length() * PI;
			Ray shadowRay(intersectionPoint + scene.sceneSettings.EPSILON * isec.normal, Lin.norm());
			auto shadow = IntersectRay(shadowRay);
			if (isec.material->texturePtr!=nullptr && (!shadow || shadow.value().t > Lin.length() || shadow.value().material->type == MaterialType::REFRACTIVE))
			{
				switch (isec.material->texturePtr->type)
				{
					case TextureType::ALBEDO:
					{
						ColorTexture* cTexture = dynamic_cast<ColorTexture*>(isec.material->texturePtr);
						finalColor = finalColor + cTexture->Sample(intersection.value().uv,intersection.value().barycentric) * light.intensity / sphereArea * std::max(0.0f, Dot(Lin.norm(), isec.normal));
					}
					break;

					case TextureType::EDGES:
					{
						EdgeTexture* eTexture = dynamic_cast<EdgeTexture*>(isec.material->texturePtr);
						finalColor = finalColor + eTexture->Sample(intersection.value().uv, intersection.value().barycentric) * light.intensity / sphereArea * std::max(0.0f, Dot(Lin.norm(), isec.normal));
					}
					break;

					case TextureType::CHECKER:
					{
						CheckerTexture* checkerTexture = dynamic_cast<CheckerTexture*>(isec.material->texturePtr);
						finalColor = finalColor + checkerTexture->Sample(intersection.value().uv, intersection.value().barycentric) * light.intensity / sphereArea * std::max(0.0f, Dot(Lin.norm(), isec.normal));
					}
					break;

					case TextureType::BITMAP:
					{
						BitmapTexture* bTexture = dynamic_cast<BitmapTexture*>(isec.material->texturePtr);
						finalColor = finalColor + bTexture->Sample(intersection.value().uv, intersection.value().barycentric) * light.intensity / sphereArea * std::max(0.0f, Dot(Lin.norm(), isec.normal));
					}
					break;
				}
			}
			else
				finalColor = finalColor + isec.material->albedo * light.intensity / sphereArea * std::max(0.0f, Dot(Lin.norm(), isec.normal));
		}
	}

	else if (isec.material->type == MaterialType::REFLECTIVE)
	{

		if (bounces > scene.sceneSettings.MAX_BOUNCE)
			return finalColor;
		bounces++;
		Vector3 reflectedDir = Reflect(out.dir, isec.normal);
		Ray Rin = Ray(intersectionPoint + scene.sceneSettings.EPSILON * isec.normal, reflectedDir);
		finalColor = finalColor + isec.material->albedo * traceRay(Rin, finalColor, bounces);
	}

	else if (isec.material->type == MaterialType::REFRACTIVE)
	{
		if (bounces > scene.sceneSettings.MAX_BOUNCE)
			return finalColor;
		bounces++;
		float n1 = 1.f;
		float n2 = isec.material->ior;
		Vector3 normal = isec.normal;
		float cosIntersectionAngle = Dot(out.dir, isec.normal);
		if (cosIntersectionAngle > 0) {
			std::swap(n1, n2);
			normal = -normal;
		}
		float cosT1 = -Dot(out.dir, normal);
		if (sinf(acosf(cosT1)) < n2 / n1)
		{
			//Refraction ray
			float sinT2 = n1 * sqrtf(1 - cosT1 * cosT1) / n2;
			float cosT2 = sqrtf(1 - sinT2 * sinT2);
			Vector3 refractDir = n1 / n2 * out.dir + (n1 / n2 * cosT1 - cosT2) * normal;
			Ray refractedRay(intersectionPoint - normal * scene.sceneSettings.EPSILON, refractDir);
			Vector3 refractionColor = traceRay(refractedRay, finalColor, bounces);

			//Reflection ray
			Vector3 reflectedDir = Reflect(out.dir, normal);
			Ray reflectedRay = Ray(intersectionPoint + scene.sceneSettings.EPSILON * normal, reflectedDir);
			Vector3 reflectionColor = traceRay(reflectedRay, finalColor, bounces);

			float kr = 0.5f * powf((1.0 - cosT1), 5);
			finalColor = kr * reflectionColor + (1 - kr) * refractionColor;
		}
		else
		{
			//reflect ray
			if (bounces > scene.sceneSettings.MAX_BOUNCE)
				return finalColor;
			bounces++;
			Vector3 reflectedDir = Reflect(out.dir, normal);
			Ray Rin = Ray(intersectionPoint + scene.sceneSettings.EPSILON * normal, reflectedDir);
			finalColor = traceRay(Rin, finalColor, bounces);
		}
	}

	return finalColor;
}

RayTracer::RayTracer(Scene& scene) : scene(scene) {}

void RayTracer::Render(std::string imgName) {
	scene.image.clear();
	for (int i = 0; i < scene.image.w * scene.image.h; i++)
	{
		int x = i % scene.image.w;
		int y = i / scene.image.w;
		Ray ray = scene.camera.CastRay(x, y);

		Vector3 finalColor = Vector3::zero();
		finalColor = 255 * Min(Vector3(1.f, 1.f, 1.f), traceRay(ray, finalColor, 0));
		scene.image.setPixel(x, y, Color((int)finalColor.x, (int)finalColor.y, (int)finalColor.z));
	}
	scene.image.writePPM(imgName);
}

void RayTracer::RenderRegion(int px_w, int px_h, int region_w, int region_h)
{
	for (int h = px_h; h < px_h+region_h; h++)
	{
		for (int w = px_w; w < px_w+region_w; w++)
		{
			if (h >= scene.sceneSettings.height || w >= scene.sceneSettings.width)
				continue;

			Ray ray = scene.camera.CastRay(w, h);

			Vector3 finalColor = Vector3::zero();
			finalColor = 255 * Min(Vector3(1.f, 1.f, 1.f), traceRay(ray, finalColor, 0));
			scene.image.setPixel(w, h, Color((int)finalColor.x, (int)finalColor.y, (int)finalColor.z));
		}
	}
}

void RayTracer::ParallelRegionsRender(std::string imgName)
{
	scene.image.clear();
	std::vector<std::jthread> threads;
	int num_threads = std::thread::hardware_concurrency();
	int h_threads = sqrt(num_threads);
	int w_threads = sqrt(num_threads);

	int region_w = scene.sceneSettings.width / w_threads;
	int region_h = scene.sceneSettings.height / h_threads;

	for (int y = 0; y < h_threads; y++)
	{
		for (int x = 0; x < w_threads; x++)
		{
			threads.push_back(std::jthread(&RayTracer::RenderRegion,this,x*region_w, y*region_h, region_w, region_h));
		}
	}
	threads.clear();
	scene.image.writePPM(imgName);
}

void RayTracer::ParallelBucketsRender(std::string imgName)
{
	scene.image.clear();
	ThreadPool thread_pool;
	thread_pool.Start(std::thread::hardware_concurrency());

	int w_threads = scene.sceneSettings.width / scene.sceneSettings.bucket_size;
	int h_threads = scene.sceneSettings.height / scene.sceneSettings.bucket_size;

	for (int y = 0; y < h_threads; y++)
	{
		for (int x = 0; x < w_threads; x++)
		{
			std::function<void()> func = std::bind(&RayTracer::RenderRegion, this, x * scene.sceneSettings.bucket_size, y * scene.sceneSettings.bucket_size, scene.sceneSettings.bucket_size, scene.sceneSettings.bucket_size);
			thread_pool.AddTask(func);
		}
	}
	thread_pool.WaitForAll();
	scene.image.writePPM(imgName);
}

void RayTracer::AABBRender(std::string imgName)
{
	scene.image.clear();
	for (int i = 0; i < scene.image.w * scene.image.h; i++)
	{
		int x = i % scene.image.w;
		int y = i / scene.image.w;
		Ray ray = scene.camera.CastRay(x, y);

		if (scene.sceneSettings.aabb.Intersect(ray))
		{
			Vector3 finalColor = Vector3::zero();
			finalColor = 255 * Min(Vector3(1.f, 1.f, 1.f), traceRay(ray, finalColor, 0));
			scene.image.setPixel(x, y, Color((int)finalColor.x, (int)finalColor.y, (int)finalColor.z));
		}
		else
		{
			scene.image.setPixel(x, y, Color(255*scene.sceneSettings.bgCol.x, 255*scene.sceneSettings.bgCol.y, 255*scene.sceneSettings.bgCol.z));
		}
	}
	scene.image.writePPM(imgName);
}
