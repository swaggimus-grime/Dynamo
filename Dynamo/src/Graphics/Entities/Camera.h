#pragma once

#include <DirectXMath.h>

class Camera {
public:
	Camera(const std::string& name, XMFLOAT3 pos);
	XMMATRIX LookAt() const;
	inline constexpr XMMATRIX Projection() const { return m_Proj; }
	void Rotate(float dx, float dy) noexcept;
	inline void SetRot(float yaw, float pitch) { m_Pitch = pitch; m_Yaw = yaw; }
	void Move(XMFLOAT3 translation);
	inline void SetPos(XMFLOAT3 newPos) { m_Pos = newPos; }
	DirectX::XMFLOAT3 GetPos() const noexcept;

private:
	std::string m_Name;
	XMMATRIX m_Proj;
	float m_Pitch;
	float m_Yaw;
	float m_MoveSpeed = 6.0f;
	float m_LookSpeed = 0.004f;
	XMFLOAT3 m_Pos = { 0, 0, 0 };
};