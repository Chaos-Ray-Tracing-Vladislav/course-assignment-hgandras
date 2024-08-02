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
		const Vector3 vertexIDs; //Vertex indices
		Vector2 uv1,uv2,uv3;
		float area;
		Vector3 normal;
		
		Triangle();

		Triangle(Vector3 v1, Vector3 v2, Vector3 v3, Vector3 vertexIDs,Vector2 uv1,Vector2 uv2, Vector2 uv3);

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
		std::vector<Vector3> uvs;
	};

	struct Intersection {
		Triangle* triangle;
		Material* material;
		float t;
		int objectID;
		Vector3 normal; //This is here, because it is defferent from the triangle's normal when 
			 			//smooth shading is used.
		Vector3 barycentric;
		Vector2 uv;
	};

	struct AABB {
		Vector3 min;
		Vector3 max;

		bool Intersect(Ray&);
	};
}
