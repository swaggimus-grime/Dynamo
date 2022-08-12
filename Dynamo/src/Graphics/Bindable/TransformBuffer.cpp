#include "dynamopch.h"
#include "TransformBuffer.h"

#include "Graphics.h"
#include "Render/Renderable.h"

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
