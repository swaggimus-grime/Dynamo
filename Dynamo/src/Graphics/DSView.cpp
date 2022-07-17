#include "dynamopch.h"
#include "DSView.h"

DepthStencilView::DepthStencilView(Graphics& g, UINT width, UINT height, DepthStencilMode mode)
{
    DXGI_FORMAT texFormat;
    DXGI_FORMAT viewFormat;
    switch (mode) {
        case DepthStencilMode::DEPTH_ONLY:
            texFormat = DXGI_FORMAT_R32_TYPELESS;
            viewFormat = DXGI_FORMAT_D32_FLOAT;
            break;
        case DepthStencilMode::DEPTH_STENCIL:
            texFormat = DXGI_FORMAT_R24G8_TYPELESS;
            viewFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
            break;
    }

    ComPtr<ID3D11Texture2D> dsBuff;
    D3D11_TEXTURE2D_DESC dsBufferDesc;
    dsBufferDesc.Width = width;
    dsBufferDesc.Height = height;
    dsBufferDesc.MipLevels = 1;
    dsBufferDesc.ArraySize = 1;
    dsBufferDesc.Format = texFormat;
    dsBufferDesc.SampleDesc.Count = 1;
    dsBufferDesc.SampleDesc.Quality = 0;
    dsBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    dsBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    dsBufferDesc.CPUAccessFlags = 0;
    dsBufferDesc.MiscFlags = 0;
    g.Device().CreateTexture2D(&dsBufferDesc, NULL, dsBuff.GetAddressOf());

    D3D11_DEPTH_STENCIL_VIEW_DESC dsViewDesc{};
    dsViewDesc.Format = viewFormat;
    dsViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
    dsViewDesc.Texture2D.MipSlice = 0;
    g.Device().CreateDepthStencilView(dsBuff.Get(), &dsViewDesc, &m_DSV);
}

void DepthStencilView::Clear(Graphics& g)
{
    g.DC().ClearDepthStencilView(m_DSV.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);
}

ReadableDepthStencilView::ReadableDepthStencilView(Graphics& g, UINT width, UINT height, DepthStencilMode mode, UINT slot)
    :DepthStencilView(g, width, height, mode), m_Slot(slot)
{
    ComPtr<ID3D11Resource> tex;
    m_DSV->GetResource(&tex);

    DXGI_FORMAT format;
    switch (mode) {
    case DepthStencilMode::DEPTH_ONLY:
        format = DXGI_FORMAT_R32_FLOAT;
        break;
    case  DepthStencilMode::DEPTH_STENCIL:
        format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
        break;
    }

    D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
    srvDesc.Format = format;
    srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    srvDesc.Texture2D.MostDetailedMip = 0;
    srvDesc.Texture2D.MipLevels = 1;
    g.Device().CreateShaderResourceView(tex.Get(), &srvDesc, &m_View);
}

void ReadableDepthStencilView::BindTexture(Graphics& g)
{
    g.DC().PSSetShaderResources(m_Slot, 1, m_View.GetAddressOf());
}
