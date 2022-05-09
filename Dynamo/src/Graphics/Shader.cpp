#include "dynamopch.h"
#include "Shader.h"
#include <d3dcompiler.h>
#include <comdef.h>

Shader::Shader(std::shared_ptr<GPU> gpu, LPCWSTR vertPath, LPCWSTR pixPath)
	:m_GPU(gpu)
{
	ID3D10Blob *pixCode;
	SHADER_CHECK(D3DReadFileToBlob(vertPath, m_VSCode.GetAddressOf()));
	SHADER_CHECK(D3DReadFileToBlob(pixPath, &pixCode));
	SHADER_CHECK(m_GPU->GetDevice()->CreateVertexShader(m_VSCode->GetBufferPointer(), m_VSCode->GetBufferSize(), nullptr, m_VS.GetAddressOf()));
	SHADER_CHECK(m_GPU->GetDevice()->CreatePixelShader(pixCode->GetBufferPointer(), pixCode->GetBufferSize(), nullptr, m_PS.GetAddressOf()));
	pixCode->Release();
}

Shader::~Shader()
{
}

void Shader::Bind()
{
	m_GPU->GetDC()->VSSetShader(m_VS.Get(), nullptr, 0);
	m_GPU->GetDC()->PSSetShader(m_PS.Get(), nullptr, 0);

	/*if(!m_VSCBuffs.empty())
		m_GPU->GetDC()->VSSetConstantBuffers(0, m_VSCBuffs.size(), m_VSCBuffs.GetArray());
	if(!m_PSCBuffs.empty())
		m_GPU->GetDC()->PSSetConstantBuffers(0, m_PSCBuffs.size(), m_PSCBuffs.GetArray());*/
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