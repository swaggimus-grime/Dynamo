#pragma once

#include "BindPass.h"
#include "Bindable/ConstantBuffer.h"

class SkyboxPass : public BindPass {
public:
	SkyboxPass(class Graphics& g, const std::string& name);
	inline void SetCamera(class Camera& cam) { m_Camera = &cam; }
	void Run(Graphics& g) override;

private:
	class SkyboxCBuff : public VertexConstantBuffer<XMMATRIX> {
	public:
		SkyboxCBuff(Graphics& g);
		virtual void Bind(Graphics& g) override;
	};

private:
	Camera* m_Camera = nullptr;
	Shared<class VertexBuffer> m_VBuff;
	Shared<class IndexBuffer> m_IBuff;
	UINT m_CubeCount;
};