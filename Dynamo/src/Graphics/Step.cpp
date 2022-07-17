#include "dynamopch.h"
#include "Step.h"

#include "Graphics.h"
#include "Renderable.h"
#include "Bindable.h"
#include "Work.h"

Step::Step(UINT passIdx)
	:m_PassIdx(passIdx)
{
}

void Step::Submit(Graphics& g, Renderable& r)
{
	g.Assign(m_PassIdx, Work{ *this, r });
}

void Step::Bind(Graphics& g) const
{
	for (const auto& b : m_Binds)
		b->Bind(g);
}

void Step::AddBind(Shared<Bindable> bind)
{
	m_Binds.push_back(std::move(bind));
}
