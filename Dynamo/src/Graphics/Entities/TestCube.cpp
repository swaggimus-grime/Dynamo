#include "TestCube.h"
#include "dynamopch.h"
#include "TestCube.h"

#include "Render/Step.h"
#include "Bindable/Texture.h"
#include "Shapes.h"
#include "Bindable/Sampler.h"
#include "Render/Technique.h"
#include "Bindable/Shader.h"
#include "Bindable/InputLayout.h"
#include "Bindable/Topology.h"

TestCube::TestCube(Graphics& g, float size)
{
	auto& shape = Cube::Make();
	shape.Transform(XMMatrixScaling(size, size, size));
	//shape.SetNormalsIndependentFlat();
	const auto geometryTag = "$cube." + std::to_string(size);
	m_VBuff = VertexBuffer::Evaluate(g, geometryTag, shape.Vertices);
	m_IBuff = IndexBuffer::Evaluate(g, geometryTag, shape.Indices);
	m_Top = Topology::Evaluate(g);

	auto tcb = std::make_shared<TransformBuffer>(g);
	tcb->SetParent(*this);

	{
		Technique shade("Shade");
		{
			Step only("lambertian");
			//only.AddBind(Texture2D::Evaluate(g, "res\\skyboxes\\ocean\\0.jpg", 0));
			//only.AddBind(Sampler::Evaluate(g));
			auto& vs = VertexShader::Evaluate(g, "res\\shaders\\Solidvs.cso");
			only.AddBind(InputLayout::Evaluate(g, shape.Vertices.Layout(), *vs));
			only.AddBind(std::move(vs));
			only.AddBind(PixelShader::Evaluate(g, "res\\shaders\\Solidps.cso"));
			only.AddBind(tcb);
			only.AddBind(Rasterizer::Evaluate(g, RS_MODE::CULL_NONE));
			shade.AddStep(std::move(only));
		}

		AddTechnique(std::move(shade));
	}
	{
		Technique outline("Outline");
		{
			Step mask("outlineMask");

			// TODO: better sub-layout generation tech for future consideration maybe
			mask.AddBind(InputLayout::Evaluate(g, shape.Vertices.Layout(), *VertexShader::Evaluate(g, "res/shaders/Solidvs.cso")));

			mask.AddBind(std::move(tcb));

			// TODO: might need to specify rasterizer when doubled-sided models start being used

			outline.AddStep(std::move(mask));
		}
		{
			Step draw("outlineDraw");

			// TODO: better sub-layout generation tech for future consideration maybe
			draw.AddBind(InputLayout::Evaluate(g, shape.Vertices.Layout(), *VertexShader::Evaluate(g, "res/shaders/Solidvs.cso")));

			draw.AddBind(std::make_shared<TransformBuffer>(g));

			// TODO: might need to specify rasterizer when doubled-sided models start being used

			outline.AddStep(std::move(draw));
		}
		AddTechnique(std::move(outline));
	}

}

XMMATRIX TestCube::ModelMat() const
{
	return TransformMat();
}

void TestCube::ShowGUI()
{
	Transformable::ShowGUI();
}

