#include "dynamopch.h"
#include "Light.h"
#include <imgui.h>

PointLight::PointLight(Graphics& g, const XMFLOAT3& pos, const XMFLOAT3& color)
	:Transformable({ pos }),
	ConstantBuffer(g, SHADER_TYPE::PS, sizeof(m_Light), 1)
{
	m_Pos = m_PrevPos = pos;
	m_Buff.Color = color;
	m_Light.Color = m_Buff.Color;
	m_Light.Ambient = { 0.1f,0.1f,0.1f };
	m_Light.Intensity = 3.f;
	m_Light.QuadAtt = 0.0030f;
	m_Light.LinAtt = 0.025f;
	m_Light.ConstAtt = 1.f;

	m_Shader = std::make_shared<Shader>(g, L"res/shaders/PointLightvs.cso", L"res/shaders/PointLightps.cso");
	m_Cube = std::make_unique<Cube>(g, m_Shader, pos);
	m_CBuff = std::make_unique<PointLightBuffer>(g);
}

void PointLight::Translate(float x, float y, float z)
{
	m_Pos.x += x - m_PrevPos.x;
	m_Pos.y += y - m_PrevPos.y;
	m_Pos.z += z - m_PrevPos.z;
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
	m_Cube->Render(g);
}

void PointLight::ShowGUI()
{
	if(ImGui::SliderFloat3("Light Color", reinterpret_cast<FLOAT*>(&m_Light.Color), 0.f, 1.f, "%.1f"))
		m_Buff.Color = m_Light.Color;
	ImGui::SliderFloat("Intensity", &m_Light.Intensity, 0.f, 10.f, "%.1f");
	Transformable::ShowGUI();
}

PointLight::PointLightBuffer::PointLightBuffer(Graphics& g)
	:ConstantBuffer(g, SHADER_TYPE::VS, sizeof(m_Buff))
{
}
