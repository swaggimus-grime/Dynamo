#include "dynamopch.h"
#include "Transformable.h"

Transformable::Transformable()
{
	m_Trans = m_Rot = m_Scale = XMMatrixIdentity();
}

void Transformable::Translate(float x, float y, float z)
{
	m_Trans = XMMatrixTranslation(x, y, z);
}

void Transformable::Rotate(float pitch, float yaw, float roll)
{
	m_Rot = XMMatrixRotationRollPitchYaw(pitch, yaw, roll);
}

void Transformable::Scale(float x, float y, float z)
{
	m_Scale = XMMatrixScaling(x, y, z);
}
