#ifndef OPENGLPLAYGROUND_VERTEX_H
#define OPENGLPLAYGROUND_VERTEX_H

#include <glm/glm.hpp>
#include <glm/vector_relational.hpp> // ref: https://stackoverflow.com/questions/46636721/how-do-i-use-glm-vector-relational-functions
#include <fmt/format.h>

#include <functional>

//#include "Utils.h"
//#include "Log.h"

struct Vertex
{
    glm::vec4 pos{};
    glm::vec4 color{};
    glm::vec3 normal{};

    Vertex() = default;
    Vertex(const Vertex &v)
    {
        pos = v.pos;
        color = v.color;
        normal = v.normal;
    }

    Vertex& operator=(const Vertex &rhs)
    {
        pos = rhs.pos;
        color = rhs.color;
        normal = rhs.normal;
        return *this;
    }

    bool operator==(const Vertex &rhs) const
    {
        return pos == rhs.pos &&
               color == rhs.color &&
               normal == rhs.normal;
    }

    bool operator<(const Vertex &rhs) const
    {
        if(glm::all(glm::lessThan(pos, rhs.pos)))
            return true;
        if(glm::all(glm::lessThan(rhs.pos, pos)))
            return false;
        if(glm::all(glm::lessThan(color, rhs.color)))
            return true;
        if(glm::all(glm::lessThan(rhs.color, color)))
            return false;
        return glm::all(glm::lessThan(normal, rhs.normal));
    }
};

template<>
struct fmt::formatter<Vertex> : fmt::formatter<std::string>
{
    template <typename FmtContext>
    auto format (Vertex v, FmtContext& ctx) {
        std::string str;
        str = fmt::format("Vertex: v {:.5f} {:.5f} {:.5f} n {:.5f} {:.5f} {:.5f}",
                          v.pos.x, v.pos.y, v.pos.z,
                          v.normal.x, v.normal.y, v.normal.z);
        return formatter<std::string>::format(str, ctx);
    }
};

template<>
struct std::hash<glm::vec4>
{
    std::size_t operator()(const glm::vec4& v) const
    {
        std::size_t h = 0;
//        Utils::hash_combine(h, v.x);
//        Utils::hash_combine(h, v.y);
//        Utils::hash_combine(h, v.z);
//        Utils::hash_combine(h, v.w);
//        INFO_TAG("hash glm::vec4", "{} -> {}", (void*)&v, h);
        return h;
    }
};

template<>
struct std::hash<glm::vec3>
{
    std::size_t operator()(const glm::vec3& v) const
    {
        std::size_t h = 0;
//        Utils::hash_combine(h, v.x);
//        Utils::hash_combine(h, v.y);
//        Utils::hash_combine(h, v.z);
//        INFO_TAG("hash glm::vec3", "{} -> {}", (void*)&v, h);
        return h;
    }
};

template<>
struct std::hash<Vertex>
{
    std::size_t operator()(const Vertex& v) const
    {
        std::size_t h = 0;
//        Utils::hash_combine(h, v.pos);
//        Utils::hash_combine(h, v.normal);
//        Utils::hash_combine(h, v.color);
//        INFO_TAG("hash Vertex", "{} -> {}", (void*)&v, h);
        return h;
    }
};

#endif //OPENGLPLAYGROUND_VERTEX_H
