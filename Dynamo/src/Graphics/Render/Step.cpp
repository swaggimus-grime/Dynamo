#include "dynamopch.h"
#include "Step.h"

#include "Graphics.h"
#include "Renderable.h"
#include "Bindable/Bindable.h"
#include "Work.h"
#include "RDG/RenderPass.h"
#include "RDG/RDG.h"

Step::Step(const std::string& passName)
	:m_PassName(std::move(passName))
{
}

void Step::Submit(Renderable& r)
{
	m_Pass->AddWork(Work{ *this, r });
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

void Step::LinkToRDG(RDG& graph)
{
	DYNAMO_ASSERT(m_Pass == nullptr, "Step was already linked");
	m_Pass = graph.GetRenderPass(m_PassName);
}

void Step::SetParent(const Renderable& parent)
{
	for (auto& b : m_Binds)
		b->SetParent(parent);
}
