#pragma once
#include "Scene.h"
#include "Geometry.h"
#include "Utils.h"
#include <queue>

class AccelerationTree {

public: 
	struct ACTreenode {
		int Child1ID;
		int Child2ID;
		Geometry::AABB AABB;
		std::map<int,std::vector<Geometry::Triangle>> objects; //To be able to refer to materials, and 
																			 //without refactoring the whole renderer object representation
	};

	std::vector<ACTreenode> nodes;
	Scene scene;
	ACTreenode root;
	int maxTriangles = 1;
	int maxDepth = 10;

	AccelerationTree(Scene&);
	
	std::optional<Geometry::Intersection> Traverse(Ray&);

private:
	void BuildTree(int, int, std::map<int, std::vector<Geometry::Triangle>>&);
	
	int countNodeTriangles(std::map<int, std::vector<Geometry::Triangle>>&);

	std::optional<Geometry::Intersection> intersectMap(ACTreenode&,Ray&);
};
 