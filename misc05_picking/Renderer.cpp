#include "Renderer.h"
#include "RendererAPI.h"
#include "Entity.h"

#include <GL/glew.h>
#include <memory>
#include <vector>

RendererAPI *g_RenderAPI;
bool g_IsRendering = false;

// TODO: pimpl
Ref<Camera> g_CurrentCamera;

void Renderer::Init()
{
    g_RenderAPI = new RendererAPI();
    g_RenderAPI->Init();
}

void Renderer::Shutdown()
{
    g_RenderAPI->Shutdown();
    delete g_RenderAPI;
}

void Renderer::BeginScene(Ref<Camera> camera)
{
    g_CurrentCamera = camera;
    g_IsRendering = true;

    g_RenderAPI->BindLineShader();
    g_RenderAPI->SetMatrix("M", glm::mat4(1.f));
    g_RenderAPI->SetMatrix("V", camera->GetView());
    g_RenderAPI->SetMatrix("P", camera->GetProjection());
    g_RenderAPI->SetBool("uEnableLight", false);
    g_RenderAPI->UnbindShader();

    g_RenderAPI->BindMeshShader();
    g_RenderAPI->SetMatrix("V", camera->GetView());
    g_RenderAPI->SetMatrix("P", camera->GetProjection());
    g_RenderAPI->SetMatrix("MV", camera->GetView());
    g_RenderAPI->SetBool("uEnableLight", true);
    g_RenderAPI->UnbindShader();
}

void Renderer::EndScene()
{
    g_RenderAPI->BindLineShader();
    g_RenderAPI->SendLineData();
    g_RenderAPI->DrawLines();
    g_RenderAPI->UnbindShader();

    // TODO: figure out how to batch rendering mesh
//    g_RenderAPI->BindMeshShader();
//    g_RenderAPI->SendMeshData();
//    g_RenderAPI->DrawMeshs();
//    g_RenderAPI->UnbindShader();

    g_RenderAPI->DrawMeshs();

    g_RenderAPI->ClearRendererState();
    g_IsRendering = false;
}

void Renderer::BeginPickingScene(Ref<Camera> camera)
{
    g_IsRendering = true;
    g_CurrentCamera = camera;
}

void Renderer::EndPickingScene()
{
    g_RenderAPI->BindPickingShader();
    g_RenderAPI->DrawEntitiesForPicking(g_CurrentCamera);
    g_RenderAPI->UnbindShader();

    g_IsRendering = false;
}

void Renderer::DrawPoint()
{
    // TODO: implement
}

void Renderer::DrawLine(const glm::vec3 &p0, const glm::vec3 &p1, const glm::vec4 &color)
{
    g_RenderAPI->PushLine(p0, p1, color);
}

void Renderer::DrawPointLight(const glm::vec3 &pos, const glm::vec3 &dir, const glm::vec4 &color, float intensity)
{
    // TODO: implement
}

void Renderer::DrawGrid(int n, int m)
{
    for(int x = -n; x <= n; x++)
    {
        if(x == 0)
            Renderer::DrawLine({x, 0.f, 0}, {x, 0.f, -m}, {1.f, 1.f, 1.f, 1.f});
        else
            Renderer::DrawLine({x, 0.f, m}, {x, 0.f, -m}, {1.f, 1.f, 1.f, 1.f});
    }

    for(int z = -m; z <= m; z++)
    {
        if(z == 0)
            Renderer::DrawLine({0, 0.f, z}, {-n, 0.f, z}, {1.f, 1.f, 1.f, 1.f});
        else
            Renderer::DrawLine({n, 0.f, z}, {-n, 0.f, z}, {1.f, 1.f, 1.f, 1.f});
    }

    float l = std::max(n, m);
    Renderer::DrawLine(glm::vec3 {0, 0, 0}, glm::vec3 {l, 0.f, 0.f}, glm::vec4 {1.f, 0.f, 0.f, 1.f});
    Renderer::DrawLine(glm::vec3 {0, 0, 0}, glm::vec3 {0.f, l, 0.f}, glm::vec4 {0.f, 1.f, 0.f, 1.f});
    Renderer::DrawLine(glm::vec3 {0, 0, 0}, glm::vec3 {0.f, 0.f, l}, glm::vec4 {0.f, 0.f, 1.f, 1.f});
}

void Renderer::DrawModel(Ref<Model> model, const glm::vec3 &pos, const glm::vec3 &rotate, const glm::vec3 &scale)
{
    for(auto mesh : model->GetMeshes())
    {

    }
}

void Renderer::DrawMesh(Ref<Mesh> mesh, const glm::vec3 &pos, const glm::vec3 &rotate, const glm::vec3 &scale, const glm::vec4 &tint)
{
    g_RenderAPI->PushMesh(mesh, pos, rotate, scale, tint);
}

void Renderer::DrawDirectionalLight(const glm::vec3 &dir, const glm::vec4 &color)
{
    g_RenderAPI->PushDirectionalLight(dir, color);
}

bool Renderer::IsSceneRendering()
{
    return g_IsRendering;
}

void Renderer::TestPickingEntity(Ref<Entity> entity)
{
    g_RenderAPI->PushPickingEntity(entity);
}

void Renderer::ClearViewport()
{
    g_RenderAPI->ClearViewport();
}
