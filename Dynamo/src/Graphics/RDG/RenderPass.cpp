#include "dynamopch.h"
#include "RenderPass.h"

void RenderPass::AddWork(const Work& work)
{
	m_Works.push_back(work);
}

void RenderPass::Run(Graphics& g)
{
	BindPass::BindAll(g);

	for (const auto& w : m_Works)
		w.Run(g);
}

void RenderPass::Clear()
{
	m_Works.clear();
}