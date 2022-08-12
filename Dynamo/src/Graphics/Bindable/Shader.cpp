#include "dynamopch.h"
#include "Shader.h"

#include "Graphics.h"
#include <d3dcompiler.h>
#include <comdef.h>
#include "Binds.h"

VertexShader::VertexShader(Graphics& g, const std::string& path)
{
	DX_ASSERT(D3DReadFileToBlob(NarrowToWide(path).c_str(), &m_Code));
	DX_ASSERT(g.Device().CreateVertexShader(m_Code->GetBufferPointer(), m_Code->GetBufferSize(), nullptr, &m_Shader));
}

void VertexShader::Bind(Graphics& g)
{
	g.DC().VSSetShader(m_Shader.Get(), nullptr, 0);
}

std::string VertexShader::CreateHash(const std::string& path)
{
	return typeid(VertexShader).name() + "#"s + path;
}

Shared<VertexShader> VertexShader::Evaluate(Graphics& g, const std::string& path)
{
	return Binds::Evaluate<VertexShader>(g, path);
}

PixelShader::PixelShader(Graphics& g, const std::string& path)
{
	ComPtr<ID3D10Blob> code;
	DX_ASSERT(D3DReadFileToBlob(NarrowToWide(path).c_str(), &code));
	DX_ASSERT(g.Device().CreatePixelShader(code->GetBufferPointer(), code->GetBufferSize(), nullptr, &m_Shader));
}

void PixelShader::Bind(Graphics& g)
{
	g.DC().PSSetShader(m_Shader.Get(), nullptr, 0);
}

std::string PixelShader::CreateHash(const std::string& path)
{
	return typeid(PixelShader).name() + "#"s + path;
}

Shared<PixelShader> PixelShader::Evaluate(Graphics& g, const std::string& path)
{
	return Binds::Evaluate<PixelShader>(g, path);
}

NullPixelShader::NullPixelShader(Graphics& g)
{
}

void NullPixelShader::Bind(Graphics& g)
{
	g.DC().PSSetShader(nullptr, nullptr, 0);
}

std::string NullPixelShader::CreateHash()
{
	return typeid(NullPixelShader).name();
}

Shared<NullPixelShader> NullPixelShader::Evaluate(Graphics& g)
{
	return Binds::Evaluate<NullPixelShader>(g);
}