#include "dynamopch.h"
#include "Shader.h"
#include <d3dcompiler.h>
#include <comdef.h>

Shader::Shader(Graphics& g, LPCWSTR vertPath, LPCWSTR pixPath)
{
	ID3D10Blob *pixCode;
	SHADER_CHECK(D3DReadFileToBlob(vertPath, m_VSCode.GetAddressOf()));
	SHADER_CHECK(D3DReadFileToBlob(pixPath, &pixCode));
	SHADER_CHECK(g.Device().CreateVertexShader(m_VSCode->GetBufferPointer(), m_VSCode->GetBufferSize(), nullptr, m_VS.GetAddressOf()));
	SHADER_CHECK(g.Device().CreatePixelShader(pixCode->GetBufferPointer(), pixCode->GetBufferSize(), nullptr, m_PS.GetAddressOf()));
	pixCode->Release();
}

Shader::~Shader()
{
}

void Shader::Bind(Graphics& g)
{
	g.DC().VSSetShader(m_VS.Get(), nullptr, 0);
	g.DC().PSSetShader(m_PS.Get(), nullptr, 0);
}

Shader::ShaderException::ShaderException(const char* file, unsigned int line, HRESULT result)
	:DynamoException(file, line)
{
	_com_error err(result);
	std::stringstream s;
	s << __super::what() << std::endl << err.ErrorMessage();
	m_What = s.str();
}

const char* Shader::ShaderException::GetType() const
{
	return "Shader Exception";
}

const char* Shader::ShaderException::what() const
{
	return m_What.c_str();
}