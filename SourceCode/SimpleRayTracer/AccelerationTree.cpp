#include "AccelerationTree.h"


AccelerationTree::AccelerationTree(Scene& scene) : scene(scene)
{
	root = {-1,-1,AccelerationTree::scene.sceneSettings.aabb,std::map<int,std::vector<Geometry::Triangle>>()};
	int depth = 0;
	int objID = 0;
	std::map<int, std::vector<Geometry::Triangle>> objects;
	for (auto& obj : AccelerationTree::scene.sceneSettings.objects)
	{
		objects.insert({ objID,obj.triangles });
		objID++;
	}
	nodes.push_back(root);
	BuildTree(0,depth,objects);
}

void AccelerationTree::BuildTree(int parentID, int depth, std::map<int, std::vector<Geometry::Triangle>>& objects)
{
	int numTriangles = AccelerationTree::countNodeTriangles(objects);
	if (numTriangles <= maxTriangles || depth > maxDepth)
	{
		nodes[parentID].objects = objects;
		return;
	}
	std::tuple<Geometry::AABB, Geometry::AABB> aabbs = nodes[parentID].AABB.Split(depth%3);

	Geometry::AABB child1AABB = std::get<0>(aabbs);
	Geometry::AABB child2AABB = std::get<1>(aabbs);

	std::map<int, std::vector<Geometry::Triangle>> child1Objects;
	std::map<int, std::vector<Geometry::Triangle>> child2Objects;
	
	for(std::map<int, std::vector<Geometry::Triangle>>::iterator iter = objects.begin(); iter != objects.end(); ++iter)
	{
		std::vector<Geometry::Triangle> trianglesC1, trianglesC2;
		for (auto& triangle : iter->second)
		{
			if (child1AABB.Intersect(triangle))
				trianglesC1.push_back(triangle);
			if (child2AABB.Intersect(triangle))
				trianglesC2.push_back(triangle);
		}
		child1Objects.insert({ iter->first,trianglesC1 });
		child2Objects.insert({ iter->first,trianglesC2 });
	}

	int c1NumTriangles = countNodeTriangles(child1Objects);
	int c2NumTriangles = countNodeTriangles(child2Objects);

	if (c1NumTriangles > 0)
	{
		ACTreenode child1{ -1,-1,child1AABB,std::map<int,std::vector<Geometry::Triangle>>() };
		nodes.push_back(child1);
		nodes[parentID].Child1ID = nodes.size()-1;
		BuildTree(nodes.size()-1, depth + 1, child1Objects);
	}
	if (c2NumTriangles > 0)
	{
		ACTreenode child2{ -1,-1,child2AABB,std::map<int,std::vector<Geometry::Triangle>>() };
		nodes.push_back(child2);
		nodes[parentID].Child2ID = nodes.size()-1;
		BuildTree(nodes.size()-1, depth + 1, child2Objects);
	}
}

int AccelerationTree::countNodeTriangles(std::map<int, std::vector<Geometry::Triangle>>& objects)
{
	int size = 0;
	for (std::map<int,std::vector<Geometry::Triangle>>::iterator iter = objects.begin(); iter != objects.end(); ++iter)
	{
		size += iter->second.size();
	}
	return size;
}

std::optional<Geometry::Intersection> AccelerationTree::Traverse(Ray& ray)
{
	std::queue<int> nodesToCheck;
	nodesToCheck.push(0);
	Geometry::Intersection closestIntersection;
	closestIntersection.t = FLT_MAX;
	bool intersected = false;
	while (nodesToCheck.size() > 0)
	{
		int currentNodeID = nodesToCheck.front();
		ACTreenode currentNode = nodes[currentNodeID];
		nodesToCheck.pop();
		if (currentNode.AABB.Intersect(ray))
		{
			int triCount = countNodeTriangles(currentNode.objects);
			if (triCount > 0)
			{
				auto currentIntersection = intersectMap(currentNode, ray);
				if (currentIntersection && closestIntersection.t > currentIntersection.value().t)
				{
					closestIntersection = currentIntersection.value();
					intersected = true;
				}
			}
			else
			{
				if (currentNode.Child1ID != -1)
					nodesToCheck.push(currentNode.Child1ID);
				if (currentNode.Child2ID != -1)
					nodesToCheck.push(currentNode.Child2ID);
			}
		}
	}
	if(intersected)
		return closestIntersection;
	return {};
}

std::optional<Geometry::Intersection> AccelerationTree::intersectMap(ACTreenode& currentNode,Ray& ray)
{
	Geometry::Intersection closestIntersection;
	float minT;
	closestIntersection.t = FLT_MAX;
	int objID = 0;
	bool intersected = false;
	for (std::map<int, std::vector<Geometry::Triangle>>::iterator iter = currentNode.objects.begin(); iter != currentNode.objects.end(); ++iter)
	{
		for (auto& triangle : iter->second)
		{
			float t = triangle.Intersect(ray);
			if (t >= 0 && t < closestIntersection.t)
			{
				intersected = true;

				minT = t;
				int materialID = scene.sceneSettings.objects[iter->first].materialID;
				closestIntersection.material = &scene.sceneSettings.materials[materialID];
				closestIntersection.triangle = &triangle;
				closestIntersection.t = minT;
				closestIntersection.objectID = objID;
				Vector3 intersectionPoint = minT * ray.dir + ray.origin;
				Vector3 barycentricCoords = barycentricCoordinates(intersectionPoint, closestIntersection.triangle);
				closestIntersection.normal = closestIntersection.material->smooth_shading ? barycentricNorm(barycentricCoords, closestIntersection,scene) : closestIntersection.triangle->normal;
				closestIntersection.barycentric = barycentricCoords;

				//Only take 1st 2 coords of UV, since 3rd is always 0
				closestIntersection.uv = barycentricCoords.x * triangle.uv2 + barycentricCoords.y * triangle.uv3 + barycentricCoords.z * triangle.uv1;
			}
			objID++;
		}
		
	}
	if (intersected) {
		return closestIntersection;
	}
	return {};
}
