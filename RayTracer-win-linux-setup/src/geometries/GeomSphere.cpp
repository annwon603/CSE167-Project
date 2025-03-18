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

    highp_dvec3 d = normalize(ray.dir);
    highp_dvec3 p0 = ray.p0;
    highp_dvec3 c = center;

    double expression = pow(dot(d, p0 - c), 2.0) - pow(length(p0 - c), 2.0) + pow(radius, 2.0);

    double t = dot(-d, (p0 - c));
    double tQuadPos = t + sqrt(expression);
    double tQuadNeg = t - sqrt(expression);

    if (expression >= 0.0 && t >= 0) {
        double tCurrent = (tQuadPos < tQuadNeg) ? tQuadPos : tQuadNeg;
        highp_dvec3 point = p0 + tCurrent * d;
        highp_dvec3 normal = normalize(point - c);
        intersections.push_back({ static_cast<float>(tCurrent), point, normal, this, nullptr});
    }

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