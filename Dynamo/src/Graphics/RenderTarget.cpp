#include "dynamopch.h"
#include "RenderTarget.h"

RenderTarget::RenderTarget(Graphics& g, UINT width, UINT height, UINT slot)
    :Bindable(CreateHash(width, height, slot)), m_Format(DXGI_FORMAT_R8G8B8A8_UNORM)
{
    D3D11_TEXTURE2D_DESC texDesc = {};
    texDesc.Width = width;
    texDesc.Height = height;
    texDesc.MipLevels = 1;
    texDesc.ArraySize = 1;
    texDesc.Format = m_Format;
    texDesc.SampleDesc.Count = 1;
    texDesc.SampleDesc.Quality = 0;
    texDesc.Usage = D3D11_USAGE_DEFAULT;
    texDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
    texDesc.CPUAccessFlags = 0;
    texDesc.MiscFlags = 0;
    ComPtr<ID3D11Texture2D> texture;
    DX_ASSERT(g.Device().CreateTexture2D(&texDesc, nullptr, &texture));

    D3D11_RENDER_TARGET_VIEW_DESC viewDesc = {};
    viewDesc.Format = texDesc.Format;
    viewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
    viewDesc.Texture2D = D3D11_TEX2D_RTV{ 0 };
    DX_ASSERT(g.Device().CreateRenderTargetView(texture.Get(), &viewDesc, &m_RTV));
}

RenderTarget::RenderTarget(Graphics& g, ID3D11Texture2D& texture, UINT slot)
    :Bindable(CreateHash(texture, slot))
{
    D3D11_TEXTURE2D_DESC texDesc;
    texture.GetDesc(&texDesc);
    D3D11_RENDER_TARGET_VIEW_DESC viewDesc = {};
    viewDesc.Format = texDesc.Format;
    m_Format = texDesc.Format;
    viewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
    viewDesc.Texture2D = D3D11_TEX2D_RTV{ 0 };
    
    DX_ASSERT(g.Device().CreateRenderTargetView(&texture, &viewDesc, &m_RTV));
}

void RenderTarget::Bind(Graphics& g)
{
    g.DC().OMSetRenderTargets(1, m_RTV.GetAddressOf(), nullptr);
}

void RenderTarget::Bind(Graphics& g, DepthStencilView& ds)
{
    g.DC().OMSetRenderTargets(1, m_RTV.GetAddressOf(), ds.m_DSV.Get());
}

void RenderTarget::Clear(Graphics& g)
{
    g.DC().ClearRenderTargetView(m_RTV.Get(), &m_ClearColor.x);
}

std::string RenderTarget::CreateHash(ID3D11Texture2D& texture, UINT slot)
{
    D3D11_TEXTURE2D_DESC d;
    texture.GetDesc(&d);
    return typeid(RenderTarget).name() + "#"s + std::to_string(d.Width) + std::to_string(d.Height) + std::to_string(slot);
}

std::string RenderTarget::CreateHash(UINT width, UINT height, UINT slot)
{
    return typeid(RenderTarget).name() + "#"s + std::to_string(width) + std::to_string(height) + std::to_string(slot);
}

ReadableRenderTarget::ReadableRenderTarget(Graphics& g, UINT width, UINT height, UINT slot)
    :RenderTarget(g, width, height, slot), m_Slot(slot)
{
    ComPtr<ID3D11Resource> tex;
    m_RTV->GetResource(&tex);
    D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
    srvDesc.Format = m_Format;
    srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    srvDesc.Texture2D.MostDetailedMip = 0;
    srvDesc.Texture2D.MipLevels = 1;
    DX_ASSERT(g.Device().CreateShaderResourceView(tex.Get(), &srvDesc, &m_View));
}

void ReadableRenderTarget::BindTexture(Graphics& g)
{
    g.DC().PSSetShaderResources(m_Slot, 1, m_View.GetAddressOf());
}
