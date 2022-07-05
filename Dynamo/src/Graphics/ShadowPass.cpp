#include "dynamopch.h"
#include "ShadowPass.h"
#include "RenderTarget.h"
#include "Light.h"

ShadowPass::ShadowPass(Graphics& g, std::shared_ptr<PointLight> light)
	:m_Light(light)
{

	m_Depth = std::make_unique<DSView>(g, g.Width(), g.Height());
	//m_Surface = std::make_unique<Plane>(g, m_Shader, XMFLOAT3(0.f, 0.f, 0.f));
}

void ShadowPass::Run(Graphics& g)
{
	auto& backbuff = g.BackBuffer();
	backbuff.Bind(g, *m_Depth);
	backbuff.Clear(g);
	m_Depth->Clear(g);
	g.SetCamera(m_Light->GetCamera());
	g.GetScene().Render(g);
}
