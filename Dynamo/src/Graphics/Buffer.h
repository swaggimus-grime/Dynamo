#pragma once

#include <d3d11.h>
#include <wrl.h>
#include "GPU.h"


class Buffer {
public:
	Buffer() = default;
	virtual void Bind() const = 0;
protected:
	void Make(std::shared_ptr<GPU> gpu, D3D11_USAGE usage, D3D11_CPU_ACCESS_FLAG cpuAccess, D3D11_BIND_FLAG bufferType, SIZE_T size, LPCVOID data, UINT stride);
protected:
	ComPtr<ID3D11Buffer> m_Buffer;
	UINT m_Stride;
	std::shared_ptr<GPU> m_GPU;
};

class VertexLayout : public std::vector<D3D11_INPUT_ELEMENT_DESC> {
public:
	VertexLayout() = default;
	void AddAttrib(LPCSTR name, DXGI_FORMAT format);
};

template<class T>
class VertexBuffer : public Buffer {
public:
	VertexBuffer() = delete;
	VertexBuffer(std::shared_ptr<GPU> gpu, std::vector<T> vertices, ID3D10Blob& vsCode, VertexLayout& layout)
	{
		Make(gpu, D3D11_USAGE_DEFAULT, (D3D11_CPU_ACCESS_FLAG)NULL, D3D11_BIND_VERTEX_BUFFER, vertices.size() * sizeof(T), vertices.data(), sizeof(T));
		m_GPU->GetDevice()->CreateInputLayout(layout.data(), layout.size(), vsCode.GetBufferPointer(), vsCode.GetBufferSize(), m_Layout.GetAddressOf());
	}

	void VertexBuffer::Bind() const
	{
		const UINT offset = 0;
		m_GPU->GetDC()->IASetVertexBuffers(0, 1, m_Buffer.GetAddressOf(), &m_Stride, &offset);
		m_GPU->GetDC()->IASetInputLayout(m_Layout.Get());
	}


private:
	ComPtr<ID3D11InputLayout> m_Layout;
};

class IndexBuffer : public Buffer {
public:
	IndexBuffer() = delete;
	IndexBuffer(std::shared_ptr<GPU> gpu, std::vector<UINT> indices);
	inline UINT Size() const { return m_Count; }
	virtual void Bind() const override;

private:
	UINT m_Count;
};

class ConstantBuffer : public Buffer {
public:
	ConstantBuffer() = delete;
	ConstantBuffer(std::shared_ptr<GPU> gpu, SIZE_T size, const void* data);
	virtual void Bind() const override;
	friend class CBuffContainer;
	inline ID3D11Buffer* GetBufferPointer() const { return m_Buffer.Get(); };
};