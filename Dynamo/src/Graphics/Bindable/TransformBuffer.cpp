#include "dynamopch.h"
#include "TransformBuffer.h"

#include "Graphics.h"
#include "Render/Renderable.h"
#include "imgui.h"
#include "Animation/AnimModel.h"

TransformBuffer::TransformBuffer(Graphics& g, UINT slot)
	:VertexConstantBuffer<Transform>(g, slot)
{
}

void TransformBuffer::Bind(Graphics& g)
{
	const auto modelView = m_Renderable->ModelMat() * g.LookAt();
	m_Mats.Model = XMMatrixTranspose(m_Renderable->ModelMat());
	m_Mats.ModelView = XMMatrixTranspose(modelView);
	m_Mats.MVP = XMMatrixTranspose(modelView * g.Projection());

	Update(g, &m_Mats);
	__super::Bind(g);
}

Transformable::Transformable()
{
}

void Transformable::SetPos(const XMFLOAT3& pos)
{
	m_Pos = pos;
	m_Trans = XMMatrixTranslation(pos.x, pos.y, pos.z);
}

void Transformable::SetRot(const XMFLOAT3& rot)
{
	m_Ori = rot;
	m_Rot = XMMatrixRotationRollPitchYaw(rot.x, rot.y, rot.z);
}

void Transformable::SetScale(const XMFLOAT3& scale)
{
	m_Size = scale;
	m_Scale = XMMatrixScaling(scale.x, scale.y, scale.z);
}

void Transformable::Move(const XMFLOAT3& delta)
{
	m_Pos.x += delta.x;
	m_Pos.y += delta.y;
	m_Pos.z += delta.z;
	m_Trans = XMMatrixTranslation(m_Pos.x, m_Pos.y, m_Pos.z);
}

void Transformable::Rotate(const XMFLOAT3& delta)
{
	m_Ori.x += delta.x;
	m_Ori.y += delta.y;
	m_Ori.z += delta.z;
	m_Rot = XMMatrixRotationRollPitchYaw(m_Ori.x, m_Ori.y, m_Ori.z);
}

void Transformable::Rotate(float dx, float dy)
{
	m_Ori.x += dx;
	m_Ori.y += dy;
	m_Ori.x = std::clamp(m_Ori.x, 0.f, 2.f * static_cast<float>(M_PI));
	m_Ori.y = std::clamp(m_Ori.y, 0.f, 2.f * static_cast<float>(M_PI));
	m_Rot = XMMatrixRotationRollPitchYaw(m_Ori.x, m_Ori.y, m_Ori.z);
}

void Transformable::ShowGUI(Graphics& g)
{
	ImGui::Text("Transform");
	if (ImGui::SliderFloat3("Position", &m_Pos.x, -100.f, 100.f))
		SetPos(m_Pos);
	else if (ImGui::SliderFloat3("Rotation", &m_Ori.x, 0.f, 2 * M_PI))
		SetRot(m_Ori);
	else if (ImGui::SliderFloat3("Scale", &m_Size.x, -10.f, 10.f))
		SetScale(m_Size);
}

ShadowCamBuffer::ShadowCamBuffer(Graphics& g, UINT slot)
	:VertexConstantBuffer<ShadowCamTransform>(g, slot), m_Camera(nullptr)
{
}

void ShadowCamBuffer::Bind(Graphics& g)
{
	auto pos = m_Camera->GetPos();
	ShadowCamTransform t{
		XMMatrixTranspose(XMMatrixTranslation(-pos.x, -pos.y, -pos.z))
	};
	
	Update(g, &t);
	__super::Bind(g);
}

void ShadowCamBuffer::SetCamera(const Camera* camera)
{
	m_Camera = camera;
}

BoneBuffer::BoneBuffer(Graphics& g, UINT slot)
	:VertexConstantBuffer<BoneTransform>(g, slot)
{
}

void BoneBuffer::Bind(Graphics& g)
{
	const XMMATRIX* boneMats = m_Model->BoneMats();
	BoneTransform t{};
	std::memcpy(t.boneMats, boneMats, sizeof(XMMATRIX) * MAX_NUM_BONES);
	Update(g, &t);
	__super::Bind(g);
}
