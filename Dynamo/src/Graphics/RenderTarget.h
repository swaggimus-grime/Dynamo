#pragma once

#include "Bindable.h"
#include "Texture.h"
#include "DSView.h"
#include "Debug/DynamoException.h"

class RenderTarget : public Bindable {
public:
	RenderTarget(Graphics& g, UINT width, UINT height, UINT slot = 0);
	RenderTarget(Graphics& g, ID3D11Texture2D& texture, UINT slot = 0);

	virtual void Bind(Graphics& g) override;
	void Bind(Graphics& g, DepthStencilView& ds);
	void Clear(Graphics& g);

	static std::string CreateHash(ID3D11Texture2D& texture, UINT slot);
	static std::string CreateHash(UINT width, UINT height, UINT slot);
protected:
	static constexpr XMFLOAT4 m_ClearColor = { 0.f, 0.f, 0.f, 1.f };
	ComPtr<ID3D11RenderTargetView> m_RTV;
	
	DXGI_FORMAT m_Format;
};

class ReadableRenderTarget : public RenderTarget {
public:
	ReadableRenderTarget(Graphics& g, UINT width, UINT height, UINT slot = 0);
	void BindTexture(Graphics& g);

private:
	ComPtr<ID3D11ShaderResourceView> m_View;
	UINT m_Slot;
};
