#include "dynamopch.h"
#include "Light.h"

PointLight::PointLight(Graphics& g, const XMFLOAT3& pos, const XMFLOAT3& color)
	:ConstantBuffer(g, SHADER_TYPE::PS, sizeof(m_Light), 1)
{
	m_Pos = pos;
	m_Color = color;

	m_Buff.Color = XMVectorSet(color.x, color.y, color.z, 1.f);
	m_Light.Color = m_Buff.Color;
	m_Shader = std::make_shared<Shader>(g, L"res/shaders/PointLightvs.cso", L"res/shaders/PointLightps.cso");
	m_Cube = std::make_unique<Cube>(g, m_Shader, pos);
	m_CBuff = std::make_unique<PointLightBuffer>(g);
}

void PointLight::Translate(float x, float y, float z)
{
	m_Pos.x += x;
	m_Pos.y += y;
	m_Pos.z += z;
	Transformable::Translate(x, y, z);
}

void PointLight::Bind(Graphics& g)
{
	m_Light.Pos = XMVector4Transform(XMLoadFloat3(&m_Pos), GetModelMat() * g.LookAt());
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
}

PointLight::PointLightBuffer::PointLightBuffer(Graphics& g)
	:ConstantBuffer(g, SHADER_TYPE::VS, sizeof(m_Buff))
{
}
