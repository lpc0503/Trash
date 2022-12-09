#define TINYOBJLOADER_IMPLEMENTATION
#include "Camera.h"

#include <glm/gtc/matrix_transform.hpp>

Camera::Camera()
{
}

void Camera::LookAt(float x, float y, float z)
{
    m_Dir = glm::vec3{x, y, z};
    Calc();
}

void Camera::Move(float x, float y, float z)
{
    m_Position.x += x;
    m_Position.y += y;
    m_Position.z += z;
    Calc();
}

Camera::Camera(const glm::mat4 &projection)
{
    m_Projection = projection;
    Calc();
}

void Camera::Calc()
{
    m_View = glm::lookAt(m_Position,	            // eye
                         m_Dir,	    // center
                         m_Up);	// up
}
