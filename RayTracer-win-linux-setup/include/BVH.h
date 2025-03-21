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

float pos_inf = std::numeric_limits<float>::infinity();
float neg_inf = -std::numeric_limits<float>::infinity();
struct BVHTriangle {
	glm::vec3 Centre;
	glm::vec3 Min;
	glm::vec3 Max;
	glm::vec3 vertA;
	glm::vec3 vert
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

}

#endif