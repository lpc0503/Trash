#include "RendererAPI.h"
#include "Entity.h"
#include "Camera.h"

//#include "Utils.h"
#include "common/shader.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/matrix_decompose.hpp>

void RendererAPI::Init()
{
    glGenVertexArrays(1, &m_LineVAO);
    glBindVertexArray(m_LineVAO);
    glGenBuffers(1, &m_LineVBO);
    glBindBuffer(GL_ARRAY_BUFFER, m_LineVBO);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(float) * 4));
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(float) * 8));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glBindVertexArray(0);
    m_LineShader = LoadShaders("StandardShading.vert", "StandardShading.frag");

    glGenVertexArrays(1, &m_MeshVAO);
    glBindVertexArray(m_MeshVAO);
    glGenBuffers(1, &m_MeshVBO);
    glBindBuffer(GL_ARRAY_BUFFER, m_MeshVBO);
    glGenBuffers(1, &m_MeshIBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_MeshIBO);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(float) * 4));
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(float) * 8));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glBindVertexArray(0);
    m_MeshShader = LoadShaders("StandardShading.vert", "StandardShading.frag");

    m_PickingShader = LoadShaders("Picking.vert", "Picking.frag");
}

void RendererAPI::Shutdown()
{
    glDeleteBuffers(1, &m_LineVBO); // TODO: delete IBO?
    glDeleteVertexArrays(1, &m_LineVAO);
    glDeleteProgram(m_LineShader);

    glDeleteBuffers(1, &m_MeshVBO);
    glDeleteBuffers(1, &m_MeshIBO);
    glDeleteVertexArrays(1, &m_MeshVAO);
    glDeleteProgram(m_MeshShader);
}

void RendererAPI::SetMatrix(const std::string &name, const glm::mat4 &mat)
{
    auto id = GetUniformID(name);
    glUniformMatrix4fv(id, 1, GL_FALSE, glm::value_ptr(mat));
}

void RendererAPI::SetFloat(const std::string &name, const float f)
{
    auto id = GetUniformID(name);
    glUniform1f(id, f);
}

void RendererAPI::SetFloat3(const std::string &name, const glm::vec3 &v)
{
    auto id = GetUniformID(name);
    glUniform3f(id, v.x, v.y, v.z);
}

void RendererAPI::SetFloat4(const std::string &name, const glm::vec4 &v)
{
    auto id = GetUniformID(name);
    glUniform4f(id, v.x, v.y, v.z, v.w);
}

void RendererAPI::SetBool(const std::string &name, bool b)
{
    auto id = GetUniformID(name);
    glUniform1i(id, b);
}

GLint RendererAPI::GetUniformID(const std::string &name)
{
    auto id = glGetUniformLocation(m_CurrentShader, name.c_str());
//    ASSERT(id >= 0, "Invalid shader uniform name");
    return id;
}

void RendererAPI::ClearRendererState()
{
    m_LineVertices.clear();
    m_Meshes.clear();
}

//////////////////////////////////////////////////////////////////////
// Line
//////////////////////////////////////////////////////////////////////

void RendererAPI::SendLineData()
{
    glBindVertexArray(m_LineVAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_LineVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * m_LineVertices.size(), glm::value_ptr(m_LineVertices[0].pos), GL_STATIC_DRAW);
}

void RendererAPI::PushLine(const glm::vec3 &p0, const glm::vec3 &p1, const glm::vec4 &color)
{
    Vertex v0, v1;
    v0.pos = glm::vec4(p0, 1.f);
    v0.color = color;
    v0.normal = glm::vec3{0.f};
    m_LineVertices.emplace_back(v0);

    v1.pos = glm::vec4(p1, 1.f);
    v1.color = color;
    v1.normal = glm::vec3{0.f};
    m_LineVertices.emplace_back(v1);
}

void RendererAPI::DrawLines()
{
    glDrawArrays(GL_LINES, 0, m_LineVertices.size());
}

//////////////////////////////////////////////////////////////////////
// Mesh
//////////////////////////////////////////////////////////////////////

void RendererAPI::PushMesh(Ref <Mesh> mesh, const glm::vec3 &pos, const glm::vec3 &rotate, const glm::vec3 &scale, const glm::vec4 &tint)
{
    m_Meshes.emplace_back(MeshData{mesh, pos, rotate, scale, tint});
}

