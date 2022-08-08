#include "dynamopch.h"
#include "BasicRenderPass.h"

#include "In.h"
#include "Out.h"
#include "Bindable/DSState.h"

BasicRenderPass::BasicRenderPass(Graphics& g, const std::string& name)
	:RenderPass(std::move(name))
{
	AddIn(BufferIn<RenderTarget>::Make("renderTarget", m_RT));
	AddIn(BufferIn<DepthStencil>::Make("depthStencil", m_DS));
	AddOut(BufferOut<RenderTarget>::Make("renderTarget", m_RT));
	AddOut(BufferOut<DepthStencil>::Make("depthStencil", m_DS));
	AddBind(DSState::Evaluate(g));
}

void BasicRenderPass::Run(Graphics& g)
{
	g.SetCamera(*m_Camera);
	RenderPass::Run(g);
}
