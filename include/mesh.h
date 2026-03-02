#ifndef MESH_H
#define MESH_H

#include "tiny_obj_loader.h"
#include <vector>
#include <string>
#include <iostream>

std::vector<float> loadOBJ(const std::string& path)
{
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string warn, err;

    bool ok = tinyobj::LoadObj(
        &attrib, &shapes, &materials,
        &warn, &err, path.c_str());

    if (!warn.empty()) std::cout << warn << std::endl;
    if (!err.empty()) std::cerr << err << std::endl;
    if (!ok) throw std::runtime_error("Failed to load OBJ");

    std::vector<float> vertices;

    for (const auto& shape : shapes)
    {
        for (const auto& index : shape.mesh.indices)
        {
            // position
            vertices.push_back(attrib.vertices[3 * index.vertex_index + 0]);
            vertices.push_back(attrib.vertices[3 * index.vertex_index + 1]);
            vertices.push_back(attrib.vertices[3 * index.vertex_index + 2]);

            // normal (assumes normals exist)
            if (!attrib.normals.empty())
            {
                vertices.push_back(attrib.normals[3 * index.normal_index + 0]);
                vertices.push_back(attrib.normals[3 * index.normal_index + 1]);
                vertices.push_back(attrib.normals[3 * index.normal_index + 2]);
            }
            else
            {
                // fallback normal if none provided
                vertices.push_back(0.0f);
                vertices.push_back(1.0f);
                vertices.push_back(0.0f);
            }
        }
    }

    return vertices;
}

#endif // MESH_H
