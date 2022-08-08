//#include "dynamopch.h"
//#include "NegativePass.h"
//#include "RenderTarget.h"
//#include "Scene.h"
//#include "Shader.h"
//#include "Shapes.h"
//
//NegativePass::NegativePass(Graphics& g)
//{
//	m_Shader = std::make_shared<Shader>(g, L"res\\shaders\\Negativevs.cso", L"res\\shaders\\Negativeps.cso");
//	m_Surface = std::make_unique<Plane>(g, m_Shader, XMFLOAT3(0.f, 0.f, 0.f));
//	m_Target = std::make_shared<ReadableRenderTarget>(g, g.Width(), g.Height());
//	g.SubmitRenderTarget(m_Target);
//	m_DS = std::make_unique<DepthStencilView>(g, g.Width(), g.Height());
//}
//
//void NegativePass::Run(Graphics& g)
//{
//	m_Target->Bind(g, *m_DS);
//	m_Target->Clear(g);
//	m_DS->Clear(g);
//	g.GetScene().Render(g);
//	g.BindBackBuffer();
//	m_Target->BindTexture(g);
//	m_Surface->Render(g);
//}
