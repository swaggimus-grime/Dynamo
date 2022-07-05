#pragma once

#include "RenderPass.h"

class ShadowPass : public RenderPass {
public:
	ShadowPass(Graphics& g, std::shared_ptr<class PointLight> light);
	virtual void Run(Graphics& g) override;

private:
	std::shared_ptr<class PointLight> m_Light;
	std::unique_ptr<class DSView> m_Depth;
	std::unique_ptr<class Plane> m_Surface;
};