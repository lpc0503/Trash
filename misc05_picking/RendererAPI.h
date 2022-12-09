#pragma once
#ifndef OPENGLPLAYGROUND_RENDERERAPI_H
#define OPENGLPLAYGROUND_RENDERERAPI_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <memory>
#include <vector>

#include "Vertex.h"
#include "Mesh.h"
#include "Core.h"

class Camera;
class Entity;

class RendererAPI
{
public:
    void Init();
    void Shutdown();

    void BindShader() const { glUseProgram(m_CurrentShader); }
    void UnbindShader() { glUseProgram(0); m_CurrentShader = 0; } // TODO: remove
    void ClearRendererState();

    void SetMatrix(const std::string &name, const glm::mat4 &mat);
    void SetFloat(const std::string &name, const float f);
    void SetFloat3(const std::string &name, const glm::vec3 &v);
    void SetFloat4(const std::string &name, const glm::vec4 &v);
    void SetBool(const std::string &name, bool b);

    void ClearViewport();

private:
    GLuint m_CurrentShader;

    GLint GetUniformID(const std::string &name);

    // Line
public:
    void BindLineShader() { assert(m_LineShader); m_CurrentShader = m_LineShader; BindShader(); } // TODO: remove
    void PushLine(const glm::vec3 &p0, const glm::vec3 &p1, const glm::vec4 &color);
    void SendLineData();
    void DrawLines();
private:
    GLuint m_LineVAO, m_LineVBO, m_LineIBO;
    GLuint m_LineShader;
    std::vector<Vertex> m_LineVertices;
    //

    // Mesh
public:
    void BindMeshShader() { assert(m_MeshShader); m_CurrentShader = m_MeshShader; BindShader(); }
    void PushMesh(Ref<Mesh> mesh, const glm::vec3 &pos, const glm::vec3 &rotate, const glm::vec3 &scale, const glm::vec4 &tint = {1.f, 1.f, 1.f, 1.f});
    void SendMeshData();
    void DrawMeshs();

private:
    GLuint m_MeshShader;
    GLuint m_MeshVAO, m_MeshVBO, m_MeshIBO;
    struct MeshData {
        Ref<Mesh> mesh;
        glm::vec3 pos;
        glm::vec3 rotate;
        glm::vec3 scale;
        glm::vec4 tint;
    };
    std::vector<MeshData> m_Meshes;

    // Light
public:
//    void BindLightShader() { assert(m_LightShader); m_CurrentShader = m_LightShader; BindShader(); }
    void PushPointLight(const glm::vec3 &pos, const glm::vec4 &color, float intensity);
    void PushDirectionalLight(const glm::vec3 &dir, const glm::vec4 &color);
    void SendLightData();

    struct DirectionalLightData {
        glm::vec3 dir;
        glm::vec4 color;
    };
    DirectionalLightData m_DirectionalLight;
    static constexpr size_t MaxNumLight = 10;

    // Picking
public:
    void BindPickingShader() { assert(m_PickingShader); m_CurrentShader = m_PickingShader; BindShader(); }
private:
    GLuint m_PickingShader;

    // Entity
public:
    void PushPickingEntity(Ref<Entity> entity) { m_Entities.push_back(entity); }
    void DrawEntitiesForPicking(Ref<Camera> camera);
private:
    std::vector<Ref<Entity>> m_Entities;
};

#endif //OPENGLPLAYGROUND_RENDERERAPI_H
