#pragma once

#include "Renderable.h"

class TestCube : public Renderable
{
public:
	TestCube(Graphics& g, float size);
	void SetPos(DirectX::XMFLOAT3 pos) noexcept;
	void SetRotation(float roll, float pitch, float yaw) noexcept;
	virtual XMMATRIX ModelMat() const override;
private:
	DirectX::XMFLOAT3 pos = { 1.0f,1.0f,1.0f };
	float roll = 0.0f;
	float pitch = 0.0f;
	float yaw = 0.0f;
};