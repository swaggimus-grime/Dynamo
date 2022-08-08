#include "dynamopch.h"
#include "Renderable.h"

#include "Graphics.h"
#include "Bindable/VertexBuffer.h"
#include "RDG/RDG.h"
#include "Render/Technique.h"

void Renderable::Submit()
{
	for (auto& t : m_Techs)
		t.Submit(*this);
}

void Renderable::LinkToRDG(RDG& graph)
{
	for (auto& t : m_Techs)
		t.LinkToRDG(graph);
}

void Renderable::Bind(Graphics& g) const
{
	m_Top->Bind(g);
	m_VBuff->Bind(g);
	m_IBuff->Bind(g);
}

void Renderable::AddTechnique(Technique tech)
{
	tech.SetParent(*this);
	m_Techs.push_back(std::move(tech));
}
