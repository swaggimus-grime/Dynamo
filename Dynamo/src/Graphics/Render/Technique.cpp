#include "dynamopch.h"
#include "Technique.h"

#include "Graphics.h"
#include "Renderable.h"
#include "RDG/RDG.h"

Technique::Technique(const std::string& name)
	:m_Name(name)
{
}

void Technique::Submit(Renderable& r)
{
	for (auto& s : m_Steps)
		s.Submit(r);
}

void Technique::LinkToRDG(RDG& graph)
{
	for (auto& s : m_Steps)
		s.LinkToRDG(graph);
}

void Technique::AddStep(const Step& step)
{
	m_Steps.push_back(std::move(step));
}

void Technique::SetParent(const Renderable& parent)
{
	for (auto& s : m_Steps)
		s.SetParent(parent);
}
