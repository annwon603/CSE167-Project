#ifndef __OBJ_H__
#define __OBJ_H__

#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

#include "GeomTriangle.h"
#include "ModelBase.h"
#include "BVH.h"

class Obj : public ModelBase {
   public:
    Obj(const char* filename, std::shared_ptr<MaterialBase> mat) {
        // Material Object can be shared between multiple geometries
        this->material = mat;

        // Load triangle soup
        loadObj(filename);
    }
    BVH bvh;

   private:
    void loadObj(const char* filename) {
        std::vector<glm::vec3> temp_vertices;
        std::vector<glm::vec3> temp_normals;
        std::vector<unsigned int> vertexIndices, normalIndices;
        std::cout << filename << std::endl;
        std::ifstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Cannot open file: " << filename << std::endl;
            exit(-1);
        }
        std::cout << "Loading " << filename << "..." << std::endl;

        std::string line;
        while (std::getline(file, line)) {
            std::istringstream ss(line);
            std::string prefix;
            ss >> prefix;

            if (prefix == "v") {
                glm::vec3 vertex;
                ss >> vertex.x >> vertex.y >> vertex.z;
                temp_vertices.push_back(vertex);
            } else if (prefix == "vn") {
                glm::vec3 normal;
                ss >> normal.x >> normal.y >> normal.z;
                temp_normals.push_back(normal);
            } else if (prefix == "f") {
                unsigned int vIndex[3], nIndex[3];
                char slash;
                for (int i = 0; i < 3; i++) {
                    ss >> vIndex[i] >> slash >> slash >> nIndex[i];
                }
                for (int i = 0; i < 3; i++) {
                    vertexIndices.push_back(vIndex[i] - 1);
                    normalIndices.push_back(nIndex[i] - 1);
                }
            }
        }
        file.close();

        std::cout << "Processing data..." << std::endl;
        for (size_t i = 0; i < vertexIndices.size(); i += 3) {
            std::vector<glm::vec3> triVertices = {
                temp_vertices[vertexIndices[i]],
                temp_vertices[vertexIndices[i + 1]],
                temp_vertices[vertexIndices[i + 2]]};
            std::vector<glm::vec3> triNormals = {
                temp_normals[normalIndices[i]],
                temp_normals[normalIndices[i + 1]],
                temp_normals[normalIndices[i + 2]]};
            geometries.push_back(std::make_unique<GeomTriangle>(triVertices, triNormals));
        }

        bvh = BVH(temp_vertices, vertexIndices);

        std::cout << "Done loading and processing. " << geometries.size() << std::endl;
    }

    bool intersect(Ray& ray) override {
        bool isIntersect = false;

        // Cache ray parameters
        vec3 p0(ray.p0);
        vec3 dir(ray.dir);

        // Transform Ray to model space
        ray.p0 = vec3(inverse_transform_matrix * vec4(ray.p0, 1.0f));
        ray.dir = normalize(vec3(inverse_transform_matrix * vec4(ray.dir, 0.0f)));

        // TODO skip stuff below if bounding box isn't encountered

        for (int idx = 0; idx < geometries.size(); idx++) {
            GeometryBase* prim = geometries[idx].get();  // get the raw pointer

            // intersections in model space
            std::vector<Intersection> intersections = prim->intersect(ray);

            // Transform intersection points to world space
            for (int i = 0; i < intersections.size(); i++) {
                intersections[i].model = this;
                intersections[i].point = vec3(transformation_matrix * vec4(intersections[i].point, 1.0f));
                intersections[i].normal = normalize(vec3(transpose(inverse_transform_matrix) * vec4(intersections[i].normal, 0.0f)));
                intersections[i].t = length(intersections[i].point - p0);
            }

            // Update ray intersections
            ray.intersections.insert(ray.intersections.end(), intersections.begin(), intersections.end());

            // Update isIntersect
            isIntersect = isIntersect || !intersections.empty();
        }

        // Restore ray parameters
        ray.p0 = p0;
        ray.dir = dir;

        return isIntersect;
    }

    glm::vec3 get_surface_point() override {
        return glm::vec3(0.0f);  // Placeholder implementation
    }
};

#endif
