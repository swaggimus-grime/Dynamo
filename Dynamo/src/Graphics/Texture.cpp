#include "dynamopch.h"
#include "Texture.h"
#include <DirectXTex.h>
#include <comdef.h>

Texture2D::Texture2D(Graphics& g, LPCWSTR path, UINT slot)
	:m_Slot(slot)
{
	DirectX::ScratchImage image;
	DirectX::LoadFromWICFile(path, DirectX::WIC_FLAGS_IGNORE_SRGB, nullptr, image);
	if (!image.GetPixels())
		throw TEX2D_PREV_EXCEP;

	if (image.GetMetadata().format != m_Format) {
		DirectX::ScratchImage converted;
		DirectX::Convert(
			*image.GetImage(0, 0, 0),
			m_Format,
			DirectX::TEX_FILTER_DEFAULT,
			DirectX::TEX_THRESHOLD_DEFAULT,
			converted
		);
		image = std::move(converted);
	}

	D3D11_TEXTURE2D_DESC texDesc;
	SecureZeroMemory(&texDesc, sizeof(D3D11_TEXTURE2D_DESC));
	texDesc.Width = image.GetMetadata().width;
	texDesc.Height = image.GetMetadata().height;
	texDesc.MipLevels = 1;
	texDesc.ArraySize = 1;
	texDesc.Format = m_Format;
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

	D3D11_SHADER_RESOURCE_VIEW_DESC viewDesc;
	SecureZeroMemory(&viewDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
	viewDesc.Format = texDesc.Format;
	viewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	viewDesc.Texture2D.MostDetailedMip = 0;
	viewDesc.Texture2D.MipLevels = 1;
	g.Device().CreateShaderResourceView(texture.Get(), &viewDesc, m_View.GetAddressOf());
}

void Texture2D::Bind(Graphics& g)
{
	g.DC().PSSetShaderResources(m_Slot, 1, m_View.GetAddressOf());
}

void Texture2D::Reload(Graphics& g, const std::wstring& newPath)
{
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