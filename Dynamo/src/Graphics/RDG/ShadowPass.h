#pragma once

#include "RenderPass.h"
#include "Entities/Camera.h"
#include "Bindable/Sampler.h"

class ShadowPass : public RenderPass 
{
public:
	ShadowPass(class Graphics& g, const std::string& name);
	virtual void Run(Graphics& g) override;
	inline void SetCamera(const Camera& cam) { m_Camera = &cam; }

	inline Shared<DepthStencil> GetDS() const { return m_DS; }
private:
	const Camera* m_Camera;
	Shared<class DepthCubemap> m_DepthCube;

	DirectX::XMFLOAT4X4 m_Proj;
	std::vector<DirectX::XMFLOAT3> m_CamDirs{ 6 };
	std::vector<DirectX::XMFLOAT3> m_CamUps{ 6 };

	static constexpr UINT s_Size = 1280;
};