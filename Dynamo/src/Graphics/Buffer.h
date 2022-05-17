#pragma once

#include <d3d11.h>
#include <wrl.h>
#include "Graphics.h"
#include "Bindable.h"

class Buffer : public Bindable {
public:
	Buffer() = default;
protected:
	void Make(Graphics& g, D3D11_USAGE usage, D3D11_CPU_ACCESS_FLAG cpuAccess, D3D11_BIND_FLAG bufferType, SIZE_T size, LPCVOID data, UINT stride);
protected:
	ComPtr<ID3D11Buffer> m_Buffer;
	UINT m_Stride;
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
	VertexBuffer(Graphics& g, const std::vector<T>& vertices, ID3D10Blob& vsCode, VertexLayout& layout)
	{
		Make(g, D3D11_USAGE_DEFAULT, (D3D11_CPU_ACCESS_FLAG)NULL, D3D11_BIND_VERTEX_BUFFER, vertices.size() * sizeof(T), vertices.data(), sizeof(T));
		g.Device().CreateInputLayout(layout.data(), layout.size(), vsCode.GetBufferPointer(), vsCode.GetBufferSize(), m_Layout.GetAddressOf());
	}

	virtual void Bind(Graphics& g) const override
	{
		const UINT offset = 0;
		g.DC().IASetVertexBuffers(0, 1, m_Buffer.GetAddressOf(), &m_Stride, &offset);
		g.DC().IASetInputLayout(m_Layout.Get());
	}

private:
	ComPtr<ID3D11InputLayout> m_Layout;
};

class IndexBuffer : public Buffer {
public:
	IndexBuffer() = delete;
	IndexBuffer(Graphics& g, const std::vector<UINT>& indices);
	inline UINT Size() const { return m_Count; }
	virtual void Bind(Graphics& g) const override;

private:
	UINT m_Count;
};

enum class SHADER_TYPE {
	VS,
	PS
};

class ConstantBuffer : public Buffer {
public:
	ConstantBuffer() = delete;
	ConstantBuffer(Graphics& g, SHADER_TYPE type, SIZE_T size, const void* data = nullptr);
	void Update(Graphics& g, SIZE_T size, const void* data) const;
	virtual void Bind(Graphics& g) const override;
	inline ID3D11Buffer* GetBufferPointer() const { return m_Buffer.Get(); };

private:
	SHADER_TYPE m_Type;
};