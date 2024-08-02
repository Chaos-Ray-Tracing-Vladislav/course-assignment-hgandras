#include "Geometry.h"


namespace Geometry {


	Triangle::Triangle() : id_v1(-1), id_v2(-1), id_v3(-1) {

	}

	Triangle::Triangle(Vector3 v1, Vector3 v2, Vector3 v3, int id1, int id2, int id3) : v1(v1), v2(v2), v3(v3), id_v1(id1), id_v2(id2), id_v3(id3)
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
}
