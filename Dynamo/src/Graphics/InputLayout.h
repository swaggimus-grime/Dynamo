#pragma once

#include "Bindable.h"

class InputLayout : public Bindable {
public:
	InputLayout(Graphics& g, const class VertexData& vertData, const class VertexShader& vs);
	virtual void Bind(Graphics& g) override;
	static std::string CreateHash(const VertexData& vertData, const VertexShader& vs);
	static Shared<InputLayout> Evaluate(Graphics& g, const class VertexData& vertData, const class VertexShader& vs);
private:
	ComPtr<ID3D11InputLayout> m_Layout;

};