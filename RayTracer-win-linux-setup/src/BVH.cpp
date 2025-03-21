#include "BVH.h"

// Define global variables
float pos_inf = std::numeric_limits<float>::infinity();
float neg_inf = -std::numeric_limits<float>::infinity();
int maxDepth = 10;

BVHTriangle::BVHTriangle(glm::vec3& a, glm::vec3& b, glm::vec3& c) : vertA(a), vertB(b), vertC(c) {
	vertA = a;
	vertB = b;
	vertC = c;
}
BVHTriangle::BVHTriangle(glm::vec3& centre, glm::vec3& min, glm::vec3& max, int& index)
{
	Centre = centre;
	Min = min;
	Max = max;
	Index = index;
}

void BoundBox::GrowToInclude(glm::vec3& point) {
	min = glm::min(min, point);
	max = glm::max(max, point);
}

void BoundBox::GrowToInclude(BVHTriangle& triangle)
{
	GrowToInclude(triangle.vertA);
	GrowToInclude(triangle.vertB);
	GrowToInclude(triangle.vertC);
}

BVH::BVH(vector<vec3> vertices, vector<int> triangleIndices) {
    BoundBox bb;

    for (vec3& vertex : vertices) {
        bb.GrowToInclude(vertex);
    }

    vector<BVHTriangle> triangles;

    for (size_t i = 0; i < triangleIndices.size(); i += 3) {
        vec3 a = vertices[triangleIndices[i]];
        vec3 b = vertices[triangleIndices[i + 1]];
        vec3 c = vertices[triangleIndices[i + 2]];
        triangles.emplace_back(a, b, c);
    }

    BNode root;
    root.boundingBox = bb;
    root.triangles = triangles;
    Split(root, 0);
}

void BVH::Split(BNode& parent, int depth) {
	if (depth == maxDepth) return;

	parent.childIndex = nodes.size();
	BNode childLeft;
	childLeft.triangleIndex = parent.triangleIndex;
	BNode childRight;
	childRight.triangleIndex = parent.triangleIndex;
	nodes.push_back(childLeft);
	nodes.push_back(childRight);

	for (int i = parent.triangleIndex; i < parent.triangleIndex + parent.triangleCount; i++) {
		bool inA = triangles[i].Centre.x < parent.boundingBox.centre().x;

		BNode child;
		if (inA) { child = childLeft; }
		else { child = childRight; }
		child.boundingBox.GrowToInclude(triangles[i]);
		child.triangleCount++;

		if (inA) {
			int swap = child.triangleIndex + child.triangleCount - 1;
			BVHTriangle tempTriangle = triangles[i];
			triangles[i] = triangles[swap];
			triangles[swap] = tempTriangle;
			childLeft.triangleIndex++;
		}

		Split(childLeft, depth + 1);
		Split(childRight, depth + 1);
}