#include "dynamopch.h"
#include "RenderPass.h"

void RenderPass::AddWork(const Work& work)
{
	m_Works.push_back(std::move(work));
}

void RenderPass::Run(Graphics& g) const
{
	for (const auto& w : m_Works)
		w.Run(g);
}