void RendererAPI::SendMeshData()
{
    /*glBindVertexArray(m_LineVAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_LineVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * m_LineVertices.size(), glm::value_ptr(m_LineVertices[0].pos), GL_STATIC_DRAW);*/
}

void RendererAPI::DrawMeshs()
{
    auto SendModelMatrix = [&](const glm::vec3 &pos, const glm::vec3 &rotate, const glm::vec3 &scale, const glm::vec4 &tint)
    {
        glm::mat4 m{1.f};
        m = glm::translate(m, pos);
        m = glm::rotate(m, glm::radians(rotate.x), {1.f, 0.f, 0.f});
        m = glm::rotate(m, glm::radians(rotate.y), {0.f, 1.f, 0.f});
        m = glm::rotate(m, glm::radians(rotate.z), {0.f, 0.f, 1.f});
        m = glm::scale(m, scale);
        SetMatrix("M", m);
        SetFloat4("Color", tint);
    };
    auto DrawMesh = [&](Ref<Mesh> mesh)
    {
        glBindVertexArray(m_MeshVAO);
        glBindBuffer(GL_ARRAY_BUFFER, m_MeshVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * mesh->m_Vertices.size(), glm::value_ptr(mesh->m_Vertices[0].pos), GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_MeshIBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * mesh->m_Indices.size(), &mesh->m_Indices[0], GL_STATIC_DRAW);
        glDrawElements(GL_TRIANGLES, mesh->m_Indices.size(), GL_UNSIGNED_INT, 0);
    };

    BindMeshShader();
    SendLightData();
    for(MeshData &md : m_Meshes)
    {
        SendModelMatrix(md.pos, md.rotate, md.scale, md.tint);
        DrawMesh(md.mesh);
    }
    UnbindShader();
}

//////////////////////////////////////////////////////////////////////
// Light
//////////////////////////////////////////////////////////////////////

void RendererAPI::PushDirectionalLight(const glm::vec3 &dir, const glm::vec4 &color)
{
    m_DirectionalLight.dir = dir;
    m_DirectionalLight.color = color;
}

void RendererAPI::SendLightData()
{
    SetFloat3("dirLight.dir", m_DirectionalLight.dir);
    SetFloat3("dirLight.ambient", {m_DirectionalLight.color.x, m_DirectionalLight.color.y, m_DirectionalLight.color.z});
    SetFloat3("dirLight.diffuse", {m_DirectionalLight.color.x, m_DirectionalLight.color.y, m_DirectionalLight.color.z});
}

//////////////////////////////////////////////////////////////////////
// Entity
//////////////////////////////////////////////////////////////////////

void RendererAPI::DrawEntitiesForPicking(Ref<Camera> camera)
{
    auto SendModelMatrix = [&](const glm::vec3 &pos, const glm::vec3 &rotate, const glm::vec3 &scale)
    {
        glm::mat4 m{1.f};
        m = glm::translate(m, pos);
        m = glm::rotate(m, glm::radians(rotate.x), {1.f, 0.f, 0.f});
        m = glm::rotate(m, glm::radians(rotate.y), {0.f, 1.f, 0.f});
        m = glm::rotate(m, glm::radians(rotate.z), {0.f, 0.f, 1.f});
        m = glm::scale(m, scale);
        m = camera->GetProjection() * camera->GetView() * m; // P * V * M
        SetMatrix("MVP", m);
    };
    auto DrawMesh = [&](Ref<Mesh> mesh)
    {
        glBindVertexArray(m_MeshVAO);
        glBindBuffer(GL_ARRAY_BUFFER, m_MeshVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * mesh->m_Vertices.size(), glm::value_ptr(mesh->m_Vertices[0].pos), GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_MeshIBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * mesh->m_Indices.size(), &mesh->m_Indices[0], GL_STATIC_DRAW);
        glDrawElements(GL_TRIANGLES, mesh->m_Indices.size(), GL_UNSIGNED_INT, 0);
    };

    for(auto entity : m_Entities)
    {
        glm::vec3 pos, rotate, scale;
        entity->transform.GetRTS(pos, rotate, scale);
        SendModelMatrix(pos, rotate, scale);
        SetFloat("PickingID", entity->id);
        DrawMesh(entity->mesh);
    }
}

void RendererAPI::ClearViewport()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

