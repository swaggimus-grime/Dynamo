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
		XMVECTOR Pos;
		XMVECTOR Color;
	} m_Light;

private:
	struct {
		XMMATRIX MVP;
		XMVECTOR Color;

	} m_Buff;

	class PointLightBuffer : public ConstantBuffer {
	public:
		PointLightBuffer(Graphics& g);
	};

private:
	std::unique_ptr<Cube> m_Cube;
	std::unique_ptr<PointLightBuffer> m_CBuff;
	std::shared_ptr<Shader> m_Shader;
	XMFLOAT3 m_Color;
	XMFLOAT3 m_Pos;
};