#include "BVH.h"
#include <iostream>

// Define global variables
int maxDepth = 10;

BVHTriangle::BVHTriangle(glm::vec3 a, glm::vec3 b, glm::vec3 c, int idx) {
	vec3 average = (a + b + c) / 3.0f;
	
	center = average;
	min = glm::min(glm::min(a, b), c);
	max = glm::max(glm::max(a, b), c);
	
	index = idx;
}

vec3 BoundBox::centre() const {
	return (min + max) * 0.5f;
}

void BoundBox::GrowToInclude(glm::vec3 pointMin, glm::vec3 pointMax) {
	if (hasPoint) {
		min.x = glm::min(pointMin.x, min.x);
		min.y = glm::min(pointMin.y, min.y);
		min.z = glm::min(pointMin.z, min.z);
		max.x = glm::max(pointMax.x, max.x);
		max.y = glm::max(pointMax.y, max.y);
		max.z = glm::max(pointMax.z, max.z);
	}
	else {
		hasPoint = true;
		min = pointMin;
		max = pointMax;
	}
}

void BoundBox::GrowToInclude(BVHTriangle& triangle)
{
	GrowToInclude(triangle.min, triangle.max);
	GrowToInclude(triangle.min, triangle.max);
	GrowToInclude(triangle.min, triangle.max);
}

BVH::BVH(vector<vec3> vertices, vector<unsigned int> indices) {
	nodes = vector<BNode>();	// Every node in this model's BVH
	triangles = vector<BVHTriangle>();	// Every triangle in this model

	BoundBox boundingBox;

	// Go through every triangle (by vertices, hence += 3)
	for (int i = 0; i < indices.size(); i += 3) {	
		vec3 a = vertices[indices[i]];
		vec3 b = vertices[indices[i + 1]];
		vec3 c = vertices[indices[i + 2]];
		
		// Recreate triangle for BVH
		BVHTriangle tri(a, b, c, i / 3.0f); // Also for index, remember one in geometries since these BVH ones swap
		
		triangles.push_back(tri);

		boundingBox.GrowToInclude(tri.min, tri.max);
	}

	BNode root(boundingBox);	// Root node for BVH graph I suppose
	Split(root, 0);	// Split until max depth
}

void BVH::Split(BNode parent, int depth) {
	if (depth == maxDepth) return;	// Depth is how many splits we want

	// Determine split direction
	vec3 size = parent.boundingBox.max - parent.boundingBox.min;
	int splitAxis;
	if (size.x > glm::max(size.y, size.z)) {
		splitAxis = 0;	// Bounds split across the x-axis
	}
	else if (size.y > glm::max(size.x, size.z)) {
		splitAxis = 1;	// Bounds split across y-axis
	}
	else {
		splitAxis = 2;	// Bounds split across z-axis
	}

	// Create two new smaller children
	parent.childIndex = nodes.size();
	BNode childA, childB;
	childA.triangleIndex = parent.triangleIndex;
	childB.triangleIndex = parent.triangleIndex;
	nodes.push_back(childA);
	nodes.push_back(childB);

	// Loop through every parent triangle and assign to children
	for (int i = parent.triangleIndex; i < parent.triangleIndex + parent.triangleCount; i++) {
		// Which triangles belong to A, rest go to B
		bool isSideA = triangles[i].center[splitAxis] < parent.boundingBox.centre()[splitAxis];
		
		BNode child;
		if (isSideA) child = childA;
		else child = childB;
		child.boundingBox.GrowToInclude(triangles[i]);	// Add all triangle vertices to bounds
		child.triangleCount++;

		// Swap triangle values around because node triangles in array have to be contiguous
		if (isSideA) {
			int swap = child.triangleIndex + child.triangleCount - 1;
			BVHTriangle savedI = triangles[i];
			triangles[i] = triangles[swap];
			triangles[swap] = savedI;
			childB.triangleIndex++;
		}
	}

	// Further split child nodes until depth reached
	Split(childA, depth + 1);
	Split(childB, depth + 1);
}
