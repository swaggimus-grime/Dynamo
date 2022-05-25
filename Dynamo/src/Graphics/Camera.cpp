#include "Camera.h"
#include <imgui.h>
#include <algorithm>

Camera::Camera(XMFLOAT3 pos)
	:m_Pos(pos), 
	m_Proj(XMMatrixPerspectiveFovLH(XMConvertToRadians(45.f), 16.f / 9.f, 0.5f, 4000.0f)),
	m_Pitch(0.f), m_Yaw(0)
{
}

XMMATRIX Camera::LookAt() 
{
	const XMVECTOR forwardBaseVector = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	// apply the camera rotations to a base vector
	const auto lookVector = XMVector3Transform(forwardBaseVector,
		XMMatrixRotationRollPitchYaw(m_Pitch, m_Yaw, 0.0f)
	);
	// generate camera transform (applied to all objects to arrange them relative
	// to camera position/orientation in world) from cam position and direction
	// camera "top" always faces towards +Y (cannot do a barrel roll)
	const auto camPos = XMLoadFloat3(&m_Pos);
	const auto camTarget = camPos + lookVector;
	return XMMatrixLookAtLH(camPos, camTarget, XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f));
}

template<typename T>
T wrap_angle(T theta) noexcept
{
	constexpr T twoPi = (T)2 * (T)M_PI;
	const T mod = (T)fmod(theta, twoPi);
	if (mod > (T)M_PI)
	{
		return mod - twoPi;
	}
	else if (mod < -(T)M_PI)
	{
		return mod + twoPi;
	}
	return mod;
}

void Camera::Rotate(float dx, float dy) noexcept
{
	m_Yaw = wrap_angle(m_Yaw + dx * m_LookSpeed);
	m_Pitch = std::clamp(static_cast<double>(m_Pitch + dy * m_LookSpeed), 0.995f * -M_PI / 2.0f, 0.995f * M_PI / 2.0f);
}

void Camera::Move(XMFLOAT3 translation)
{	
	XMStoreFloat3(&translation, XMVector3Transform(
		XMLoadFloat3(&translation),
		XMMatrixRotationRollPitchYaw(m_Pitch, m_Yaw, 0.0f) *
		XMMatrixScaling(m_MoveSpeed, m_MoveSpeed, m_MoveSpeed)
	));
	m_Pos = {
		m_Pos.x + translation.x,
		m_Pos.y + translation.y,
		m_Pos.z + translation.z
	};
}

DirectX::XMFLOAT3 Camera::GetPos() const noexcept
{
	return m_Pos;
}

void Camera::ShowGUI()
{
	static bool open = true;
	if (!open)
		return;

	ImGui::Begin("Camera Settings", &open);
	ImGui::SliderFloat("Move Speed", &m_MoveSpeed, 5.f, 100.f);
	ImGui::End();
}