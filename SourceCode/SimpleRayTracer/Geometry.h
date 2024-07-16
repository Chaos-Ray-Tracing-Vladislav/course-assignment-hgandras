#pragma once
#include "Vector.h"
#include "Ray.h"
#include <tuple>
#include <optional>
#include "Material.h"

namespace Geometry
{
	struct Triangle
	{
	
		const Vector3 v1, v2, v3; //Positions of the vertices
		const int id_v1, id_v2, id_v3; //Vertex indices
		float area;
		Vector3 normal;
		
		Triangle();

		Triangle(Vector3 v1, Vector3 v2, Vector3 v3, int id1, int id2, int id3);

		/// <summary>
		/// (These might change, this is just an idea at the moment)
		/// Returns information about the intersection: normal, ray parameter. 
		/// If intersection does not happen, it is null.
		/// </summary>
		/// <param name="ray"></param>
		/// <returns></returns>
		float Intersect(const Ray& ray);

		static float Area(const Vector3 v1, const Vector3 v2, const Vector3 v3);

		static Vector3 Normal(const Vector3 v1, const Vector3 v2, const Vector3 v3);
	};

	struct Object {
		std::vector<Geometry::Triangle> triangles;
		std::vector<Vector3> vertex_normals;
		int materialID;
	};

	struct Intersection {
		Triangle* triangle;
		Material* material;
		float t;
		int objectID;
		Vector3 normal; //This is here, because it is defferent from the triangle's normal when 
			 			//smooth shading is used.
	};
}
