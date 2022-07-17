#pragma once

#include "Renderable.h"
#include "ConstantBuffer.h"

class Skybox : public Renderable {
public:
	Skybox(Graphics& g, const std::string& texDir);
	virtual XMMATRIX ModelMat() const override;

private:
	class SkyboxCbuff : public VertexConstantBuffer<XMMATRIX> {
	public:
		SkyboxCbuff(Graphics& g);
		virtual void Bind(Graphics& g) override;

	private:
		XMMATRIX m_VP;
	};
};