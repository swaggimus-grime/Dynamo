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
			auto& vs = VertexShader::Evaluate(g, "res/shaders/PointLightvs.cso");
			only.AddBind(InputLayout::Evaluate(g, shape.Vertices.Layout(), *vs));
			only.AddBind(vs);
			only.AddBind(MakeShared<LightBuff>(g, *this));
			only.AddBind(PixelShader::Evaluate(g, "res/shaders/PointLightps.cso"));
			lambertian.AddStep(only);
		}

		AddTechnique(lambertian);
	}
}

void PointLight::Bind(Graphics& g)
{
	XMStoreFloat3(&m_Light.Pos, XMVector3Transform(XMLoadFloat3(&m_Pos), g.LookAt()));
	m_LightData->Update(g, &m_Light);
	m_LightData->Bind(g);
}

void PointLight::SetPos(const XMFLOAT3& pos)
{
	Transformable::SetPos(pos);
	m_Light.Pos = pos;
}

void PointLight::ShowGUI()
{
	ImGui::ColorEdit3("Diffuse Color", &m_Light.Color.x);
	ImGui::SliderFloat("Intensity", &m_Light.Intensity, 0.f, 10.0f, "%.2f");
	ImGui::ColorEdit3("Ambient", &m_Light.Ambient.x);

	ImGui::Text("Attenuation");
	ImGui::SliderFloat("Constant", &m_Light.ConstAtt, 0.05f, 10.0f, "%.2f");
	ImGui::SliderFloat("Linear", &m_Light.LinAtt, 0.0001f, 4.0f, "%.4f");
	ImGui::SliderFloat("Quadratic", &m_Light.QuadAtt, 0.00001f, 10.0f, "%.7f");

	Transformable::ShowGUI();
}

PointLight::LightBuff::LightBuff(Graphics& g, PointLight& light)
	:m_Light(light), VertexConstantBuffer(g)
{
}

void PointLight::LightBuff::Bind(Graphics& g)
{
	LightDraw l = { XMMatrixTranspose(m_Light.ModelMat() * g.LookAt() * g.Projection()), m_Light.Color() };
	Update(g, &l);
	__super::Bind(g);
}
