#pragma once

#include "ConstantBuffer.h"

struct Transform {
	XMMATRIX Model;
	XMMATRIX ModelView;
	XMMATRIX MVP;
};

class TransformBuffer : public VertexConstantBuffer<Transform> {
public:
	TransformBuffer(Graphics& g, class Renderable& r, UINT slot = 0);
	virtual void Bind(Graphics& g) override;

private:
	Transform m_Mats;
	Renderable& m_Renderable;
};