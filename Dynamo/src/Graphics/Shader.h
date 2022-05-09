#pragma once

#include "GPU.h"
#include "Debug/DynamoException.h"
#include <unordered_map>
#include "Buffer.h"

enum class SHADER_TYPE {
	VERTEX_SHADER,
	PIXEL_SHADER
};

class Shader {
public:
	Shader(std::shared_ptr<GPU> gpu, LPCWSTR vertPath, LPCWSTR pixPath);
	~Shader();
	void Bind();
	inline ID3D10Blob& GetVSCode() const { return *m_VSCode.Get(); }
public:
	class ShaderException : public DynamoException {
	public:
		ShaderException(const char* file, unsigned int line, HRESULT result);
		virtual const char* GetType() const override;
		virtual const char* what() const override;
	};

private:
	std::shared_ptr<GPU> m_GPU;
	ComPtr<ID3D11VertexShader> m_VS;
	ComPtr<ID3D11PixelShader> m_PS;
	ComPtr<ID3D10Blob> m_VSCode;
	
};

#define SHADER_EXCEP(result) Shader::ShaderException(__FILE__, __LINE__, result)
#define SHADER_CHECK(result) { if(result) throw SHADER_EXCEP(result); }