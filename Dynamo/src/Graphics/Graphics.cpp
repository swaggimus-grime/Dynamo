#include "dynamopch.h"
#include "Graphics.h"
#include <d3d11.h>
#include "Model.h"

Graphics::Graphics(HWND hWnd)
{
    DXGI_SWAP_CHAIN_DESC scd;
    SecureZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));
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

    ID3D11Resource* backBuff;
    m_SC->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&backBuff));
    m_Device->CreateRenderTargetView(backBuff, nullptr, &m_RTV);
    backBuff->Release();

    Model model("bruh");
}

Graphics::~Graphics()
{
}

void Graphics::Render()
{
    float color[] = { 1.f, 0.f, 1.f, 1.f };
    m_DC->ClearRenderTargetView(*&m_RTV, color);

    m_SC->Present(0, 0);
}
