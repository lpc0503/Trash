#pragma once
#ifndef OPENGLPLAYGROUND_MESH_H
#define OPENGLPLAYGROUND_MESH_H

#include <vector>
#include "Vertex.h"

class Mesh
{
public:
    std::vector<Vertex> m_Vertices;
    std::vector<uint32_t> m_Indices;
    std::string m_Name;
//    std::vector<Texture2D*> m_Textures; // TODO: add support for texture

public:
    Mesh() = default;
    Mesh(const std::vector<Vertex> &vertices, const std::vector<uint32_t> &indices/*, const std::vector<Texture2D*> &textures*/);
};

#endif //OPENGLPLAYGROUND_MESH_H
