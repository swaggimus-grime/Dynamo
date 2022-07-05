#pragma once

#include "Graphics.h"
#include "Texture.h"

class DSView {
public:
	DSView(Graphics& g, UINT width, UINT height, UINT slot = 0);
	void Clear(Graphics& g);

	friend class RenderTarget;

private:
	ComPtr<ID3D11DepthStencilView> m_DSV;
};

//class WriteDSView : public DSView {
//public:
//	DSView(Graphics& g, UINT width, UINT height, UINT slot = 0);
//};