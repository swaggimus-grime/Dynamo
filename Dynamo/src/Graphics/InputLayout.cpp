#include "InputLayout.h"
#include "InputLayout.h"
#include "dynamopch.h"
#include "InputLayout.h"

#include "Shader.h"
#include "VertexBuffer.h"
#include "Shader.h"
#include "Binds.h"

InputLayout::InputLayout(Graphics& g, const VertexData& vertData, const VertexShader& vs)
	:Bindable(CreateHash(vertData, vs))
{
	g.Device().CreateInputLayout(vertData.Layout().Data(), vertData.Layout().Size(), vs.Code().GetBufferPointer(), vs.Code().GetBufferSize(), &m_Layout);
}

void InputLayout::Bind(Graphics& g)
{
	g.DC().IASetInputLayout(m_Layout.Get());
}

std::string InputLayout::CreateHash(const VertexData& vertData, const VertexShader& vs)
{
	return typeid(InputLayout).name() + "#"s + std::to_string(vertData.Size()) + std::to_string(vs.Code().GetBufferSize());
}

Shared<InputLayout> InputLayout::Evaluate(Graphics& g, const VertexData& vertData, const VertexShader& vs)
{
	return Binds::Evaluate<InputLayout>(g, vertData, vs);
}
