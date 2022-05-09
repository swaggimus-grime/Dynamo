#include "dynamopch.h"
#include "GPU.h"

GPU::GPU(HWND& hWnd)
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
    m_SC->GetBuffer(0, __uuidof(ID3D11Resource), reinterpret_cast<void**>(&backBuff));
    m_Device->CreateRenderTargetView(backBuff, nullptr, &m_RTV);
    backBuff->Release();

    D3D11_VIEWPORT viewport;
    SecureZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));
    viewport.TopLeftX = 0;
    viewport.TopLeftY = 0;
    viewport.Width = 1280;
    viewport.Height = 720;
    viewport.MinDepth = 0.f;
    viewport.MaxDepth = 1.f;

    m_DC->OMSetRenderTargets(1, m_RTV.GetAddressOf(), nullptr);
    m_DC->RSSetViewports(1, &viewport);

    SetTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}
