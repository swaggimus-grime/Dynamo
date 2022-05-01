#include "dynamopch.h"
#include "Buffer.h"

template<class T> 
VertexBuffer::VertexBuffer(std::shared_ptr<GPU> gpu, std::vector<T> vertices)
{
	Make(gpu, D3D11_BIND_VERTEX_BUFFER, vertices.size(), vertices.data(), sizeof(T));
}

void VertexBuffer::Bind() const
{
	const UINT offset = 0;
	m_GPU->GetDC()->IASetVertexBuffers(0, 1, m_Buffer.GetAddressOf(), &m_Stride, &offset);
}

void Buffer::Make(std::shared_ptr<GPU> gpu, D3D11_BIND_FLAG bufferType, SIZE_T size, LPCVOID data, UINT stride)
{
	m_GPU = gpu;
	m_Stride = stride;

	D3D11_SUBRESOURCE_DATA subResData;
	SecureZeroMemory(&subResData, sizeof(D3D11_SUBRESOURCE_DATA));
	subResData.pSysMem = data;
	subResData.SysMemPitch = 0;
	subResData.SysMemSlicePitch = 0;
	D3D11_BUFFER_DESC buffDesc = { 0 };
	buffDesc.Usage = D3D11_USAGE_DEFAULT;
	buffDesc.ByteWidth = size;
	buffDesc.BindFlags = bufferType;
	buffDesc.CPUAccessFlags = NULL;
	buffDesc.MiscFlags = NULL;
	buffDesc.StructureByteStride = m_Stride;
	HRESULT hr = m_GPU->GetDevice()->CreateBuffer(&buffDesc, &subResData, &m_Buffer);
}

IndexBuffer::IndexBuffer(std::shared_ptr<GPU> gpu, SIZE_T size, LPCVOID data, UINT stride)
{
	Make(gpu, D3D11_BIND_INDEX_BUFFER, size, data, stride);
}

void IndexBuffer::Bind() const
{
	m_GPU->GetDC()->IASetIndexBuffer(m_Buffer.Get(), DXGI_FORMAT_R32_UINT, 0);
}