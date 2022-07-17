#pragma once

#include "Graphics.h"
#include "Debug/DynamoException.h"
#include <unordered_map>
#include "Bindable.h"

class VertexShader : public Bindable {
public:
	VertexShader(Graphics& g, const std::string& path);
	ID3D10Blob& Code() const { return *(m_Code.Get()); }
	virtual void Bind(Graphics& g) override;
	static std::string CreateHash(const std::string& path);
	static Shared<VertexShader> Evaluate(Graphics& g, const std::string& path);
private:
	ComPtr<ID3D11VertexShader> m_Shader;
	ComPtr<ID3D10Blob> m_Code;
};

class PixelShader : public Bindable {
public:
	PixelShader(Graphics& g, const std::string& path);
	virtual void Bind(Graphics& g) override;
	static std::string CreateHash(const std::string& path);
	static Shared<PixelShader> Evaluate(Graphics& g, const std::string& path);
private:
	ComPtr<ID3D11PixelShader> m_Shader;
};
