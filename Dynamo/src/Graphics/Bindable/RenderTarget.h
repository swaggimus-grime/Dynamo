#pragma once

#include "Buffer.h"

class Graphics;
class DepthStencil;

class RenderTarget : public Buffer {
public:
	virtual void BindBuffer(Graphics& g) override;
	virtual void BindBuffer(Graphics& g, Buffer* depthStencil) override;
	virtual void BindBuffer(Graphics& g, DepthStencil* depthStencil);
	void Clear(Graphics& gfx) override;
	void Clear(Graphics& gfx, const std::array<float, 4>& color);
	inline UINT Width() const { return m_Width; }
	inline UINT Height() const { return m_Height; }
	inline ID3D11RenderTargetView* GetTarget() const { return m_RTV.Get(); }
	inline ID3D11Texture2D* GetTextureFrFr() const { return m_Texture.Get(); }
private:
	void BindBuffer(Graphics& g, ID3D11DepthStencilView* pDepthStencilView);

protected:
	RenderTarget(Graphics& gfx, ID3D11Texture2D* pTexture, std::optional<UINT> face);
	RenderTarget(Graphics& gfx, UINT width, UINT height, bool canBindShaderInput);

protected:
	UINT m_Width;
	UINT m_Height;
	ComPtr<ID3D11RenderTargetView> m_RTV;
	ComPtr<ID3D11Texture2D> m_Texture;
};

class ReadableRenderTarget : public RenderTarget {
public:
	ReadableRenderTarget(Graphics& g, UINT width, UINT height, UINT slot);
	void Bind(Graphics& g) override;
	inline ID3D11ShaderResourceView* GetTexture() const { return m_SRV.Get(); }

private:
	UINT m_Slot;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_SRV;
};

class WriteOnlyRenderTarget : public RenderTarget
{
public:
	void Bind(Graphics& g) override;
	WriteOnlyRenderTarget(Graphics& g, ID3D11Texture2D* pTexture, std::optional<UINT> face = {});
	WriteOnlyRenderTarget(Graphics& g, UINT width, UINT height);
	inline ID3D11ShaderResourceView* GetView() const { return m_SRV.Get(); }

private:
	ComPtr<ID3D11ShaderResourceView> m_SRV;
};