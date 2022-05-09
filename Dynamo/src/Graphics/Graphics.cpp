#include "dynamopch.h"
#include "Graphics.h"
#include <d3d11.h>
#include "Gui.h"
#include "Camera.h"
#include <imgui_impl_dx11.h>
#include "Camera.h"
#include "Renderable.h"
#include "Shader.h"
#include "Buffer.h"


struct Transform {
    XMMATRIX MVP;
    XMMATRIX ModelView;
};

struct Light {
    XMFLOAT3 Pos{ 0.f, 10.f, 5.f };
    XMFLOAT4 Color{ 1.f, 1.f, 1.f, 1.f };
};

static struct Data2D {
    struct VERTEX {
        float x;
        float y;
    };

    const std::vector<VERTEX> vertices = { {-0.5f, -0.5f }, {0.f, 0.5f}, {0.5f, -0.5f} };
    std::shared_ptr<Shader> flatShader;
    VertexLayout layout;
    std::shared_ptr<VertexBuffer<VERTEX>> vbuff;
    Data2D(std::shared_ptr<GPU> gpu) {
        flatShader = std::make_shared<Shader>(gpu, L"res/shaders/Flatvs.cso", L"res/shaders/Flatps.cso");
        layout.AddAttrib("Pos", DXGI_FORMAT_R32G32_FLOAT);
        vbuff = std::make_shared<VertexBuffer<VERTEX>>(gpu, vertices, flatShader->GetVSCode(), layout);
    }
}* data2D;

Transform transform;
Light light;
std::unique_ptr<ConstantBuffer> transformBuff;
std::unique_ptr<ConstantBuffer> lightBuff;

Graphics::Graphics(HWND hWnd)
{
    m_GPU = std::make_shared<GPU>(hWnd);
    ImGui_ImplDX11_Init(m_GPU->GetDevice(), m_GPU->GetDC());
    transformBuff = std::make_unique<ConstantBuffer>(m_GPU, sizeof(Transform), &transform);
    lightBuff = std::make_unique<ConstantBuffer>(m_GPU, sizeof(Light), &light);
    data2D = new Data2D(m_GPU);
}

Graphics::~Graphics()
{
    delete data2D;
    ImGui_ImplDX11_Shutdown();
}

void Graphics::BeginFrame(std::shared_ptr<Camera> camera)
{
    m_GPU->Clear({ 0.f, 0.f, 0.f, 1.f });
    m_Camera = camera;
    Gui::BeginFrame();
}

void Graphics::Render(std::shared_ptr<Renderable> r)
{
    auto mvp = XMMatrixIdentity() * m_Camera->LookAt();
    transform.ModelView = XMMatrixTranspose(mvp);
    mvp *= m_Camera->Projection();
    transform.MVP = XMMatrixTranspose(mvp);
    D3D11_MAPPED_SUBRESOURCE mapped;
    m_GPU->GetDC()->Map(transformBuff->GetBufferPointer(), NULL, D3D11_MAP_WRITE_DISCARD, NULL, &mapped);
    memcpy(mapped.pData, &transform, sizeof(transform));
    m_GPU->GetDC()->Unmap(transformBuff->GetBufferPointer(), NULL);
    const auto pBuff = transformBuff->GetBufferPointer();
    m_GPU->GetDC()->VSSetConstantBuffers(0, 1, &pBuff);

    r->Render();
}

void Graphics::DrawTriangle()
{
    auto mvp = XMMatrixIdentity() * m_Camera->LookAt();
    transform.ModelView = XMMatrixTranspose(mvp);
    mvp *= m_Camera->Projection();
    transform.MVP = XMMatrixTranspose(mvp);
    D3D11_MAPPED_SUBRESOURCE mapped;
    m_GPU->GetDC()->Map(transformBuff->GetBufferPointer(), NULL, D3D11_MAP_WRITE_DISCARD, NULL, &mapped);
    memcpy(mapped.pData, &transform, sizeof(transform));
    m_GPU->GetDC()->Unmap(transformBuff->GetBufferPointer(), NULL);
    const auto pBuff = transformBuff->GetBufferPointer();
    m_GPU->GetDC()->VSSetConstantBuffers(0, 1, &pBuff);
    data2D->flatShader->Bind();
    data2D->vbuff->Bind();
    m_GPU->GetDC()->Draw(data2D->vertices.size(), 0);
}

void Graphics::EndFrame()
{       
    Gui::EndFrame();
    m_GPU->Present();
}