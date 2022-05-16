#include "dynamopch.h"
#include "Skybox.h"
#include "Texture.h"
#include <DirectXTex.h>

#define CUBEMAP_NUM_FACES 6

Cubemap::Cubemap(Graphics& g, const std::wstring& texDir, UINT slot)
{
	m_Slot = slot;
	std::vector<DirectX::ScratchImage> images;
	images.resize(CUBEMAP_NUM_FACES);
	for (UINT i = 0; i < CUBEMAP_NUM_FACES; i++) {
		std::wstring path = texDir + L"/" + std::to_wstring(i) + L".jpg";
		DirectX::ScratchImage image;
		TEX2D_ASSERT(DirectX::LoadFromWICFile(path.c_str(), DirectX::WIC_FLAGS_IGNORE_SRGB, nullptr, image));
		images[i] = std::move(image);
	}
	
	D3D11_TEXTURE2D_DESC mapDesc = {};
	mapDesc.Width = images[0].GetMetadata().width;
	mapDesc.Height = images[0].GetMetadata().height;
	mapDesc.MipLevels = 1;
	mapDesc.ArraySize = CUBEMAP_NUM_FACES;
	mapDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	mapDesc.SampleDesc.Count = 1;
	mapDesc.SampleDesc.Quality = 0;
	mapDesc.Usage = D3D11_USAGE_DEFAULT;
	mapDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	mapDesc.CPUAccessFlags = 0;
	mapDesc.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;

	D3D11_SUBRESOURCE_DATA data[CUBEMAP_NUM_FACES];
	for (UINT i = 0; i < CUBEMAP_NUM_FACES; i++) {
		data[i].pSysMem = images[i].GetPixels();
		data[i].SysMemPitch = images[i].GetImage(0, 0, 0)->rowPitch;
		data[i].SysMemSlicePitch = 0;
	}

	ComPtr<ID3D11Texture2D> cubemap;
	TEX2D_ASSERT(g.Device().CreateTexture2D(&mapDesc, data, &cubemap));

	D3D11_SHADER_RESOURCE_VIEW_DESC mapViewDesc = {};
	mapViewDesc.Format = mapDesc.Format;
	mapViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
	mapViewDesc.Texture2D.MostDetailedMip = 0;
	mapViewDesc.Texture2D.MipLevels = 1;
	g.Device().CreateShaderResourceView(cubemap.Get(), &mapViewDesc, &m_View);
}

std::vector<XMFLOAT3> verts = {
	{ -.5f,-.5f,-.5f },
	{ .5f,-.5f,-.5f },
	{ -.5f,.5f,-.5f },
	{ .5f,.5f,-.5f },
	{ -.5f,-.5f,.5f },
	{ .5f,-.5f,.5f },
	{ -.5f,.5f,.5f },
	{ .5f,.5f,.5f }
};

std::vector<UINT> inds = {
	0,2,1, 2,3,1,
	1,3,5, 3,7,5,
	2,6,3, 3,6,7,
	4,5,7, 4,7,6,
	0,4,2, 2,4,6,
	0,1,4, 1,5,4
};
Skybox::Skybox(Graphics& g, const std::wstring& texDir, std::shared_ptr<Shader> shader)
	:m_Shader(shader)
{
	
	VertexLayout layout;
	layout.AddAttrib("Pos", DXGI_FORMAT_R32G32B32_FLOAT);
	m_Cube.VBuff = std::make_shared<VertexBuffer<XMFLOAT3>>(g, verts, m_Shader->GetVSCode(), layout);
	m_Cube.IBuff = std::make_shared<IndexBuffer>(g, inds);
	m_Cube.Textures.push_back(std::make_shared<Cubemap>(g, texDir, 0));
}

void Skybox::Render(Graphics& g) const
{
	m_Cube.VBuff->Bind(g);
	m_Cube.IBuff->Bind(g);
	m_Cube.Textures[0]->Bind(g);
	g.DC().DrawIndexed(m_Cube.IBuff->Size(), 0, 0);
}
