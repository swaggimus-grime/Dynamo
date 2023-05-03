#include "dynamopch.h"
#include "Graphics.h"

#include <imgui.h>
#include <imgui_impl_dx11.h>
#include "Entities/Camera.h"
#include "GUI/Gui.h"
#include "Bindable/DepthStencil.h"
#include "Bindable/RenderTarget.h"

Graphics::Graphics(HWND hWnd, UINT width, UINT height)
    :m_Width(width), m_Height(height), m_Mipped(false)
{
    D3D_FEATURE_LEVEL fl;
    DX_ASSERT(D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, NULL, D3D11_CREATE_DEVICE_DEBUG, nullptr, NULL, D3D11_SDK_VERSION, &m_Device, &fl, &m_DC));
    m_MaxSamples = D3D11_MAX_MULTISAMPLE_SAMPLE_COUNT;
    m_QualityLevel = 0;
    while (m_MaxSamples > 0)
    {
        DX_ASSERT(m_Device->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, m_MaxSamples, &m_QualityLevel));
        if (m_QualityLevel > 0)
            break;
        m_MaxSamples /= 2;
    }
   
    DXGI_SWAP_CHAIN_DESC scd;
    SecureZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));
    scd.BufferDesc.Width = width;
    scd.BufferDesc.Height = height;
    scd.BufferDesc.RefreshRate.Numerator = 0;
    scd.BufferDesc.RefreshRate.Denominator = 0;
    scd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
    scd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    scd.BufferCount = 2;
    scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    scd.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
    scd.OutputWindow = hWnd;
    scd.SampleDesc.Count = 1;
    scd.SampleDesc.Quality = 0;
    scd.Windowed = TRUE;
    //scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

    DX_ASSERT(D3D11CreateDeviceAndSwapChain(
        NULL,
        D3D_DRIVER_TYPE_HARDWARE,
        NULL,
        D3D11_CREATE_DEVICE_DEBUG,
        NULL,
        NULL,
        D3D11_SDK_VERSION,
        &scd,
        &m_SC,
        &m_Device,
        NULL,
        &m_DC));

    ComPtr<ID3D11Texture2D> backbuff;
    m_SC->GetBuffer(0, __uuidof(ID3D11Texture2D), &backbuff);
    m_FinalOutput = MakeShared<WriteOnlyRenderTarget>(*this, backbuff.Get());

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
    m_SC->SetFullscreenState(false, nullptr);
}

void Graphics::BeginFrame()
{
    Gui::BeginFrame();

    ID3D11ShaderResourceView* const pNullTex = nullptr;
    m_DC->PSSetShaderResources(0, 1, &pNullTex); // fullscreen input texture
    m_DC->PSSetShaderResources(3, 1, &pNullTex); // shadow map texture
}

void Graphics::EndFrame()
{
    Gui::EndFrame();
    DX_ASSERT(m_SC->Present(1, 0));
}

void Graphics::SubmitRenderTarget(std::shared_ptr<RenderTarget> r)
{
    m_Targets.push_back(r);
}

void Graphics::OnWindowResize(UINT width, UINT height)
{
    m_FinalOutput.reset();
    m_SC->ResizeBuffers(0, width, height, DXGI_FORMAT_UNKNOWN, DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH);
    ComPtr<ID3D11Texture2D> backbuff;
    m_SC->GetBuffer(0, __uuidof(ID3D11Texture2D), &backbuff);
    m_FinalOutput.reset(new WriteOnlyRenderTarget(*this, backbuff.Get()));
    m_Width = width;
    m_Height = height;
}