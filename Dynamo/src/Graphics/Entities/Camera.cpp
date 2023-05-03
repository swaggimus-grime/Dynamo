#include "dynamopch.h"
#include "Camera.h"

Camera::Camera(const std::string& name, XMFLOAT3 pos)
	:m_Pos(pos), m_Name(std::move(name)),
	m_Proj(XMMatrixPerspectiveLH(1.f, 9 / 16.f, 0.5f, 400.0f)),
	m_Pitch(0), m_Yaw(0)
{
}

XMMATRIX Camera::LookAt() const
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
	
	SetPos({ 
		m_Pos.x + translation.x,
		m_Pos.y + translation.y,
		m_Pos.z + translation.z
	});
}

DirectX::XMFLOAT3 Camera::GetPos() const noexcept
{
	return m_Pos;
}

//void Camera::ShowGUI(Graphics& g)
//{
//	static bool open = true;
//	if (!open)
//		return;
//
//	ImGui::Begin("Camera Settings", &open);
//	ImGui::SliderFloat("Move Speed", &m_MoveSpeed, 250.f, 1000.f);
//	ImGui::End();
//}