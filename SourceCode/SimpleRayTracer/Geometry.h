#pragma once
#include "Vector.h"
#include "Ray.h"
#include <tuple>
#include <optional>
#include "Material.h"

namespace Geometry
{
	struct Intersection {
		Material::ColorMaterial material;
		Vector3 normal;
		float t;
	};

	struct Triangle
	{
	
		Vector3 v1, v2, v3; //Positions of the vertices
		Material::ColorMaterial material;
		
		Triangle(Vector3 v1,Vector3 v2,Vector3 v3) : v1(v1),v2(v2),v3(v3),material(Material::ColorMaterial{Color(255,0,0)})
		{
			
		}

		Triangle(Vector3 v1, Vector3 v2, Vector3 v3,Material::ColorMaterial color) : v1(v1), v2(v2), v3(v3), material(color)
		{

		}

		Vector3 normal()
		{
			return Cross(v2-v1, v3-v1).norm();
		}

		float area()
		{
			return Cross(v2 - v1, v3 - v1).length() / 2;
		}

		/// <summary>
		/// (These might change, this is just an idea at the moment)
		/// Returns information about the intersection: normal, ray parameter. 
		/// If intersection does not happen, it is null.
		/// </summary>
		/// <param name="ray"></param>
		/// <returns></returns>
		std::optional<Intersection> Intersect(const Ray& ray)
		{
			if(Dot(ray.dir,normal()) == 0 )
				return {};
			float D = -Dot(normal(), v1);
			float t = -(Dot(normal(), ray.origin) + D) / Dot(normal(), ray.dir);
			if (t<0) 
				return {};
			
			Vector3 P = ray.origin + t * ray.dir;
			Vector3 edge0 = v2 - v1;
			Vector3 edge1 = v3 - v2;
			Vector3 edge2 = v1 - v3;
			Vector3 C0 = P - v1;
			Vector3 C1 = P - v2;
			Vector3 C2 = P - v3;
			if (Dot(normal(), Cross(edge0, C0)) > 0 &&
				Dot(normal(), Cross(edge1, C1)) > 0 &&
				Dot(normal(), Cross(edge2, C2)) > 0) return Intersection{material,normal(),t};
			return {};
		}

	};
}
