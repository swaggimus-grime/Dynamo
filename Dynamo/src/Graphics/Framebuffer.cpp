#include "dynamopch.h"
#include "Framebuffer.h"
#include "Graphics.h"

Framebuffer::Framebuffer(Graphics& g, UINT width, UINT height, BOOL depthEnable)
    :m_DepthEnable(depthEnable)
{
    g.AddFramebuffer(std::make_shared<Framebuffer>(*this));
    Reset(g, width, height);
}

void Framebuffer::Clear(Graphics& g)
{
    g.DC().ClearRenderTargetView(m_ColorView.Get(), &m_ClearColor.x);
    g.DC().ClearDepthStencilView(m_DSView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0.f);
}

void Framebuffer::Bind(Graphics& g)
{
    g.DC().OMSetRenderTargets(1, m_ColorView.GetAddressOf(), m_DSView.Get());
}

void Framebuffer::Reset(Graphics& g, UINT width, UINT height)
{
    ID3D11Texture2D* backBuff;
    g.SC().GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&backBuff));
    g.Device().CreateRenderTargetView(backBuff, nullptr, m_ColorView.GetAddressOf());
    backBuff->Release();

    if (m_DepthEnable) {
        ComPtr<ID3D11Texture2D> dsBuff;
        D3D11_TEXTURE2D_DESC dsBufferDesc;
        dsBufferDesc.Width = width;
        dsBufferDesc.Height = height;
        dsBufferDesc.MipLevels = 1;
        dsBufferDesc.ArraySize = 1;
        dsBufferDesc.Format = DXGI_FORMAT_D32_FLOAT;
        dsBufferDesc.SampleDesc.Count = 1;
        dsBufferDesc.SampleDesc.Quality = 0;
        dsBufferDesc.Usage = D3D11_USAGE_DEFAULT;
        dsBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
        dsBufferDesc.CPUAccessFlags = 0;
        dsBufferDesc.MiscFlags = 0;
        g.Device().CreateTexture2D(&dsBufferDesc, NULL, dsBuff.GetAddressOf());

        D3D11_DEPTH_STENCIL_VIEW_DESC dsViewDesc{};
        dsViewDesc.Format = dsBufferDesc.Format;
        dsViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
        dsViewDesc.Texture2D.MipSlice = 0;
        g.Device().CreateDepthStencilView(dsBuff.Get(), &dsViewDesc, m_DSView.GetAddressOf());
    }
}