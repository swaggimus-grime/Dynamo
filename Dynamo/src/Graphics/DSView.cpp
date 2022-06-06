#include "dynamopch.h"
#include "DSView.h"

DSView::DSView(Graphics& g, UINT width, UINT height)
{
    ComPtr<ID3D11Texture2D> dsBuff;
    D3D11_TEXTURE2D_DESC dsBufferDesc;
    dsBufferDesc.Width = width;
    dsBufferDesc.Height = height;
    dsBufferDesc.MipLevels = 1;
    dsBufferDesc.ArraySize = 1;
    dsBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
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
    g.Device().CreateDepthStencilView(dsBuff.Get(), &dsViewDesc, &m_DSV);
}

void DSView::Clear(Graphics& g)
{
    g.DC().ClearDepthStencilView(m_DSV.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);
}