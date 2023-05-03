#include "dynamopch.h"
#include "Texture.h"
#include <comdef.h>
#include "Binds.h"

Texture2D::Texture2D(Graphics& g, const std::string& path, UINT slot)
	:m_Slot(slot)
{
	DirectX::ScratchImage image;
	DX_ASSERT(DirectX::LoadFromWICFile(NarrowToWide(path).c_str(), DirectX::WIC_FLAGS_IGNORE_SRGB, nullptr, image));

	if (image.GetMetadata().format != Format) {
		DirectX::ScratchImage converted;
		DirectX::Convert(
			*image.GetImage(0, 0, 0),
			Format,
			DirectX::TEX_FILTER_DEFAULT,
			DirectX::TEX_THRESHOLD_DEFAULT,
			converted
		);
		image = std::move(converted);
	}

	CreateView(g, image, m_View, false);
	CreateView(g, image, m_MippedView, true);
}

void Texture2D::Bind(Graphics& g)
{
	g.DC().PSSetShaderResources(m_Slot, 1, g.Mipped() ? m_MippedView.GetAddressOf() : m_View.GetAddressOf());
}

std::string Texture2D::CreateHash(const std::string& path, UINT slot)
{
	return typeid(Texture2D).name() + "#"s + path + std::to_string(slot);
}

Shared<Texture2D> Texture2D::Evaluate(Graphics& g, const std::string& path, UINT slot)
{
	return Binds::Evaluate<Texture2D>(g, path, slot);
}

