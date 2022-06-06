#pragma once

#include "Bindable.h"
#include "Texture.h"
#include "DSView.h"

class RenderTarget : public Bindable {
private:
	class RTTexture : public Texture2D {
	public:
		RTTexture(Graphics& g, class RenderTarget& r, UINT slot);
	};

public:
	RenderTarget(Graphics& g, UINT width, UINT height, UINT slot = 0);
	RenderTarget(Graphics& g, ID3D11Texture2D& texture, UINT slot = 0);
	virtual void Bind(Graphics& g) override;
	void Bind(Graphics& g, DSView& ds);
	void Clear(Graphics& g);
	inline Texture2D& Tex() const { return *m_Tex; }

private:
	static constexpr XMFLOAT4 m_ClearColor = { 0.f, 0.f, 0.f, 1.f };
	ComPtr<ID3D11RenderTargetView> m_RTV;
	std::unique_ptr<Texture2D> m_Tex;
};