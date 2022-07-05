#include "dynamopch.h"
#include "Light.h"
#include <imgui.h>
#include "Camera.h"

PointLight::PointLight(Graphics& g, const XMFLOAT3& pos, const XMFLOAT3& color)
	:Transformable({ pos }),
	ConstantBuffer(g, SHADER_TYPE::PS, sizeof(m_Light), 1)
{
	m_Pos = m_PrevPos = pos;
	m_Buff.Color = color;
	m_Light.Color = m_Buff.Color;
	m_Light.Ambient = { 0.5f,0.5f,0.5f };
	m_Light.Intensity = 3.f;
	m_Light.QuadAtt = 0.0030f;
	m_Light.LinAtt = 0.025f;
	m_Light.ConstAtt = 1.f;

	m_Shader = std::make_shared<Shader>(g, L"res/shaders/PointLightvs.cso", L"res/shaders/PointLightps.cso");
	m_Shape = std::make_unique<Sphere>(g, m_Shader, pos, 5.f);
	m_CBuff = std::make_unique<PointLightBuffer>(g);
	m_Camera = std::make_unique<Camera>(m_Pos);
}

void PointLight::Translate(float x, float y, float z)
{
	m_Pos.x += x - m_PrevPos.x;
	m_Pos.y += y - m_PrevPos.y;
	m_Pos.z += z - m_PrevPos.z;
	m_Camera->Move({ x - m_PrevPos.x, y - m_PrevPos.y, z - m_PrevPos.z });
	m_PrevPos = m_Pos;
	Transformable::Translate(x, y, z);
}

void PointLight::Bind(Graphics& g)
{
	XMStoreFloat3(&m_Light.Pos, XMVector3Transform(XMLoadFloat3(&m_Pos), g.LookAt()));
	Update(g, sizeof(m_Light), &m_Light);
	ConstantBuffer::Bind(g);
}

void PointLight::Render(Graphics& g) 
{
	m_Buff.MVP = XMMatrixTranspose(GetModelMat() * g.LookAt() * g.Projection());
	m_CBuff->Update(g, sizeof(m_Buff), &m_Buff);
	m_CBuff->Bind(g);
	m_Shader->Bind(g);
	m_Shape->Render(g);
}

void PointLight::ShowGUI(Graphics& g)
{
	if(ImGui::ColorEdit3("Diffuse Color", &m_Light.Color.x))
		m_Buff.Color = m_Light.Color;
	ImGui::SliderFloat("Intensity", &m_Light.Intensity, 0.f, 10.0f, "%.2f");
	ImGui::ColorEdit3("Ambient", &m_Light.Ambient.x);

	ImGui::Text("Attenuation");
	ImGui::SliderFloat("Constant", &m_Light.ConstAtt, 0.05f, 10.0f, "%.2f");
	ImGui::SliderFloat("Linear", &m_Light.LinAtt, 0.0001f, 4.0f, "%.4f");
	ImGui::SliderFloat("Quadratic", &m_Light.QuadAtt, 0.00001f, 10.0f, "%.7f");
	Transformable::ShowGUI(g);
}

PointLight::PointLightBuffer::PointLightBuffer(Graphics& g)
	:ConstantBuffer(g, SHADER_TYPE::VS, sizeof(m_Buff))
{
}
