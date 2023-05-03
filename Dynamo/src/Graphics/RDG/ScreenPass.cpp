#include "dynamopch.h"
#include "ScreenPass.h"

#include "Bindable/VertexBuffer.h"

ScreenPass::ScreenPass(Graphics& g, const std::string& name)
	:BindPass(name)
{
	VertexLayout planeLayout(ATTRIB_POS);
	VertexData plane(planeLayout, 4);
	plane.Pos(0) = { -1, 1, 0 };
	plane.Pos(1) = { 1, 1, 0 };
	plane.Pos(2) = { -1, -1, 0 };
	plane.Pos(3) = { 1, -1, 0 };
	AddBind(VertexBuffer::Evaluate(g, "$ScreenPlane", std::move(plane)));
	std::vector<UINT> indices = { 0, 1, 2, 1, 3, 2 };
	AddBind(IndexBuffer::Evaluate(g, "$ScreenPlane", std::move(indices)));
	const auto& vs = VertexShader::Evaluate(g, "res/shaders/Screenvs.hlsl");
	AddBind(InputLayout::Evaluate(g, planeLayout, *vs));
	AddBind(std::move(vs));
	AddBind(Rasterizer::Evaluate(g));
	AddBind(Topology::Evaluate(g));
}

void ScreenPass::Run(Graphics& g)
{
	BindAll(g);
	g.DC().DrawIndexed(6, 0, 0);
}
