#include "dynamopch.h"
#include "TransformBuffer.h"

#include "Graphics.h"
#include "Renderable.h"

TransformBuffer::TransformBuffer(Graphics& g, Renderable& r, UINT slot)
	:VertexConstantBuffer<Transform>(g, slot), m_Renderable(r)
{
}

void TransformBuffer::Bind(Graphics& g)
{
	m_Mats.Model = XMMatrixTranspose(m_Renderable.ModelMat());
	m_Mats.ModelView = XMMatrixTranspose(g.LookAt()) * m_Mats.Model;
	m_Mats.MVP = XMMatrixTranspose(g.Projection()) * m_Mats.ModelView;

	Update(g, &m_Mats);
	__super::Bind(g);
}