void Texture2D::CreateView(Graphics& g, DirectX::ScratchImage& image, ComPtr<ID3D11ShaderResourceView>& view, bool mipped)
{
	D3D11_TEXTURE2D_DESC texDesc;
	SecureZeroMemory(&texDesc, sizeof(D3D11_TEXTURE2D_DESC));
	texDesc.Width = image.GetMetadata().width;
	texDesc.Height = image.GetMetadata().height;
	texDesc.MipLevels = static_cast<UINT>(!mipped);
	texDesc.ArraySize = 1;
	texDesc.Format = Format;
	texDesc.Usage = D3D11_USAGE_DEFAULT;
	texDesc.SampleDesc.Count = 1;
	texDesc.SampleDesc.Quality = 0;
	texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	if (mipped)
		texDesc.BindFlags |= D3D11_BIND_RENDER_TARGET;
	texDesc.CPUAccessFlags = 0;
	texDesc.MiscFlags = mipped ? D3D11_RESOURCE_MISC_GENERATE_MIPS : NULL;

	ComPtr<ID3D11Texture2D> texture;
	if (mipped) {
		DX_ASSERT(g.Device().CreateTexture2D(&texDesc, nullptr, &texture));
		g.DC().UpdateSubresource(texture.Get(), 0, nullptr, image.GetPixels(), image.GetImage(0, 0, 0)->rowPitch, 0);
	}
	else {
		D3D11_SUBRESOURCE_DATA sd = {};
		sd.pSysMem = image.GetPixels();
		sd.SysMemPitch = image.GetImage(0, 0, 0)->rowPitch;
		DX_ASSERT(g.Device().CreateTexture2D(&texDesc, &sd, &texture));
	}

	D3D11_SHADER_RESOURCE_VIEW_DESC viewDesc;
	SecureZeroMemory(&viewDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
	viewDesc.Format = texDesc.Format;
	viewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	viewDesc.Texture2D.MostDetailedMip = 0;
	viewDesc.Texture2D.MipLevels = mipped ? -1 : 1;
	DX_ASSERT(g.Device().CreateShaderResourceView(texture.Get(), &viewDesc, view.GetAddressOf()));

	if (mipped)
		g.DC().GenerateMips(view.Get());
}

Cubemap::Cubemap(Graphics& g, const std::string& texDir, UINT slot)
{
	m_Slot = slot;
	std::vector<DirectX::ScratchImage> images;
	images.resize(6);
	for (UINT i = 0; i < 6; i++) {
		std::wstring path = NarrowToWide(texDir + "\\" + std::to_string(i) + ".jpg");
		DirectX::ScratchImage image;
		DX_ASSERT(DirectX::LoadFromWICFile(path.c_str(), DirectX::WIC_FLAGS_IGNORE_SRGB, nullptr, image));
		images[i] = std::move(image);
	}

	D3D11_TEXTURE2D_DESC mapDesc = {};
	mapDesc.Width = images[0].GetMetadata().width;
	mapDesc.Height = images[0].GetMetadata().height;
	mapDesc.MipLevels = 1;
	mapDesc.ArraySize = 6;
	mapDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	mapDesc.SampleDesc.Count = 1;
	mapDesc.SampleDesc.Quality = 0;
	mapDesc.Usage = D3D11_USAGE_DEFAULT;
	mapDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	mapDesc.CPUAccessFlags = 0;
	mapDesc.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;

	D3D11_SUBRESOURCE_DATA data[6];
	for (UINT i = 0; i < 6; i++) {
		data[i].pSysMem = images[i].GetPixels();
		data[i].SysMemPitch = images[i].GetImage(0, 0, 0)->rowPitch;
		data[i].SysMemSlicePitch = 0;
	}

	ComPtr<ID3D11Texture2D> cubemap;
	DX_ASSERT(g.Device().CreateTexture2D(&mapDesc, data, &cubemap));

	D3D11_SHADER_RESOURCE_VIEW_DESC mapViewDesc = {};
	mapViewDesc.Format = mapDesc.Format;
	mapViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
	mapViewDesc.Texture2D.MostDetailedMip = 0;
	mapViewDesc.Texture2D.MipLevels = 1;
	DX_ASSERT(g.Device().CreateShaderResourceView(cubemap.Get(), &mapViewDesc, &m_View));
}

void Cubemap::Bind(Graphics& g)
{
	g.DC().PSSetShaderResources(m_Slot, 1, m_View.GetAddressOf());
}

std::string Cubemap::CreateHash(const std::string& texDir, UINT slot)
{
	return typeid(Cubemap).name() + "#"s + texDir + std::to_string(slot);
}

Shared<Cubemap> Cubemap::Evaluate(Graphics& g, const std::string& texDir, UINT slot)
{
	return Binds::Evaluate<Cubemap>(g, texDir, slot);
}

DepthCubemap::DepthCubemap(Graphics& g, UINT size, UINT slot)
	:m_Slot(slot)
{
	D3D11_TEXTURE2D_DESC textureDesc = {};
	textureDesc.Width = size;
	textureDesc.Height = size;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 6;
	textureDesc.Format = DXGI_FORMAT_R32_TYPELESS;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_DEPTH_STENCIL;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;
	// create the texture resource
	ComPtr<ID3D11Texture2D> pTexture;
	DX_ASSERT(g.Device().CreateTexture2D(
		&textureDesc, nullptr, &pTexture
	));

	// create the resource view on the texture
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = DXGI_FORMAT_R32_FLOAT;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = 1;
	DX_ASSERT(g.Device().CreateShaderResourceView(
		pTexture.Get(), &srvDesc, &m_View
	));

	// make depth buffer resources for capturing shadow map
	for (UINT face = 0; face < 6; face++)
	{
		m_DepthBuffs.push_back(MakeShared<WriteOnlyDepthStencil>(g, pTexture, face));
	}
}

void DepthCubemap::Bind(Graphics& g)
{
	g.DC().PSSetShaderResources(m_Slot, 1, m_View.GetAddressOf());
}

Shared<WriteOnlyDepthStencil> DepthCubemap::DepthBuff(size_t idx) const
{
	return m_DepthBuffs[idx];
}

std::string DepthCubemap::CreateHash(UINT size, UINT slot)
{
	return typeid(DepthCubemap).name() + "#"s + std::to_string(size) + " " + std::to_string(slot);
}

Shared<DepthCubemap> DepthCubemap::Evaluate(Graphics& g, UINT size, UINT slot)
{
	return Binds::Evaluate<DepthCubemap>(g, size, slot);
}
