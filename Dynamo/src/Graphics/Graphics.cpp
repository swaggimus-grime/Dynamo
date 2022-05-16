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

Graphics::Graphics(HWND hWnd, UINT width, UINT height)
{
    m_GPU = std::make_unique<GPU>(hWnd);
    D3D11_VIEWPORT viewport{};
    viewport.TopLeftX = 0;
    viewport.TopLeftY = 0;
    viewport.Width = width;
    viewport.Height = height;
    viewport.MinDepth = 0.f;
    viewport.MaxDepth = 1.f;
    m_GPU->SetViewport(viewport);

    ImGui_ImplDX11_Init(m_GPU->GetDevice(), m_GPU->GetDC());

}

Graphics::~Graphics()
{
    ImGui_ImplDX11_Shutdown();
}

void Graphics::BeginFrame()
{
    Gui::BeginFrame();
}

void Graphics::EndFrame()
{       
    Gui::EndFrame();
    m_GPU->Present();
}