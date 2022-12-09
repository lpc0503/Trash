#include "Entity.h"

#include <map>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/matrix_decompose.hpp>

static std::map<uint32_t, Entity*> g_IDtoEntity; // TODO: to Ref

void Entity::Transform::GetRTS(glm::vec3 &pos, glm::vec3 &rotate, glm::vec3 &scale)
{
    // TODO: 因為我們 renderer 目前只支援輸入個別的資料而不支援輸入 matrix 所以這邊先做轉換
    // ref: https://stackoverflow.com/questions/17918033/glm-decompose-mat4-into-translation-and-rotation
    //      https://stackoverflow.com/questions/12048046/quaternions-euler-angles-rotation-matrix-trouble-glm
    glm::quat rotation;
    glm::vec3 skew;
    glm::vec4 perspective;

    glm::decompose(modelMatrix, scale, rotation, pos, skew, perspective);
    //rotation = glm::conjugate(rotation);
    rotate = glm::eulerAngles(rotation);
    rotate = glm::degrees(rotate);
}

glm::mat4 Entity::Transform::GetLocalModelMatrix()
{
    const glm::mat4 transformX = glm::rotate(glm::mat4(1.0f),
                                             glm::radians(rotate.x),
                                             glm::vec3(1.0f, 0.0f, 0.0f));
    const glm::mat4 transformY = glm::rotate(glm::mat4(1.0f),
                                             glm::radians(rotate.y),
                                             glm::vec3(0.0f, 1.0f, 0.0f));
    const glm::mat4 transformZ = glm::rotate(glm::mat4(1.0f),
                                             glm::radians(rotate.z),
                                             glm::vec3(0.0f, 0.0f, 1.0f));

    // Y * X * Z
    const glm::mat4 rotationMatrix = transformY * transformX * transformZ;

    // translation * rotation * scale (also known as TRS matrix)
    return glm::translate(glm::mat4(1.0f), pos) *
           rotationMatrix *
           glm::scale(glm::mat4(1.0f), scale);
}

// Entity

uint32_t Entity::s_NextID = 1; // TODO: move

Entity::Entity(const std::string &name_)
        : name(name_)
{
    id = s_NextID++;
    g_IDtoEntity[id] = this;
}

Ref<Entity> Entity::Create(const std::string &name_)
{
    return MakeRef<Entity>(name_);
}

void Entity::Move(const glm::vec3 &off)
{
    transform.pos += off;
    UpdateSelfAndChild();
}

// TODO: delete this function in future
void Entity::Render()
{
//    ASSERT(Renderer::IsSceneRendering() == true, "MUST call Renderer::BeginScene() first!");

    glm::vec3 pos{0.f}, rotate{0.f}, scale{0.f};
    transform.GetRTS(pos, rotate, scale);
    Renderer::DrawMesh(mesh, pos, -rotate, scale, color);

//        INFO("{}, {}, {}", rotate.x, rotate.y, rotate.z);

    for(auto child : children)
    {
        child->Render();
    }
}

void Entity::UpdateSelfAndChild()
{
    if (parent)
        transform.modelMatrix = /*parent->transform.GetLocalModelMatrix()*/parent->transform.modelMatrix * transform.GetLocalModelMatrix();
    else
        transform.modelMatrix = transform.GetLocalModelMatrix();

    for (auto child : children)
    {
        child->UpdateSelfAndChild();
    }
}

void Entity::AddChild(Entity *ent)
{
    children.push_back(ent);
    ent->parent = this;
    //
    UpdateSelfAndChild();
}

Entity* Entity::GetEntityByID(uint32_t id)
{
    return g_IDtoEntity.count(id) ? g_IDtoEntity[id] : nullptr;
}

//Ref<Entity> Entity::GetEntityByID(uint32_t id)
//{
//    return std::shared_ptr<Entity>(g_IDtoEntity[id]);
//}
