#pragma once

#include <d3d11.h>
#include "Bindable.h"

class Framebuffer : public Bindable {
public:
	friend class Graphics;
	Framebuffer(Graphics& g, UINT width, UINT height, BOOL depthEnable);
	virtual void Bind(Graphics& g) override;
	void Clear(Graphics& g);

private:
	void Reset(Graphics& g, UINT width, UINT height);

private:
	static constexpr XMFLOAT4 m_ClearColor = {0.f, 0.f, 0.f, 1.f};
	ComPtr<ID3D11RenderTargetView> m_ColorView;
	ComPtr<ID3D11DepthStencilState> m_DSState;
	ComPtr<ID3D11DepthStencilView> m_DSView;
	BOOL m_DepthEnable;
};