#include "Geometry.h"
#include <limits>

namespace Geometry {

	Triangle::Triangle() {}

	Triangle::Triangle(Vector3 v1, Vector3 v2, Vector3 v3, Vector3 vertexIDs, Vector2 uv1,Vector2 uv2,Vector2 uv3) : v1(v1), v2(v2), v3(v3), vertexIDs(vertexIDs),uv1(uv1),uv2(uv2), uv3(uv3)
	{
		area = Cross(v2 - v1, v3 - v1).length() / 2;
		normal = Cross(v2 - v1, v3 - v1).norm();
	}

	/// <summary>
	/// (These might change, this is just an idea at the moment)
	/// Returns information about the intersection: normal, ray parameter. 
	/// If intersection does not happen, it is null.
	/// </summary>
	/// <param name="ray"></param>
	/// <returns></returns>
	float Triangle:: Intersect(const Ray& ray)
	{
		if (Dot(ray.dir, normal) == 0)
			return -1;
		float D = -Dot(normal, v1);
		float t = -(Dot(normal, ray.origin) + D) / Dot(normal, ray.dir);
		if (t < 0)
			return -1;

		Vector3 P = ray.origin + t * ray.dir;
		Vector3 edge0 = v2 - v1;
		Vector3 edge1 = v3 - v2;
		Vector3 edge2 = v1 - v3;
		Vector3 C0 = P - v1;
		Vector3 C1 = P - v2;
		Vector3 C2 = P - v3;
		if (Dot(normal, Cross(edge0, C0)) > 0 &&
			Dot(normal, Cross(edge1, C1)) > 0 &&
			Dot(normal, Cross(edge2, C2)) > 0) return t;
		return -1;
	}

	float Triangle::Area(const Vector3 v1, const Vector3 v2, const Vector3 v3)
	{
		return  Cross(v2 - v1, v3 - v1).length() / 2;
	}

	Vector3 Triangle::Normal(const Vector3 v1, const Vector3 v2, const Vector3 v3)
	{
		return Cross(v2 - v1, v3 - v1).norm();
	}


	//From https://medium.com/@bromanz/another-view-on-the-classic-ray-aabb-intersection-algorithm-for-bvh-traversal-41125138b525
	bool AABB::Intersect(Ray& ray)
	{
		float tmin = -std::numeric_limits<float>::max();
		float tmax = std::numeric_limits<float>::max();

		Vector3 invD(1 / ray.dir.x, 1 / ray.dir.y, 1 / ray.dir.z);

		Vector3 t0s = (min - ray.origin) * invD;
		Vector3 t1s = (max - ray.origin) * invD;

		Vector3 tsmaller = Min(t0s, t1s);
		Vector3 tbigger = Max(t0s, t1s);

		tmin = std::max(tmin, std::max(tsmaller.x, std::max(tsmaller.y, tsmaller.z)));
		tmax = std::min(tmax, std::min(tbigger.x, std::min(tbigger.y, tbigger.z)));

		return tmin < tmax;
	}
}
