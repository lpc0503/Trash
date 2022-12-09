#pragma once
#ifndef OPENGLPLAYGROUND_CAMERA_H
#define OPENGLPLAYGROUND_CAMERA_H

#include <glm/glm.hpp>

class Camera
{
public:
    Camera();
    Camera(const glm::mat4 &projection);

public:
    void LookAt(float x, float y, float z);
    void Move(float x, float y, float z);

public:
    const glm::mat4& GetProjection()       { return m_Projection; }
    const glm::mat4& GetView()             { return m_View; }
    float GetX() { return m_Position.x; }
    float GetZ() { return m_Position.y; }
    float GetY() { return m_Position.z; }
    glm::vec3 GetDir() { return m_Dir - m_Position; }
    void SetProjection(const glm::mat4 &m) { m_Projection = m; Calc(); }
    const glm::vec3& GetPosition() const { return m_Position; }
    void SetPosition(float x, float y, float z) { m_Position = glm::vec3{x, y, z}; Calc(); }
    void SetPosition(const glm::vec3 &pos) { m_Position = pos; Calc(); }

private:
    void Calc();

    glm::vec3 m_Position;
    glm::mat4 m_Projection;
    glm::mat4 m_View;

    glm::vec3 m_Dir = {0.0f, 0.0f, 0.0f}; // 不是方向 是他媽的被看得那個點
    glm::vec3 m_Up = {0.0, 1.0, 0.0};
};

#endif //OPENGLPLAYGROUND_CAMERA_H
