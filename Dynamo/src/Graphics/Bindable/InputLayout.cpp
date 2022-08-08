#include "InputLayout.h"
#include "InputLayout.h"
#include "dynamopch.h"
#include "InputLayout.h"

#include "Shader.h"
#include "VertexBuffer.h"
#include "Shader.h"
#include "Binds.h"

InputLayout::InputLayout(Graphics& g, const VertexLayout& layout, const VertexShader& vs)
{
	g.Device().CreateInputLayout(layout.Data(), layout.Size(), vs.Code().GetBufferPointer(), vs.Code().GetBufferSize(), &m_Layout);
}

void InputLayout::Bind(Graphics& g)
{
	g.DC().IASetInputLayout(m_Layout.Get());
}

std::string InputLayout::CreateHash(const  VertexLayout& layout, const VertexShader& vs)
{
	return typeid(InputLayout).name() + "#"s + std::to_string(layout.Size()) + std::to_string(vs.Code().GetBufferSize());
}

Shared<InputLayout> InputLayout::Evaluate(Graphics& g, const VertexLayout& layout, const VertexShader& vs)
{
	return Binds::Evaluate<InputLayout>(g, layout, vs);
}
