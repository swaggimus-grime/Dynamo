#include "dynamopch.h"
#include "Technique.h"

#include "Graphics.h"
#include "Renderable.h"

void Technique::Submit(Graphics& g, Renderable& r)
{
	for (auto& s : m_Steps)
		s.Submit(g, r);
}

void Technique::AddStep(const Step& step)
{
	m_Steps.push_back(std::move(step));
}
