#pragma once

#include "Buffer.h"

class Graphics;

enum class DS_USAGE {
	DEPTH_STENCIL,
	DEPTH_ONLY
};

class DepthStencil : public Buffer {
public:
	friend class RenderTarget;

	virtual void BindBuffer(Graphics& g) override;
	virtual void BindBuffer(Graphics& g, Buffer* other) override;
	void BindBuffer(Graphics& g, RenderTarget* other);
	virtual void Clear(Graphics& g) override;

protected:
	DepthStencil(Graphics& gfx, ComPtr<ID3D11Texture2D> texture, UINT face);
	DepthStencil(Graphics& gfx, UINT width, UINT height, bool canBindShaderInput, DS_USAGE usage);

protected:
	ComPtr<ID3D11DepthStencilView> m_DSV;
	unsigned int m_Width;
	unsigned int m_Height;
};

class ReadableDepthStencil: public DepthStencil {
public:
	ReadableDepthStencil(Graphics& g, UINT slot, DS_USAGE usage = DS_USAGE::DEPTH_ONLY);
	ReadableDepthStencil(Graphics& g, UINT width, UINT height, UINT slot, DS_USAGE usage = DS_USAGE::DEPTH_ONLY);
	inline ID3D11ShaderResourceView* GetTexture() const { return m_SRV.Get(); }
	virtual void Bind(Graphics& g) override;

private:
	UINT m_Slot;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_SRV;
};

class WriteOnlyDepthStencil : public DepthStencil {
public:
	WriteOnlyDepthStencil(Graphics& g, ComPtr<ID3D11Texture2D> texture, UINT face);
	WriteOnlyDepthStencil(Graphics& g);
	WriteOnlyDepthStencil(Graphics& g, UINT width, UINT height);
	virtual void Bind(Graphics& gfx) override;
};