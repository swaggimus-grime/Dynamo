#pragma once

#include "RenderPass.h"

class Camera;

class BasicRenderPass : public RenderPass {
public:
	BasicRenderPass(Graphics& g, const std::string& name);
	virtual void Run(Graphics& g) override;
	inline void SetCamera(const Camera& c) { m_Camera = &c; }

private:
	const Camera* m_Camera = nullptr;
};