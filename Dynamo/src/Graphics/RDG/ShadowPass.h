//#pragma once
//
//#include "RenderPass.h"
//
//class ShadowPass : public RenderPass {
//public:
//	ShadowPass(Graphics& g, std::shared_ptr<class PointLight> light);
//	virtual void Run(Graphics& g) override;
//
//private:
//	std::shared_ptr<class PointLight> m_Light;
//	std::unique_ptr<class ReadableDepthStencilView> m_Depth;
//	std::shared_ptr<class RenderTarget> m_DepthTarget;
//	std::unique_ptr<class Plane> m_Surface;
//	std::shared_ptr<class Shader> m_Shader;
//
//	XMMATRIX m_MLP;
//	std::unique_ptr<class ConstantBuffer> m_TransBuff;
//};