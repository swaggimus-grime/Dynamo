#include "dynamopch.h"
#include "BindPass.h"

#include "Graphics.h"
#include "Bindable/RenderTarget.h"
#include "Bindable/DepthStencil.h"

BindPass::BindPass(const std::string& name, const std::vector<Shared<Bindable>>& binds)
	:Pass(std::move(name)), m_Binds(std::move(binds))
{
}

void BindPass::AddBind(Shared<Bindable> bind)
{
	m_Binds.push_back(std::move(bind));
}

void BindPass::BindAll(Graphics& g)
{
	BindFramebuffer(g);
	for (auto& b : m_Binds)
		b->Bind(g);
}

void BindPass::Finish()
{
	Pass::Finish();
	if (!m_RT && !m_DS)
	{
		throw DYNAMO_EXCEP("BindPass [" + Name() + "] needs at least one of a renderTarget or depthStencil");
	}
}

void BindPass::BindFramebuffer(Graphics& g)
{
	if (m_RT)
		m_RT->BindBuffer(g, m_DS.get());
	else if(m_DS)
		m_DS->BindBuffer(g);
}
