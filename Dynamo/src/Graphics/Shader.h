#pragma once

#include "Graphics.h"
#include "Debug/DynamoException.h"
#include <unordered_map>
#include "Bindable.h"

class Shader : public Bindable {
public:
	Shader(Graphics& g, LPCWSTR vertPath, LPCWSTR pixPath);
	~Shader();
	virtual void Bind(Graphics& g) const override;
	inline ID3D10Blob& GetVSCode() const { return *m_VSCode.Get(); }
public:
	class ShaderException : public DynamoException {
	public:
		ShaderException(const char* file, unsigned int line, HRESULT result);
		virtual const char* GetType() const override;
		virtual const char* what() const override;
	};

private:
	ComPtr<ID3D11VertexShader> m_VS;
	ComPtr<ID3D11PixelShader> m_PS;
	ComPtr<ID3D10Blob> m_VSCode;
};

#define SHADER_EXCEP(result) Shader::ShaderException(__FILE__, __LINE__, result)
#define SHADER_CHECK(result) { if(result) throw SHADER_EXCEP(result); }