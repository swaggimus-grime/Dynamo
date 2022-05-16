#pragma once

#include <d3d11.h>
#include "Bindable.h"

class Framebuffer : public Bindable {
public:
	Framebuffer(Graphics& g, UINT width, UINT height, BOOL depthEnable);
	virtual void Bind(Graphics& g) const override;
	void Clear(Graphics& g);
private:
	static constexpr XMVECTOR clearColor = {0.f, 0.f, 0.f, 1.f};
	ComPtr<ID3D11RenderTargetView> m_ColorView;
	ComPtr<ID3D11DepthStencilState> m_DSState;
	ComPtr<ID3D11DepthStencilView> m_DSView;
};