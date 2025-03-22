#ifndef __BVH_H__
#define __BVH_H__

#include <cmath>
#include <limits>
#include <glm/glm.hpp>
#include <vector>

using namespace glm;
using namespace std;

struct BVHTriangle {
	vec3 center;
	vec3 min;
	vec3 max;
	int index;

	BVHTriangle(vec3 a, vec3 b, vec3 c, int idx);
};

struct BoundBox {
	vec3 min;
	vec3 max;
	vec3 centre() const;
	bool hasPoint;

	void GrowToInclude(vec3 pointMin, vec3 pointMax);
	void GrowToInclude(BVHTriangle& triangle);

	BoundBox() {
		hasPoint = false;
		min = vec3(1.0f) * std::numeric_limits<float>::infinity();
		max = vec3(1.0f) * -std::numeric_limits<float>::infinity();
	}
};

struct BNode {
	BoundBox boundingBox;
	int triangleIndex;	// Where first triangle is stored in all triangles
	int triangleCount;	// How many triangles are there (must be contiguous)
	int childIndex;	// Needed to reference both children nodes

	BNode(BoundBox boundingBox) : boundingBox(boundingBox), triangleIndex(0), triangleCount(0), childIndex(0) {}
	BNode() : triangleIndex(0), triangleCount(0), childIndex(0) { }
};

class BVH {
public:
	vector<BNode> nodes;	// List of every node in the BVH
	vector<BVHTriangle> triangles;	// List of every triangle in the model

	BVH(vector<vec3> vertices, vector<unsigned int> indices);

	void Split(BNode parent, int depth);	// How we split a node into two smaller ones
};

#endif