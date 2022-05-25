#include "dynamopch.h"
#include "Buffer.h"

#include <comdef.h>
void Buffer::Make(Graphics& g, D3D11_USAGE usage, D3D11_CPU_ACCESS_FLAG cpuAccess, D3D11_BIND_FLAG bufferType, SIZE_T size, LPCVOID data, UINT stride)
{
	m_Stride = stride;

	if (bufferType == D3D11_BIND_CONSTANT_BUFFER && size % 16 != 0)
		size += size / 16 ? (size % 16) : 16 - size;

	D3D11_BUFFER_DESC buffDesc = { 0 };
	buffDesc.Usage = usage;
	buffDesc.ByteWidth = size;
	buffDesc.BindFlags = bufferType;
	buffDesc.CPUAccessFlags = cpuAccess;
	buffDesc.MiscFlags = NULL;
	buffDesc.StructureByteStride = m_Stride;

	D3D11_SUBRESOURCE_DATA subResData{};
	if (data) {
		subResData.pSysMem = data;
		subResData.SysMemPitch = 0;
		subResData.SysMemSlicePitch = 0;
	}

	HRESULT hr = g.Device().CreateBuffer(&buffDesc, data ? &subResData : nullptr, &m_Buffer);
	_com_error err(hr);
	;
}

IndexBuffer::IndexBuffer(Graphics& g, const std::vector<UINT>& indices)
	:m_Count(indices.size())
{
	Make(g, D3D11_USAGE_DEFAULT, (D3D11_CPU_ACCESS_FLAG)NULL, D3D11_BIND_INDEX_BUFFER, indices.size() * sizeof(UINT), indices.data(), sizeof(UINT));
}

void IndexBuffer::Bind(Graphics& g)
{
	g.DC().IASetIndexBuffer(m_Buffer.Get(), DXGI_FORMAT_R32_UINT, 0);
}

void VertexLayout::AddAttrib(LPCSTR name, DXGI_FORMAT format)
{
	D3D11_INPUT_ELEMENT_DESC attribDesc;
	SecureZeroMemory(&attribDesc, sizeof(D3D11_INPUT_ELEMENT_DESC));
	attribDesc.Format = format;
	attribDesc.SemanticName = name;
	attribDesc.AlignedByteOffset = m_Offset;
	attribDesc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	m_Layouts.push_back(attribDesc);

	switch (format) {
	case DXGI_FORMAT_R32G32_FLOAT:
		m_Offset += 2 * sizeof(FLOAT);
		break;
	case DXGI_FORMAT_R32G32B32_FLOAT:
		m_Offset += 3 * sizeof(FLOAT);
		break;
	default:
		//TODO: Add exception for unknown format
		break;
	}
}

ConstantBuffer::ConstantBuffer(Graphics& g, SHADER_TYPE type, SIZE_T size, UINT slot, const void* data)
	:m_Type(type), m_Slot(slot)
{
	Make(g, D3D11_USAGE_DYNAMIC, D3D11_CPU_ACCESS_WRITE, D3D11_BIND_CONSTANT_BUFFER, size, data, 0);
}

void ConstantBuffer::Update(Graphics& g, SIZE_T size, const void* data) const
{
	D3D11_MAPPED_SUBRESOURCE subData{};
	g.DC().Map(m_Buffer.Get(), NULL, D3D11_MAP_WRITE_DISCARD, NULL, &subData);
	memcpy(subData.pData, data, size);
	g.DC().Unmap(m_Buffer.Get(), NULL);
}

void ConstantBuffer::Bind(Graphics& g)
{
	if (m_Type == SHADER_TYPE::VS)
		g.DC().VSSetConstantBuffers(m_Slot, 1, m_Buffer.GetAddressOf());
	else
		g.DC().PSSetConstantBuffers(m_Slot, 1, m_Buffer.GetAddressOf());
}
