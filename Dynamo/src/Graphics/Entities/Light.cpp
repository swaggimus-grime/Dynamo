#include "dynamopch.h"
#include "Light.h"

#include <imgui.h>
#include "Entities/Camera.h"
#include "Entities/Shapes.h"
#include "Bindable/Shader.h"
#include "Bindable/InputLayout.h"
#include "Entities/ObjectCBuffs.h"

PointLight::PointLight(Graphics& g, const XMFLOAT3& pos, const XMFLOAT3& color)
	:m_Color(color)
{
	m_Camera = MakeShared<Camera>("Light", pos);
	SetPos(pos);

    m_Light.Pos = m_Pos;
	m_Light.Color =  color;
	m_Light.Ambient = { 0.1f,0.1f,0.1f };
	m_Light.Intensity = 1.f;
	m_Light.QuadAtt = 0.003f;
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
            auto& vs = VertexShader::Evaluate(g, "res\\shaders\\Solidvs.hlsl");
            only.AddBind(InputLayout::Evaluate(g, shape.Vertices.Layout(), *vs));
            only.AddBind(vs);
            only.AddBind(PixelShader::Evaluate(g, "res\\shaders\\Solidps.hlsl"));
            only.AddBind(MakeShared<TransformBuffer>(g));
            lambertian.AddStep(std::move(only));
        }

        AddTechnique(std::move(lambertian));
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
	m_Camera->SetPos(pos);
}

void PointLight::SetRot(const XMFLOAT3& rot)
{
	Transformable::SetRot(rot);
	m_Camera->SetRot(rot.x, rot.y);
}

void PointLight::Move(const XMFLOAT3& delta)
{
    Transformable::Move(delta);
    m_Camera->Move(delta);
}

void PointLight::Rotate(float dx, float dy)
{
    Transformable::Rotate(dx, dy);
    m_Camera->Rotate(dx, dy);
}

void PointLight::ShowGUI(Graphics& g)
{
	Transformable::ShowGUI(g);

	ImGui::ColorEdit3("Diffuse Color", &m_Light.Color.x);
	ImGui::SliderFloat("Intensity", &m_Light.Intensity, 0.f, 10.0f);
	ImGui::ColorEdit3("Ambient", &m_Light.Ambient.x);

	ImGui::Text("Attenuation");
	ImGui::SliderFloat("Constant", &m_Light.ConstAtt, 0.f, 1.f);
	ImGui::SliderFloat("Linear", &m_Light.LinAtt, 0.f, 1.f);
	ImGui::SliderFloat("Quadratic", &m_Light.QuadAtt, 0.f, 0.05f);
}
