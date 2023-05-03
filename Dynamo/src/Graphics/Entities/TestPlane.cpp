//#include "TestPlane.h"
//#include "dynamopch.h"
//#include "TestPlane.h"
//
//#include "Render/Step.h"
//#include "Bindable/Texture.h"
//#include "Shapes.h"
//#include "Bindable/Sampler.h"
//#include "Render/Technique.h"
//#include "Bindable/Shader.h"
//#include "Bindable/InputLayout.h"
//#include "Bindable/Topology.h"
//#include "ObjectCBuffs.h"
//
//TestPlane::TestPlane(Graphics& g, float size)
//	:m_Layout(ATTRIB_POS | ATTRIB_TEX | ATTRIB_NORM | ATTRIB_TAN | ATTRIB_BITAN)
//{
//	auto& shape = Plane::MakeIndependentTextured();
//	shape.Transform(XMMatrixScaling(size, size, 1));
//	const auto geometryTag = "$plane." + std::to_string(size);
//	m_VBuff = VertexBuffer::Evaluate(g, geometryTag, shape.Vertices);
//	m_IBuff = IndexBuffer::Evaluate(g, geometryTag, shape.Indices);
//	m_Top = Topology::Evaluate(g);
//
//	auto tcb = std::make_shared<TransformBuffer>(g);
//	tcb->SetParent(*this);
//
//	{
//		Technique shade("Shade");
//		{
//			Step only("lambertian");
//			only.AddBind(Sampler::Evaluate(g));
//			auto& vs = VertexShader::Evaluate(g, "res\\shaders\\Modelvs.hlsl");
//			only.AddBind(InputLayout::Evaluate(g, shape.Vertices.Layout(), *vs));
//			only.AddBind(std::move(vs));
//			only.AddBind(PixelShader::Evaluate(g, "res\\shaders\\Modelps.hlsl"));
//			only.AddBind(tcb);
//			only.AddBind(Rasterizer::Evaluate(g, RS_MODE::CULL_NONE));
//			only.AddBind(Texture2D::Evaluate(g, "res\\images\\brickwall.jpg", 0));
//			only.AddBind(Texture2D::Evaluate(g, "res\\images\\brickwall_normal.jpg", 2));
//			auto specular = PixelConstantBuffer<Specular>::Evaluate(g);
//			Specular spec;
//			spec.SpecPower = 2.f;
//			spec.SpecColor = { 0.5f, 0.4f, 0.3f, 1.f };
//			spec.hasSpecMap = false;
//			specular->Update(g, &spec);
//			only.AddBind(std::move(specular));
//			shade.AddStep(std::move(only));
//		}
//
//		AddTechnique(std::move(shade));
//	}
//	{
//		Technique outline("Outline");
//		{
//			Step mask("outlineMask");
//
//			// TODO: better sub-layout generation tech for future consideration maybe
//			mask.AddBind(InputLayout::Evaluate(g, shape.Vertices.Layout(), *VertexShader::Evaluate(g, "res/shaders/Solidvs.hlsl")));
//
//			mask.AddBind(std::move(tcb));
//
//			// TODO: might need to specify rasterizer when doubled-sided models start being used
//
//			outline.AddStep(std::move(mask));
//		}
//		{
//			Step draw("outlineDraw");
//
//			// TODO: better sub-layout generation tech for future consideration maybe
//			draw.AddBind(InputLayout::Evaluate(g, shape.Vertices.Layout(), *VertexShader::Evaluate(g, "res/shaders/Solidvs.hlsl")));
//
//			draw.AddBind(std::make_shared<TransformBuffer>(g));
//
//			// TODO: might need to specify rasterizer when doubled-sided models start being used
//
//			outline.AddStep(std::move(draw));
//		}
//		AddTechnique(std::move(outline));
//	}
//
//}
//
//XMMATRIX TestPlane::ModelMat() const
//{
//	return TransformMat();
//}
//
//void TestPlane::ShowGUI()
//{
//	Transformable::ShowGUI();
//}
//
