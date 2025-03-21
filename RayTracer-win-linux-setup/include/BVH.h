#ifndef __BVH_H__
#define __BVH_H__

#include <cmath>
#include <limits>
#include <glm/glm.hpp>
#include "Ray.h"
#include "Obj.h"
#include "Intersection.h"
#include "RayTracer.h"
#include "ModelBase.h"

using namespace glm;
using namespace std;

float pos_inf = std::numeric_limits<float>::infinity();
float neg_inf = -std::numeric_limits<float>::infinity();
struct BVHTriangle {
	glm::vec3 Centre;
	glm::vec3 Min;
	glm::vec3 Max;
	glm::vec3 vertA;
	glm::vec3 vertB;
	glm::vec3 vertC;
	int Index;
	BVHTriangle(glm::vec3& centre, glm::vec3& min, glm::vec3& max, int& index)
	{
		Centre = centre;
		Min = min;
		Max = max;
		Index = index;
	}
};
struct BoundBox {
	glm::vec3 min = glm::vec3(1.0f) * pos_inf;
	glm::vec3 max = glm::vec3(1.0f) * neg_inf;
	glm::vec3 centre() const {
		return (min + max) * 0.5f;
	}


	void GrowToInclude(glm::vec3& point) {
		min = glm::min(min, point);
		max = glm::max(max, point);
	}

	void GrowToInclude(BVHTriangle& triangle)
	{
		GrowToInclude(triangle.vertA);
		GrowToInclude(triangle.vertB);
		GrowToInclude(triangle.vertC);
	}
};

struct Node {
	BoundBox boundingBox;
	vector<BVHTriangle> triangles;
	Node* childLeft;
	Node* childRight;

	Node(BoundBox boundingBox, vector<BVHTriangle> triangles) : boundingBox(boundingBox), triangles(triangles) { }
};

class BVH {
public:
	BVH(vector<vec3> vertices, vector<int> triangleIndices) {
		BoundBox bb;

		for (vector<vec3>::iterator itr = vertices.begin(); itr != vertices.end(); ++itr) {
			 bb.GrowToInclude(*itr);
		}

		vector<BVHTriangle> triangles;

		for (int i = 0; i < triangleIndices.size(); i++) {
			vec3 a = vertices[triangleIndices[i]];
			vec3 b = vertices[triangleIndices[i + 1]];
			vec3 c = vertices[triangleIndices[i + 2]];
			//triangles.push_back(BVHTriangle(vector<vec3>({a, b, c}), vector<vec3>()));
		}

		Node root(bb, triangles);
		Split(root, 0);
	}

	void Split(Node parent, int depth) {
		//if (depth == maxDepth) return;

		for (vector<BVHTriangle>::iterator itr = parent.triangles.begin(); itr != parent.triangles.end(); ++itr) {
			/*bool inA = (*itr).Centre < parent.boundingBox.centre.x;
			Node child;
			if (inA) child = parent.childLeft;
			else child = parent.childRight;
			child.triangles.push_back(*itr);
			child.boundingBox.growToInclude(*itr);

			Split(parent.childLeft, depth + 1);
			Split(parent.childRight, depth + 1);*/
		}
	}
};

#endif