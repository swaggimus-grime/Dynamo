#include "dynamopch.h"
#include "Light.h"

#include <imgui.h>
#include "Camera.h"
#include "Shapes.h"
#include "Bindable/Shader.h"
#include "Bindable/InputLayout.h"

PointLight::PointLight(Graphics& g, const XMFLOAT3& color)
	:m_Color(color)
{
	m_Light.Pos = m_Pos;
	m_Light.Color =  color;
	m_Light.Ambient = { 0.5f,0.5f,0.5f };
	m_Light.Intensity = 3.f;
	m_Light.QuadAtt = 0.0030f;
	m_Light.LinAtt = 0.025f;
	m_Light.ConstAtt = 1.f;

	m_LightData = MakeUnique<PixelConstantBuffer<Light>>(g, 1);

	auto& shape = Cube::Make();
	m_VBuff = MakeUnique<VertexBuffer>(g, shape.Vertices);
	m_IBuff = MakeUnique<IndexBuffer>(g, shape.Indices);
	m_Top = Topology::Evaluate(g);

	{
		Technique lambertian("Shade");
		{
			Step only("lambertian");
			auto& vs = VertexShader::Evaluate(g, "res/shaders/Solidvs.cso");
			only.AddBind(InputLayout::Evaluate(g, shape.Vertices.Layout(), *vs));
			only.AddBind(vs);
			only.AddBind(MakeShared<LightColorBuff>(g, *this));
			only.AddBind(PixelShader::Evaluate(g, "res/shaders/Solidps.cso"));
			only.AddBind(MakeShared<TransformBuffer>(g));
			lambertian.AddStep(only);
		}

		AddTechnique(lambertian);
	}
}

void PointLight::Bind(Graphics& g)
{
	m_LightData->Update(g, &m_Light);
	m_LightData->Bind(g);
}

void PointLight::SetPos(const XMFLOAT3& pos)
{
	Transformable::SetPos(pos);
	m_Light.Pos = pos;
}

//void PointLight::ShowGUI(Graphics& g)
//{
//	if(ImGui::ColorEdit3("Diffuse Color", &m_Light.Color.x))
//		m_Buff.Color = m_Light.Color;
//	ImGui::SliderFloat("Intensity", &m_Light.Intensity, 0.f, 10.0f, "%.2f");
//	ImGui::ColorEdit3("Ambient", &m_Light.Ambient.x);
//
//	ImGui::Text("Attenuation");
//	ImGui::SliderFloat("Constant", &m_Light.ConstAtt, 0.05f, 10.0f, "%.2f");
//	ImGui::SliderFloat("Linear", &m_Light.LinAtt, 0.0001f, 4.0f, "%.4f");
//	ImGui::SliderFloat("Quadratic", &m_Light.QuadAtt, 0.00001f, 10.0f, "%.7f");
//	Transformable::ShowGUI(g);
//}
//
//PointLight::PointLightBuffer::PointLightBuffer(Graphics& g)
//	:ConstantBuffer(g, SHADER_TYPE::VS, sizeof(m_Buff))
//{
//}

PointLight::LightColorBuff::LightColorBuff(Graphics& g, PointLight& light)
	:m_Light(light), PixelConstantBuffer(g)
{
}

void PointLight::LightColorBuff::Bind(Graphics& g)
{
	auto c = m_Light.Color();
	auto color = { c.x, c.y, c.z, 1.f };
	Update(g, &c);
	__super::Bind(g);
}

//LightContainer::LightContainer(Graphics& g, UINT slot)
//	:PixelConstantBuffer(g, slot)
//{
//}
//
//LightContainer::~LightContainer()
//{
//	delete LightArray;
//}
//
//UINT LightContainer::Submit(const Light& light)
//{
//	static UINT idx = 0;
//	DYNAMO_ASSERT(idx < MAX_LIGHTS, "Too many lights");
//	LightArray->m_Lights[idx] = light;
//	return idx++;
//}
//
//void LightContainer::UpdateLight(UINT idx, const Light& light)
//{
//	DYNAMO_ASSERT(idx < MAX_LIGHTS, "Invalid index");
//	LightArray->m_Lights[idx] = light;
//}
//
//void LightContainer::Bind(Graphics& g)
//{
//	Update(g, &LightArray->m_Lights);
//	__super::Bind(g);
//}
//
//Shared<LightContainer> LightContainer::Evaluate(Graphics& g, UINT slot)
//{
//	return Binds::Evaluate<LightContainer>(g, slot);
//}
