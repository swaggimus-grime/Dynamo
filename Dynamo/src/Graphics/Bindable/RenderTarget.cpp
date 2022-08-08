#include "dynamopch.h"
#include "RenderTarget.h"

#include "Graphics.h"
#include "DepthStencil.h"

void RenderTarget::BindBuffer(Graphics& g)
{
	ID3D11DepthStencilView* const null = nullptr;
	BindBuffer(g, null);
}

void RenderTarget::BindBuffer(Graphics& g, Buffer* depthStencil)
{
	assert(dynamic_cast<DepthStencil*>(depthStencil) != nullptr);
	BindBuffer(g, static_cast<DepthStencil*>(depthStencil));
}

void RenderTarget::BindBuffer(Graphics& g, DepthStencil* depthStencil)
{
	BindBuffer(g, depthStencil ? depthStencil->m_DSV.Get() : nullptr);
}

void RenderTarget::Clear(Graphics& g)
{
	Clear(g, { 1.0f,0.0f,0.0f,0.0f });
}

void RenderTarget::Clear(Graphics& g, const std::array<float, 4>& color)
{
	g.DC().ClearRenderTargetView(m_RTV.Get(), color.data());
}

void RenderTarget::BindBuffer(Graphics& g, ID3D11DepthStencilView* pDepthStencilView)
{
	g.DC().OMSetRenderTargets(1, m_RTV.GetAddressOf(), pDepthStencilView);

	// configure viewport
	D3D11_VIEWPORT vp;
	vp.Width = (float)m_Width;
	vp.Height = (float)m_Height;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0.0f;
	vp.TopLeftY = 0.0f;
	g.DC().RSSetViewports(1u, &vp);
}

RenderTarget::RenderTarget(Graphics& g, ID3D11Texture2D* pTexture, std::optional<UINT> face)
{
	// get information from texture about dimensions
	D3D11_TEXTURE2D_DESC textureDesc;
	pTexture->GetDesc(&textureDesc);
	m_Width = textureDesc.Width;
	m_Height = textureDesc.Height;

	// create the target view on the texture
	D3D11_RENDER_TARGET_VIEW_DESC rtvDesc = {};
	rtvDesc.Format = textureDesc.Format;
	if (face.has_value())
	{
		rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DARRAY;
		rtvDesc.Texture2DArray.ArraySize = 1;
		rtvDesc.Texture2DArray.FirstArraySlice = *face;
		rtvDesc.Texture2DArray.MipSlice = 0;
	}
	else
	{
		rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
		rtvDesc.Texture2D = D3D11_TEX2D_RTV{ 0 };
	}

	g.Device().CreateRenderTargetView(
		pTexture, &rtvDesc, &m_RTV
	);
}

RenderTarget::RenderTarget(Graphics& g, UINT width, UINT height)
	:m_Width(width),	m_Height(height)
{
	// create texture resource
	D3D11_TEXTURE2D_DESC textureDesc = {};
	textureDesc.Width = width;
	textureDesc.Height = height;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE; // never do we not want to bind offscreen RTs as inputs
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;
	ComPtr<ID3D11Texture2D> pTexture;
	g.Device().CreateTexture2D(
		&textureDesc, nullptr, &pTexture
	);

	// create the target view on the texture
	D3D11_RENDER_TARGET_VIEW_DESC rtvDesc = {};
	rtvDesc.Format = textureDesc.Format;
	rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	rtvDesc.Texture2D = D3D11_TEX2D_RTV{ 0 };
	g.Device().CreateRenderTargetView(
		pTexture.Get(), &rtvDesc, &m_RTV
	);
}

ReadableRenderTarget::ReadableRenderTarget(Graphics& g, UINT width, UINT height, UINT slot)
	:RenderTarget(g, width, height), m_Slot(slot)
{
	ComPtr<ID3D11Resource> pRes;
	m_RTV->GetResource(&pRes);

	// create the resource view on the texture
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = 1;
	g.Device().CreateShaderResourceView(
		pRes.Get(), &srvDesc, &m_SRV
	);
}

void ReadableRenderTarget::Bind(Graphics& g)
{
	g.DC().PSSetShaderResources(m_Slot, 1, m_SRV.GetAddressOf());
}

void WriteOnlyRenderTarget::Bind(Graphics& g)
{
	assert("Cannot bind OuputOnlyRenderTarget as shader input" && false);
}

WriteOnlyRenderTarget::WriteOnlyRenderTarget(Graphics& g, ID3D11Texture2D* pTexture, std::optional<UINT> face)
	:RenderTarget(g, pTexture, face)
{
}
