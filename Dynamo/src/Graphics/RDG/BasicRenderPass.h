#pragma once

#include "RenderPass.h"
#include "Bindable/TransformBuffer.h"

class Camera;
class Sampler;
class ShadowSampler;

class BasicRenderPass : public RenderPass {
public:
	BasicRenderPass(Graphics& g, const std::string& name);
	virtual void Run(Graphics& g) override;
	inline void SetCamera(const Camera& c) { m_Camera = &c; }
	inline void SetShadowCamera(const Camera& c) { m_ShadowTransform->SetCamera(&c); }
	virtual void OnNodeSelect(Graphics& g) override;

private:
	const Camera* m_Camera = nullptr;
	Shared<ShadowCamBuffer> m_ShadowTransform;

	Shared<Sampler> m_Sampler;
	Shared<ShadowSampler> m_ShadowSampler;
	static const std::string s_Options[3];
	std::string m_Option;
};