#include "Renderable.h"
#include "Renderable.h"
#include "Renderable.h"
#include "dynamopch.h"
#include "Renderable.h"

#include "Graphics.h"
#include "VertexBuffer.h"

void Renderable::Submit(Graphics& g)
{
	for (auto& t : m_Techs)
		t.Submit(g, *this);
}

void Renderable::Bind(Graphics& g) const
{
	g.DC().IASetPrimitiveTopology(m_Top);
	m_VBuff->Bind(g);
	m_IBuff->Bind(g);
}

void Renderable::AddTechnique(const Technique& tech)
{
	m_Techs.push_back(std::move(tech));
}
