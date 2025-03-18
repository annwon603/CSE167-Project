#include "GeomSphere.h"

#include <iostream>
#include <utility>

#include "Intersection.h"
#include "Ray.h"

std::vector<Intersection> GeomSphere::intersect(Ray &ray) {
    /**
     * NOTE: Ray is already transformed to the Model coordinate space.
     */
    using namespace glm;

    // vector to store the intersections
    std::vector<Intersection> intersections;

    /**
     * TODO: Implement the Ray intersection with the current geometry
     */

    double t = 0.0f;
    vec3 d = glm::normalize(ray.dir);

    double addEnd1 = dot(-d , (ray.p0 - center));
    double expression = pow(dot(d, ray.p0 - center), 2) - pow(length(ray.p0 - center),2) + pow(radius, 2);

    if (expression < 0) {
        return intersections;
    }

    if (expression == 0)
    {
        t = addEnd1;
        vec3 point = ray.p0 + static_cast<float>(t) * d;
        vec3 normal = glm::normalize(point - center);
        intersections.push_back({ static_cast<float>(t), point, normal, this, nullptr });
    }
    else {
        double square = sqrt(expression);
        double posSum = addEnd1 + square;
        double negSum = addEnd1 - square;
        double tCurrent = (posSum < negSum) ? posSum : negSum;
        vec3 point = ray.p0 + static_cast<float>(tCurrent) * d;
        vec3 normal = glm::normalize(point - center);
        intersections.push_back({ static_cast<float>(tCurrent) , point, normal, this , nullptr });

    }

   /* float expression = pow(dot(d, ray.p0 - center), 2) - pow(length(ray.p0 - center), 2) + pow(radius, 2);

    float tQuadPos = dot(-d, (ray.p0 - center)) + sqrt(expression);
    float tQuadNeg = dot(-d, (ray.p0 - center)) - sqrt(expression);

    if (expression >= 0) {
        float tCurrent = (tQuadPos < tQuadNeg) ? tQuadPos : tQuadNeg;
        vec3 point = ray.p0 + tCurrent * ray.dir;
        vec3 normal = glm::normalize(point - center);
        intersections.push_back({ tCurrent, point, normal, this, nullptr });
    }*/

    /**
     * Once you find the intersection, add it to the `intersections` vector.
     *
     * Example:
     * Suppose the ray intersects the current geometry at a point `vec3 point`
     * at a distance `float t`, and the unit normal vector at the intersection
     * point is `vec3 normal`. You would then insert an intersection into the
     * vector as follows:
     *
     * intersections.push_back({t, point, normal, this, nullptr});
     *
     * Note:
     * - Here we pass the Model pointer as `nullptr` because it will be
     *   populated by the Model::intersect() function call.
     * - Only add intersections that are in front of the camera, i.e.,
     *   where t > 0.
     */

    /**
     * TODO: Update `intersections`
     */

    return intersections;
};