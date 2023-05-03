#pragma once

#include "Graphics.h"
#include "Debug/DynamoException.h"
#include "Bindable.h"
#include <DirectXTex.h>
#include "DepthStencil.h"

class Texture2D : public Bindable {
public:
	Texture2D() = default;
	Texture2D(Graphics& g, const std::string& path, UINT slot);
	virtual void Bind(Graphics& g) override;
	static std::string CreateHash(const std::string& path, UINT slot);
	static Shared<Texture2D> Evaluate(Graphics& g, const std::string& path, UINT slot);

private:
	static const DXGI_FORMAT Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	void CreateView(Graphics& g, DirectX::ScratchImage& image, ComPtr<ID3D11ShaderResourceView>& view, bool mipped);

protected:
	UINT m_Slot;
	ComPtr<ID3D11ShaderResourceView> m_View;
	ComPtr<ID3D11ShaderResourceView> m_MippedView;
};

class Cubemap : public Bindable {
public:
	Cubemap(Graphics& g, const std::string& texDir, UINT slot = 0);
	virtual void Bind(Graphics& g) override;
	static std::string CreateHash(const std::string& texDir, UINT slot);
	static Shared<Cubemap> Evaluate(Graphics& g, const std::string& texDir, UINT slot = 0);

private:
	UINT m_Slot;
	ComPtr<ID3D11ShaderResourceView> m_View;
};

class DepthCubemap : public Bindable {
public:
	DepthCubemap(Graphics& g, UINT size, UINT slot = 3);
	virtual void Bind(Graphics& g) override;
	Shared<WriteOnlyDepthStencil> DepthBuff(size_t idx) const;

	static std::string CreateHash(UINT size, UINT slot);
	static Shared<DepthCubemap> Evaluate(Graphics& g, UINT size, UINT slot = 3);

private:
	UINT m_Slot;
	bool m_Mipped;
	ComPtr<ID3D11ShaderResourceView> m_View;
	std::vector<Shared<WriteOnlyDepthStencil>> m_DepthBuffs;
};
