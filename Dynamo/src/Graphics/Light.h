#pragma once

#include "Transformable.h"
#include "Renderable.h"
#include "Shapes.h"
#include "Buffer.h"
#include "GUI/GUIable.h"

class PointLight : public Transformable, public Renderable, public ConstantBuffer, public GUIable {
public:
	PointLight(Graphics& g, const XMFLOAT3& pos, const XMFLOAT3& color = {1.f, 1.f, 1.f});
	virtual void Translate(float x, float y, float z) override;
	virtual void Render(Graphics& g) override;
	virtual void ShowGUI() override;
	virtual void Bind(Graphics& g) override;
private:
	struct {
		alignas(16) XMFLOAT3 Pos;
		alignas(16) XMFLOAT3 Color;
		alignas(16) XMFLOAT3 Ambient;
		float Intensity;
		float QuadAtt;
		float LinAtt;
		float ConstAtt;
	} m_Light;

private:
	struct {
		XMMATRIX MVP;
		alignas(16) XMFLOAT3 Color;

	} m_Buff;

	class PointLightBuffer : public ConstantBuffer {
	public:
		PointLightBuffer(Graphics& g);
	};

private:
	std::unique_ptr<Cube> m_Cube;
	std::unique_ptr<PointLightBuffer> m_CBuff;
	std::shared_ptr<Shader> m_Shader;

	XMFLOAT3 m_PrevPos;
	XMFLOAT3 m_Pos;
};