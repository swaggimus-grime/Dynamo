#pragma once

#include "Graphics.h"
#include "Debug/DynamoException.h"
#include "Bindable.h"

class Texture2D : public Bindable {
public:
	Texture2D() = default;
	Texture2D(Graphics& g, LPCWSTR path, UINT slot);
	virtual void Bind(Graphics& g) const override;
	static DXGI_FORMAT GetFormat(UINT numComponents);
	static UINT GetNumComponents(DXGI_FORMAT format);

	class Texture2DException : public DynamoException {
	public:
		Texture2DException(const char* file, unsigned int line, HRESULT result);
		virtual const char* GetType() const override;
		virtual const char* what() const override;
	};

protected:
	UINT m_Slot;
	ComPtr<ID3D11ShaderResourceView> m_View;
};

#define TEX2D_PREV_EXCEP Texture2D::Texture2DException::Texture2DException(__FILE__, __LINE__, GetLastError())
#define TEX2D_EXCEP(res) Texture2D::Texture2DException::Texture2DException(__FILE__, __LINE__, res)
#define TEX2D_ASSERT(hr) { if(hr != S_OK) throw TEX2D_EXCEP(hr); }