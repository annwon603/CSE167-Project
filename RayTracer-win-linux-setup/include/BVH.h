#ifndef __BVH_H__
#define __BVH_H__

#include <cmath>
#include <limits>
#include <glm/glm.hpp>
#include <vector>

using namespace glm;
using namespace std;

struct BVHTriangle {
	glm::vec3 Centre;
	glm::vec3 Min;
	glm::vec3 Max;
	glm::vec3 vertA;
	glm::vec3 vertB;
	glm::vec3 vertC;
	int Index;
	BVHTriangle(glm::vec3& centre, glm::vec3& min, glm::vec3& max, int& index);
	BVHTriangle(glm::vec3& a, glm::vec3& b, glm::vec3& c);
};

struct BoundBox {
	glm::vec3 min = glm::vec3(1.0f) * std::numeric_limits<float>::infinity();
	glm::vec3 max = glm::vec3(1.0f) * -std::numeric_limits<float>::infinity();
	glm::vec3 centre() const;

	void GrowToInclude(glm::vec3& point);

	void GrowToInclude(BVHTriangle& triangle);
};

struct BNode {
	BoundBox boundingBox;
	vector<BVHTriangle> triangles;
	int triangleIndex;
	int triangleCount;
	int childIndex;

	BNode(BoundBox boundingBox, vector<BVHTriangle> triangles) : boundingBox(boundingBox), triangles(triangles) { }
	BNode() { }
};

class BVH {
public:
	vector<BNode> nodes;
	vector<BVHTriangle> triangles;

	BVH(vector<vec3> vertices, vector<unsigned int> triangleIndices);
	BVH();

	void Split(BNode parent, int depth);
};

#endif