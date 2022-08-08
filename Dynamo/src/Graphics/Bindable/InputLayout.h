#pragma once

#include "Bindable.h"
#include <d3d11.h>

class InputLayout : public Bindable {
public:
	InputLayout(Graphics& g, const class VertexLayout& layout, const class VertexShader& vs);
	virtual void Bind(Graphics& g) override;
	static std::string CreateHash(const VertexLayout& layout, const VertexShader& vs);
	static Shared<InputLayout> Evaluate(Graphics& g, const VertexLayout& layout, const class VertexShader& vs);
private:
	ComPtr<ID3D11InputLayout> m_Layout;

};