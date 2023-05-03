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

class Camera;

class PointLight : public Renderable, public Transformable {
public:
	PointLight(Graphics& g, const XMFLOAT3& pos, const XMFLOAT3& color = {1.f, 1.f, 1.f});
	inline Shared<Camera> GetCamera() const { return m_Camera; }
	void Bind(Graphics& g);
	virtual void SetPos(const XMFLOAT3& pos) override;
	virtual void SetRot(const XMFLOAT3& rot) override;
	virtual void Move(const XMFLOAT3& delta) override;
	virtual void Rotate(float dx, float dy) override;
	inline XMFLOAT3 Color() const { return m_Color; }
	virtual XMMATRIX ModelMat() const { return TransformMat(); }
	virtual void ShowGUI(Graphics& g) override;

private:
	XMFLOAT3 m_Color;
	Shared<Camera> m_Camera;
	Unique<PixelConstantBuffer<Light>> m_LightData;
	Light m_Light;
};