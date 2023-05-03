#include "dynamopch.h"
#include "Work.h"

#include "Graphics.h"
#include "Step.h"
#include "Renderable.h"

Work::Work(Step& step, Renderable& renderable)
	:m_Step(step), m_Renderable(renderable)
{
}

void Work::Run(Graphics& g) const
{
	m_Step.Bind(g);
	m_Renderable.Bind(g);
	g.DC().DrawIndexed(m_Renderable.IndexCount(), 0, 0);
}