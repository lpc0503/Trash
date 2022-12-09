#define TINYOBJLOADER_IMPLEMENTATION
#include "Mesh.h"

Mesh::Mesh(const std::vector<Vertex> &vertices, const std::vector<uint32_t> &indices)
    : m_Vertices(vertices),
      m_Indices(indices)
{
}
