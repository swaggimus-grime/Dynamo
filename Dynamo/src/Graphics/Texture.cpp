#include "dynamopch.h"
#include "Texture.h"
#include <DirectXTex.h>
#include <comdef.h>

DXGI_FORMAT Texture2D::GetFormat(UINT numComponents)
{
	switch (numComponents) {
	case 1:
		return DXGI_FORMAT_R32_FLOAT;
	case 2:
		return DXGI_FORMAT_R32G32_FLOAT;
	case 3:
		return DXGI_FORMAT_R32G32B32_FLOAT;
	case 4:
		return DXGI_FORMAT_R32G32B32A32_FLOAT;
	default:
		return DXGI_FORMAT_UNKNOWN;
	}
}

UINT Texture2D::GetNumComponents(DXGI_FORMAT format)
{
	switch (format) {
	case DXGI_FORMAT_R32_FLOAT:
	case DXGI_FORMAT_R8_UNORM:
		return 1;
	case DXGI_FORMAT_R32G32_FLOAT:
	case DXGI_FORMAT_R8G8_UNORM:
		return 2;
	case DXGI_FORMAT_R32G32B32_FLOAT:
		return 3;
	case DXGI_FORMAT_R32G32B32A32_FLOAT:
	case DXGI_FORMAT_R8G8B8A8_UNORM:
		return 4;
	default:
		return DXGI_FORMAT_UNKNOWN;
	}
}

Texture2D::Texture2D(Graphics& g, LPCWSTR path, UINT slot)
	:m_Slot(slot)
{
	DirectX::ScratchImage image;
	DirectX::LoadFromWICFile(path, DirectX::WIC_FLAGS_IGNORE_SRGB, nullptr, image);
	if (!image.GetPixels())
		throw TEX2D_PREV_EXCEP;

	D3D11_TEXTURE2D_DESC texDesc = { 0 };
	texDesc.Width = image.GetMetadata().width;
	texDesc.Height = image.GetMetadata().height;
	texDesc.MipLevels = 1;
	texDesc.ArraySize = 1;
	texDesc.Format = image.GetMetadata().format;
	texDesc.Usage = D3D11_USAGE_DEFAULT;
	texDesc.SampleDesc.Count = 1;
	texDesc.SampleDesc.Quality = 0;
	texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	texDesc.CPUAccessFlags = 0;
	texDesc.MiscFlags = 0;

	ComPtr<ID3D11Texture2D> texture;
	D3D11_SUBRESOURCE_DATA data = { 0 };
	data.pSysMem = image.GetPixels();
	data.SysMemPitch = image.GetImage(0, 0, 0)->rowPitch;
	data.SysMemSlicePitch = 0;
	g.Device().CreateTexture2D(&texDesc, &data, &texture);

	D3D11_SHADER_RESOURCE_VIEW_DESC viewDesc = {};
	viewDesc.Format = texDesc.Format;
	viewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	viewDesc.Texture2D.MostDetailedMip = 0;
	viewDesc.Texture2D.MipLevels = 1;
	g.Device().CreateShaderResourceView(texture.Get(), &viewDesc, m_View.GetAddressOf());
}

void Texture2D::Bind(Graphics& g) const
{
	g.DC().PSSetShaderResources(m_Slot, 1, m_View.GetAddressOf());
}

Texture2D::Texture2DException::Texture2DException(const char* file, unsigned int line, HRESULT result)
	:DynamoException(file, line)
{
	_com_error err(result);
	std::stringstream s;
	s << __super::what() << std::endl << err.ErrorMessage();
	m_What = s.str();
}

const char* Texture2D::Texture2DException::GetType() const
{
	return "Texture2D Exception";
}

const char* Texture2D::Texture2DException::what() const
{
	return m_What.c_str();
}