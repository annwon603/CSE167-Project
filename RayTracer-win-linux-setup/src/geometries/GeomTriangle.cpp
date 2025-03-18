#define GLM_FORCE_RADIANS
#define GLM_ENABLE_EXPERIMENTAL
#include "GeomTriangle.h"

#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
#include <iostream>

#include "Intersection.h"
#include "Ray.h"

GeomTriangle::GeomTriangle(std::vector<glm::vec3>& vertices, std::vector<glm::vec3>& normals) {
    this->vertices[0] = vertices[0];
    this->vertices[1] = vertices[1];
    this->vertices[2] = vertices[2];

    this->normals[0] = normals[0];
    this->normals[1] = normals[1];
    this->normals[2] = normals[2];
}

std::vector<Intersection> GeomTriangle::intersect(Ray& ray) {
    using namespace glm;
    
    std::vector<Intersection> intersections;

    mat4 transformationMatrix(vec4(vertices[0], 1.0), vec4(vertices[1], 1.0), vec4(vertices[2], 1.0), vec4(-ray.dir, 0.0));
    vec4 rayCoords(ray.p0, 1.0);

    vec4 intersectionCoords = inverse(transformationMatrix) * rayCoords;
    vec3 lambdas(intersectionCoords[0], intersectionCoords[1], intersectionCoords[2]);

    double t = intersectionCoords[3];

    if (lambdas[0] >= 0 && lambdas[1] >= 0 && lambdas[2] >= 0 && t >= 0) {
        vec3 intersectionPoint = lambdas[0] * vertices[0] + lambdas[1] * vertices[1] + lambdas[2] * vertices[2];
        vec3 intersectionNormal = normalize(lambdas[0] * normals[0] + lambdas[1] * normals[1] + lambdas[2] * normals[2]);

        intersections.push_back({static_cast<float>(t), intersectionPoint, intersectionNormal, this, nullptr});
    }

    return intersections;
}