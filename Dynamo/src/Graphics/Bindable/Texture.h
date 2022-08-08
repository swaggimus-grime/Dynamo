#pragma once

#include "Graphics.h"
#include "Debug/DynamoException.h"
#include "Bindable.h"

class Texture2D : public Bindable {
public:
	Texture2D() = default;
	Texture2D(Graphics& g, const std::string& path, UINT slot);
	virtual void Bind(Graphics& g) override;
	virtual void Reload(Graphics& g, const std::wstring& newPath);
	static std::string CreateHash(const std::string& path, UINT slot);
	static Shared<Texture2D> Evaluate(Graphics& g, const std::string& path, UINT slot);
private:
	static const DXGI_FORMAT Format = DXGI_FORMAT_B8G8R8A8_UNORM;

protected:
	UINT m_Slot;
	ComPtr<ID3D11ShaderResourceView> m_View;
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
