#pragma once

#include <d3d11.h>
#include <wrl.h>
#include "GPU.h"

class Buffer {
public:
	Buffer() = default;
	virtual void Bind() const = 0;
protected:
	void Make(std::shared_ptr<GPU> gpu, D3D11_BIND_FLAG bufferType, SIZE_T size, LPCVOID data, UINT stride);
protected:
	ComPtr<ID3D11Buffer> m_Buffer;
	UINT m_Stride;
	std::shared_ptr<GPU> m_GPU;
};

class VertexBuffer : public Buffer {
public:
	template<class T>
	VertexBuffer(std::shared_ptr<GPU> gpu, std::vector<T> vertices);
	virtual void Bind() const override;
};

class IndexBuffer : public Buffer {
public:
	IndexBuffer(std::shared_ptr<GPU> gpu, SIZE_T size, LPCVOID data, UINT stride);
	virtual void Bind() const override;
};