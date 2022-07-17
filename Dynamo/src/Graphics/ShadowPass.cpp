//#include "dynamopch.h"
//#include "ShadowPass.h"
//#include "RenderTarget.h"
//#include "Light.h"
//#include "Entity.h"
//
//ShadowPass::ShadowPass(Graphics& g, std::shared_ptr<PointLight> light)
//	:m_Light(light)
//{
//	m_DepthTarget = std::make_shared<RenderTarget>(g, g.Width(), g.Height());
//	g.SubmitRenderTarget(m_DepthTarget);
//	m_Depth = std::make_unique<ReadableDepthStencilView>(g, g.Width(), g.Height());
//	m_Shader = std::make_shared<Shader>(g, L"res\\shaders\\ShadowMapvs.cso");
//
//	
//	m_Surface = std::make_unique<Plane>(g, std::make_shared<Shader>(g, L"res\\shaders\\DepthMapvs.cso", L"res\\shaders\\DepthMapps.cso"), XMFLOAT3(0.f, 0.f, 0.f));
//	m_TransBuff = std::make_unique<ConstantBuffer>(g, SHADER_TYPE::VS, sizeof(m_MLP));
//}
//
//void ShadowPass::Run(Graphics& g)
//{
//	m_DepthTarget->Bind(g, *m_Depth);
//	m_DepthTarget->Clear(g);
//	m_Depth->Clear(g);
//
//	m_Shader->Bind(g);
//	const auto& lp = m_Light->GetCamera().LookAt() * g.Projection();
//	m_TransBuff->Bind(g);
//	for (const auto& r : g.GetScene()) {
//		m_MLP = XMMatrixTranspose(r.second->GetModelMat() * lp);
//		m_TransBuff->Update(g, sizeof(m_MLP), &m_MLP);
//		r.second->RenderPos(g);
//	}
//	
//	g.BindBackBuffer();
//	m_Depth->BindTexture(g);
//	m_Surface->Render(g);
//}
