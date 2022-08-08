#pragma once

#include "Render/Renderable.h"
#include "Bindable/TransformBuffer.h"
#include "Bindable/ConstantBuffer.h"

//#define MAX_LIGHTS 2

struct Light {
	alignas(16) XMFLOAT3 Pos;
	alignas(16) XMFLOAT3 Color;
	alignas(16) XMFLOAT3 Ambient;
	float Intensity;
	float QuadAtt;
	float LinAtt;
	float ConstAtt;

};

//static struct Lights {
//	Light m_Lights[MAX_LIGHTS];
//}*LightArray = new Lights[MAX_LIGHTS];
//
//class LightContainer : public PixelConstantBuffer<Lights> {
//public:
//	LightContainer(Graphics& g, UINT slot = 0);
//	~LightContainer();
//	UINT Submit(const Light& light);
//	void UpdateLight(UINT idx, const Light& light);
//	virtual void Bind(Graphics& g) override;
//	static Shared<LightContainer> Evaluate(Graphics& g, UINT slot = 0);
//};

class PointLight : public Renderable, public Transformable {
public:
	PointLight(Graphics& g, const XMFLOAT3& color = {1.f, 1.f, 1.f});
	inline XMFLOAT3 Color() const { return m_Color; }
	virtual XMMATRIX ModelMat() const { return TransformMat(); }

private:
	XMFLOAT3 m_Color;

private:
	class LightColorBuff : public PixelConstantBuffer<XMFLOAT4> {
	public:
		LightColorBuff(Graphics& g, PointLight& light);
		virtual void Bind(Graphics& g) override;

	private:
		PointLight& m_Light;
	};

private:

};