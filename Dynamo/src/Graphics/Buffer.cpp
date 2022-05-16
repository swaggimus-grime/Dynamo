#include "dynamopch.h"
#include "Buffer.h"

void Buffer::Make(Graphics& g, D3D11_USAGE usage, D3D11_CPU_ACCESS_FLAG cpuAccess, D3D11_BIND_FLAG bufferType, SIZE_T size, LPCVOID data, UINT stride)
{
	m_Stride = stride;

	D3D11_SUBRESOURCE_DATA subResData;
	SecureZeroMemory(&subResData, sizeof(D3D11_SUBRESOURCE_DATA));
	subResData.pSysMem = data;
	subResData.SysMemPitch = 0;
	subResData.SysMemSlicePitch = 0;

	D3D11_BUFFER_DESC buffDesc = { 0 };
	buffDesc.Usage = usage;
	buffDesc.ByteWidth = size;
	buffDesc.BindFlags = bufferType;
	buffDesc.CPUAccessFlags = cpuAccess;
	buffDesc.MiscFlags = NULL;
	buffDesc.StructureByteStride = m_Stride;
	HRESULT hr = g.Device().CreateBuffer(&buffDesc, &subResData, &m_Buffer);
}

IndexBuffer::IndexBuffer(Graphics& g, const std::vector<UINT>& indices)
	:m_Count(indices.size())
{
	Make(g, D3D11_USAGE_DEFAULT, (D3D11_CPU_ACCESS_FLAG)NULL, D3D11_BIND_INDEX_BUFFER, indices.size() * sizeof(UINT), indices.data(), sizeof(UINT));
}

void IndexBuffer::Bind(Graphics& g) const
{
	g.DC().IASetIndexBuffer(m_Buffer.Get(), DXGI_FORMAT_R32_UINT, 0);
}

void VertexLayout::AddAttrib(LPCSTR name, DXGI_FORMAT format)
{
	D3D11_INPUT_ELEMENT_DESC attribDesc;
	SecureZeroMemory(&attribDesc, sizeof(D3D11_INPUT_ELEMENT_DESC));
	attribDesc.Format = format;
	attribDesc.SemanticName = name;
	attribDesc.AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	attribDesc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	push_back(attribDesc);
}

ConstantBuffer::ConstantBuffer(Graphics& g, SHADER_TYPE type, SIZE_T size, const void* data)
	:m_Type(type)
{
	Make(g, D3D11_USAGE_DYNAMIC, D3D11_CPU_ACCESS_WRITE, D3D11_BIND_CONSTANT_BUFFER, size, data, 0);
}

void ConstantBuffer::Update(Graphics& g, SIZE_T size, const void* data)
{
	D3D11_MAPPED_SUBRESOURCE subData{};
	g.DC().Map(m_Buffer.Get(), NULL, D3D11_MAP_WRITE_DISCARD, NULL, &subData);
	memcpy(subData.pData, data, size);
	g.DC().Unmap(m_Buffer.Get(), NULL);
}

void ConstantBuffer::Bind(Graphics& g) const
{
	if (m_Type == SHADER_TYPE::VS)
		g.DC().VSSetConstantBuffers(0, 1, m_Buffer.GetAddressOf());
	else
		g.DC().PSSetConstantBuffers(0, 1, m_Buffer.GetAddressOf());
}
