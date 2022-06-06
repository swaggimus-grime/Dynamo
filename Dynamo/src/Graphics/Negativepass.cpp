#include "dynamopch.h"
#include "Negativepass.h"
#include "RenderTarget.h"
#include "Scene.h"
#include "Shader.h"
#include "Shapes.h"

Negativepass::Negativepass(Graphics& g)
{
	m_Shader = std::make_shared<Shader>(g, L"res\\shaders\\Negativevs.cso", L"res\\shaders\\Negativeps.cso");
	m_Surface = std::make_unique<Plane>(g, m_Shader, XMFLOAT3(0.f, 0.f, 0.f));
	m_Target = std::make_shared<RenderTarget>(g, g.Width(), g.Height());
	g.SubmitRenderTarget(m_Target);
	m_DS = std::make_unique<DSView>(g, g.Width(), g.Height());
}

void Negativepass::Run(Graphics& g)
{
	m_Target->Bind(g, *m_DS);
	m_Target->Clear(g);
	m_DS->Clear(g);
	g.GetScene().Render(g);
	g.BindBackBuffer();
	m_Target->Tex().Bind(g);
	m_Surface->Render(g);
}
