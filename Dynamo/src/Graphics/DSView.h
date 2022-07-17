#pragma once

#include "Graphics.h"
#include "Texture.h"

enum class DepthStencilMode {
	DEPTH_STENCIL,
	DEPTH_ONLY
};

class DepthStencilView {
public:
	DepthStencilView(Graphics& g, UINT width, UINT height, DepthStencilMode mode = DepthStencilMode::DEPTH_STENCIL);
	void Clear(Graphics& g);

	friend class RenderTarget;

protected:
	ComPtr<ID3D11DepthStencilView> m_DSV;
};

class ReadableDepthStencilView : public DepthStencilView {
public:
	ReadableDepthStencilView(Graphics& g, UINT width, UINT height, DepthStencilMode mode = DepthStencilMode::DEPTH_STENCIL, UINT slot = 0);
	void BindTexture(Graphics& g);

private:
	ComPtr<ID3D11ShaderResourceView> m_View;
	UINT m_Slot;
};