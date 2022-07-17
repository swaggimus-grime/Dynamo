#include "dynamopch.h"
#include "IndexBuffer.h"

#include "Graphics.h"

IndexBuffer::IndexBuffer(Graphics& g, const std::vector<UINT>& indices)
	:Bindable(CreateHash(indices)), m_Count(indices.size())
{
	D3D11_BUFFER_DESC buffDesc = {};
	buffDesc.Usage = D3D11_USAGE_DEFAULT;
	buffDesc.ByteWidth = indices.size() * sizeof(UINT);
	buffDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	buffDesc.CPUAccessFlags = NULL;
	buffDesc.MiscFlags = NULL;
	buffDesc.StructureByteStride = sizeof(UINT);

	D3D11_SUBRESOURCE_DATA subResData{};
	subResData.pSysMem = indices.data();
	subResData.SysMemPitch = 0;
	subResData.SysMemSlicePitch = 0;

	g.Device().CreateBuffer(&buffDesc, &subResData, &m_Buff);
}

void IndexBuffer::Bind(Graphics& g) 
{
	g.DC().IASetIndexBuffer(m_Buff.Get(), DXGI_FORMAT_R32_UINT, 0);
}

std::string IndexBuffer::CreateHash(const std::vector<UINT>& indices)
{
	return typeid(IndexBuffer).name() + "#"s + std::to_string(indices.size());
}
