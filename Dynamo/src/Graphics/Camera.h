#pragma once
#include <DirectXMath.h>

class Camera
{
public:
	Camera(XMFLOAT3 pos = { 0.0f,0.0f,0.0f });
	XMMATRIX LookAt();
	inline constexpr XMMATRIX Projection() const { return m_Proj; }
	void Rotate(float dx, float dy) noexcept;
	void Move(XMFLOAT3 translation);
	DirectX::XMFLOAT3 GetPos() const noexcept;

private:
	XMMATRIX m_Proj;
	float m_Pitch;
	float m_Yaw;
	static constexpr float m_MoveSpeed = 20.0f;
	static constexpr float m_LookSpeed = 0.004f;

	XMFLOAT3 m_Pos;
};