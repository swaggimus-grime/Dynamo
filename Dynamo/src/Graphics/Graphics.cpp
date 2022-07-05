#include "dynamopch.h"
#include "Graphics.h"
#include "RenderTarget.h"
#include <dxgi.h>
#include <d3d11.h>
#include "GUI/Gui.h"
#include "Camera.h"
#include <imgui_impl_dx11.h>
#include "Camera.h"
#include "DSView.h"

Graphics::Graphics(HWND hWnd, UINT width, UINT height)
{
    DXGI_SWAP_CHAIN_DESC scd;
    SecureZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));
    scd.BufferDesc.Width = width;
    scd.BufferDesc.Height = height;
    scd.BufferDesc.RefreshRate.Numerator = 0;
    scd.BufferDesc.RefreshRate.Denominator = 0;
    scd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
    scd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    scd.BufferCount = 1;
    scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    scd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
    scd.OutputWindow = hWnd;
    scd.SampleDesc.Count = 1;
    scd.SampleDesc.Quality = 0;
    scd.Windowed = TRUE;
    scd.Flags = NULL;

    D3D11CreateDeviceAndSwapChain(
        NULL,
        D3D_DRIVER_TYPE_HARDWARE,
        NULL,
        NULL,
        NULL,
        NULL,
        D3D11_SDK_VERSION,
        &scd,
        &m_SC,
        &m_Device,
        NULL,
        &m_DC);

    ComPtr<ID3D11Texture2D> backbuff;
    m_SC->GetBuffer(0, __uuidof(ID3D11Texture2D), &backbuff);
    m_FinalOutput = std::make_unique<WriteRenderTarget>(*this, *backbuff.Get());
    m_DepthStencil = std::make_unique<DSView>(*this, width, height);

    m_DC->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    D3D11_VIEWPORT viewport{};
    viewport.TopLeftX = 0;
    viewport.TopLeftY = 0;
    viewport.Width = width;
    viewport.Height = height;
    viewport.MinDepth = 0.f;
    viewport.MaxDepth = 1.f;
    m_DC->RSSetViewports(1, &viewport);

    ImGui_ImplDX11_Init(m_Device.Get(), m_DC.Get());
}

Graphics::~Graphics()
{
    ImGui_ImplDX11_Shutdown();
}

void Graphics::BeginFrame(Camera& camera)
{
    m_LookAt = std::move(camera.LookAt());
    m_Projection = std::move(camera.Projection());
    BindBackBuffer();
    m_FinalOutput->Clear(*this);
    m_DepthStencil->Clear(*this);
    Gui::BeginFrame();
}

void Graphics::EndFrame()
{
    Gui::EndFrame();
    m_SC->Present(0, 0);
}

void Graphics::BindBackBuffer()
{
    m_FinalOutput->Bind(*this, *m_DepthStencil);
}

void Graphics::SubmitRenderTarget(std::shared_ptr<RenderTarget> r)
{
    m_Targets.push_back(r);
}

void Graphics::OnWindowResize(UINT width, UINT height)
{
    ComPtr<ID3D11Texture2D> tex;
    m_SC->GetBuffer(0, __uuidof(ID3D11Texture2D), &tex);
    m_FinalOutput.reset(new WriteRenderTarget(*this, *tex.Get()));
    m_SC->ResizeBuffers(0, width, height, DXGI_FORMAT_R8G8B8A8_UNORM, 0);
    for (auto& rt : m_Targets)
        rt.reset(new RenderTarget(*this, width, height));
    m_Width = width;
    m_Height = height;
}