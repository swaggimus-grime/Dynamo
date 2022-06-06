#pragma once

#include "Graphics.h"

class DSView {
public:
	DSView(Graphics& g, UINT width, UINT height);
	void Clear(Graphics& g);

	friend class RenderTarget;

private:
	ComPtr<ID3D11DepthStencilView> m_DSV;
};