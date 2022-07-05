#pragma once

#include "RenderPass.h"
#include "RenderTarget.h"

class NegativePass : public RenderPass {
public:
	NegativePass(Graphics& g);
	virtual void Run(Graphics& g) override;

private:
	std::shared_ptr<class Shader> m_Shader;
	std::unique_ptr<class Plane> m_Surface;
	std::shared_ptr<ReadWriteRenderTarget> m_Target;
	std::unique_ptr<DSView> m_DS;
};