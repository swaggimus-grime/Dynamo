#pragma once

#include "Bindable.h"
#include "Texture.h"
#include "DSView.h"
#include "Debug/DynamoException.h"

class RenderTarget : public Bindable {
protected:
	class RenderTargetException : public DynamoException {
	public:
		RenderTargetException(const char* file, unsigned int line, HRESULT result);
		virtual const char* GetType() const override;
		virtual const char* what() const override;
	};

public:
	RenderTarget(Graphics& g, UINT width, UINT height, UINT slot = 0);
	RenderTarget(Graphics& g, ID3D11Texture2D& texture, UINT slot = 0);

	virtual void Bind(Graphics& g) override;
	void Bind(Graphics& g, DSView& ds);

	void Clear(Graphics& g);
protected:
	static constexpr XMFLOAT4 m_ClearColor = { 0.f, 0.f, 0.f, 1.f };
	ComPtr<ID3D11RenderTargetView> m_RTV;
	
	DXGI_FORMAT m_Format;
};

class ReadWriteRenderTarget : public RenderTarget {
public:
	ReadWriteRenderTarget(Graphics& g, UINT width, UINT height, UINT slot = 0);
	void BindTexture(Graphics& g);
private:
	ComPtr<ID3D11ShaderResourceView> m_View;
	UINT m_Slot;
};

class WriteRenderTarget : public RenderTarget {
public:
	WriteRenderTarget(Graphics& g, ID3D11Texture2D& texture, UINT slot = 0);
	
};

#define RT_EXCEP(r) RenderTargetException::RenderTargetException(__FILE__, __LINE__, r)
#define RT_CHECK(result) { if(result) throw RT_EXCEP(result); }