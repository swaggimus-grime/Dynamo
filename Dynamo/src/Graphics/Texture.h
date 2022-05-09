#pragma once

#include "GPU.h"
#include "Debug/DynamoException.h"

class Texture2D {
public:
	Texture2D(std::shared_ptr<GPU> gpu, LPCWSTR path, UINT slot);
	void Bind();
	static DXGI_FORMAT GetFormat(UINT numComponents);
	static UINT GetNumComponents(DXGI_FORMAT format);

	class Texture2DException : public DynamoException {
	public:
		//Texture2DException(const char* file, unsigned int line, HRESULT result);
		Texture2DException(const char* file, unsigned int line, const char* msg);
		virtual const char* GetType() const override;
		virtual const char* what() const override;
	};
private:
	std::shared_ptr<GPU> m_GPU;
	UINT m_Slot;
	ComPtr<ID3D11Texture2D> m_Texture;
	ComPtr<ID3D11ShaderResourceView> m_View;
};

#define TEX2D_EXCEP(msg) Texture2D::Texture2DException::Texture2DException(__FILE__, __LINE__, msg)