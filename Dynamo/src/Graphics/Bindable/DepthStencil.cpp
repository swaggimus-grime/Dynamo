#include "dynamopch.h"
#include "DepthStencil.h"

#include "Graphics.h"
#include "RenderTarget.h"

DXGI_FORMAT MapUsageTypeless(DS_USAGE usage)
{
    switch (usage)
    {
    case DS_USAGE::DEPTH_STENCIL:
        return DXGI_FORMAT::DXGI_FORMAT_R24G8_TYPELESS;
    case DS_USAGE::DEPTH_ONLY:
        return DXGI_FORMAT::DXGI_FORMAT_R32_TYPELESS;
    }
    throw std::runtime_error{ "Base usage for Typeless format map in DepthStencil." };
}

DXGI_FORMAT MapUsageTyped(DS_USAGE usage)
{
    switch (usage)
    {
    case DS_USAGE::DEPTH_STENCIL:
        return DXGI_FORMAT::DXGI_FORMAT_D24_UNORM_S8_UINT;
    case DS_USAGE::DEPTH_ONLY:
        return DXGI_FORMAT::DXGI_FORMAT_D32_FLOAT;
    }
    throw std::runtime_error{ "Base usage for Typed format map in DepthStencil." };
}

DXGI_FORMAT MapUsageColored(DS_USAGE usage)
{
    switch (usage)
    {
    case DS_USAGE::DEPTH_STENCIL:
        return DXGI_FORMAT::DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
    case DS_USAGE::DEPTH_ONLY:
        return DXGI_FORMAT::DXGI_FORMAT_R32_FLOAT;
    }
    throw std::runtime_error{ "Base usage for Colored format map in DepthStencil." };
}

void DepthStencil::BindBuffer(Graphics& g)
{
    g.DC().OMSetRenderTargets(0, nullptr, m_DSV.Get());
}

void DepthStencil::BindBuffer(Graphics& g, Buffer* other)
{
    assert(dynamic_cast<RenderTarget*>(other) != nullptr);
    BindBuffer(g, static_cast<RenderTarget*>(other));
}

void DepthStencil::BindBuffer(Graphics& g, RenderTarget* other)
{
    other->BindBuffer(g, this);
}

void DepthStencil::Clear(Graphics& g)
{
    g.DC().ClearDepthStencilView(m_DSV.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);
}

DepthStencil::DepthStencil(Graphics& g, ComPtr<ID3D11Texture2D> texture, UINT face)
{
    D3D11_TEXTURE2D_DESC descTex = {};
    texture->GetDesc(&descTex);
    m_Width = descTex.Width;
    m_Height = descTex.Height;

    // create target view of depth stensil texture
    D3D11_DEPTH_STENCIL_VIEW_DESC descView = {};
    descView.Format = DXGI_FORMAT_D32_FLOAT;
    descView.Flags = 0;
    descView.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DARRAY;
    descView.Texture2DArray.MipSlice = 0;
    descView.Texture2DArray.ArraySize = 1;
    descView.Texture2DArray.FirstArraySlice = face;
    g.Device().CreateDepthStencilView(
        texture.Get(), &descView, &m_DSV
    );
}

DepthStencil::DepthStencil(Graphics& g, UINT width, UINT height, bool canBindShaderInput, DS_USAGE usage)
    :m_Width(width), m_Height(height)
{
    // create depth stensil texture
    ComPtr<ID3D11Texture2D> texture;
    D3D11_TEXTURE2D_DESC descDepth = {};
    descDepth.Width = width;
    descDepth.Height = height;
    descDepth.MipLevels = 1u;
    descDepth.ArraySize = 1u;
    descDepth.Format = MapUsageTypeless(usage);
    descDepth.SampleDesc.Count = 1u;
    descDepth.SampleDesc.Quality = 0u;
    descDepth.Usage = D3D11_USAGE_DEFAULT;
    descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL | (canBindShaderInput ? D3D11_BIND_SHADER_RESOURCE : 0);
    g.Device().CreateTexture2D(&descDepth, nullptr, &texture);

    // create target view of depth stensil texture
    D3D11_DEPTH_STENCIL_VIEW_DESC descView = {};
    descView.Format = MapUsageTyped(usage);
    descView.Flags = 0;
    descView.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
    descView.Texture2D.MipSlice = 0;
    g.Device().CreateDepthStencilView(
        texture.Get(), &descView, &m_DSV
    );
}

ReadableDepthStencil::ReadableDepthStencil(Graphics& g, UINT slot, DS_USAGE usage)
    :ReadableDepthStencil(g, g.Width(), g.Height(), slot, usage)
{
}

ReadableDepthStencil::ReadableDepthStencil(Graphics& g, UINT width, UINT height, UINT slot, DS_USAGE usage)
    :DepthStencil(g, width, height, true, usage), m_Slot(slot)
{
    ComPtr<ID3D11Resource> pRes;
    m_DSV->GetResource(&pRes);

    D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
    srvDesc.Format = MapUsageColored(usage);
    srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    srvDesc.Texture2D.MostDetailedMip = 0;
    srvDesc.Texture2D.MipLevels = 1;
    g.Device().CreateShaderResourceView(
        pRes.Get(), &srvDesc, &m_SRV
    );
}

void ReadableDepthStencil::Bind(Graphics& g)
{
    g.DC().PSSetShaderResources(m_Slot, 1u, m_SRV.GetAddressOf());
}

WriteOnlyDepthStencil::WriteOnlyDepthStencil(Graphics& g, ComPtr<ID3D11Texture2D> texture, UINT face)
    :DepthStencil(g, std::move(texture), face)
{
}

WriteOnlyDepthStencil::WriteOnlyDepthStencil(Graphics& g)
    :WriteOnlyDepthStencil(g, g.Width(), g.Height())
{
}

WriteOnlyDepthStencil::WriteOnlyDepthStencil(Graphics& g, UINT width, UINT height)
    :DepthStencil(g, width, height, false, DS_USAGE::DEPTH_STENCIL)
{
}

void WriteOnlyDepthStencil::Bind(Graphics& gfx)
{
    assert("OutputOnlyDepthStencil cannot be bound as shader input" && false);
}
