#define GLM_FORCE_RADIANS
#define GLM_ENABLE_EXPERIMENTAL
#include "GeomTriangle.h"

#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
#include <iostream>

#include "Intersection.h"
#include "Ray.h"

GeomTriangle::GeomTriangle(std::vector<glm::vec3> &vertices, std::vector<glm::vec3> &normals) {
    this->vertices[0] = vertices[0];
    this->vertices[1] = vertices[1];
    this->vertices[2] = vertices[2];

    this->normals[0] = normals[0];
    this->normals[1] = normals[1];
    this->normals[2] = normals[2];
}

std::vector<Intersection> GeomTriangle::intersect(Ray &ray) {
    using namespace glm;

    std::vector<Intersection> intersections;
    
    // Barycentric coordinates
    vec4 coords = inverse(mat4(vec4(vertices[0], 1.0), vec4(vertices[1], 1.0), vec4(vertices[2], 1.0), vec4(-ray.dir, 0.0))) * vec4(ray.p0, 1.0);

    // Add intersection with triangle
    if (coords[0] >= 0 && coords[1] >= 0 && coords[2] >= 0) {
        // Interpolated point and normal vectors
        vec3 point = coords[0] * vertices[0] + coords[1] * vertices[1] + coords[2] * vertices[2];
        vec3 normal = normalize(coords[0] * normals[0] + coords[1] * normals[1] + coords[2] * normals[2]);

        intersections.push_back({ length(point - ray.p0), point, normal, this, nullptr});
    }
    return intersections;
}