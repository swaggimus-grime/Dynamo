#include "dynamopch.h"
#include "Buffer.h"

void Buffer::Make(std::shared_ptr<GPU> gpu, D3D11_USAGE usage, D3D11_CPU_ACCESS_FLAG cpuAccess, D3D11_BIND_FLAG bufferType, SIZE_T size, LPCVOID data, UINT stride)
{
	m_Stride = stride;
	m_GPU = gpu;

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
	HRESULT hr = m_GPU->GetDevice()->CreateBuffer(&buffDesc, &subResData, &m_Buffer);
}

IndexBuffer::IndexBuffer(std::shared_ptr<GPU> gpu, std::vector<UINT> indices)
	:m_Count(indices.size())
{
	Make(gpu, D3D11_USAGE_DEFAULT, (D3D11_CPU_ACCESS_FLAG)NULL, D3D11_BIND_INDEX_BUFFER, indices.size() * sizeof(UINT), indices.data(), sizeof(UINT));
}

void IndexBuffer::Bind() const
{
	m_GPU->GetDC()->IASetIndexBuffer(m_Buffer.Get(), DXGI_FORMAT_R32_UINT, 0);
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


void ConstantBuffer::Bind() const
{

}

ConstantBuffer::ConstantBuffer(std::shared_ptr<GPU> gpu, SIZE_T size, const void* data)
{
	Make(gpu, D3D11_USAGE_DYNAMIC, D3D11_CPU_ACCESS_WRITE, D3D11_BIND_CONSTANT_BUFFER, size, data, 0);
}
