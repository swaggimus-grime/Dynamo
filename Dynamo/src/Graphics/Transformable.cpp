#include "dynamopch.h"
#include "Transformable.h"
#include <imgui.h>

Transformable::Transformable(const Transform& t)
    :m_PVec(t.Pos), m_RVec(t.Rot), m_SVec(t.Scale)
{
    m_Trans = m_Rot = m_Scale = XMMatrixIdentity();
    Translate(m_PVec);
    Rotate(m_RVec);
    Scale(m_SVec);
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

void Transformable::Translate(const XMFLOAT3& t)
{
    Translate(t.x, t.y, t.z);
}

void Transformable::Rotate(const XMFLOAT3& r)
{
    Rotate(r.x, r.y, r.z);
}

void Transformable::Scale(const XMFLOAT3& s)
{
    Scale(s.x, s.y, s.z);
}

void Transformable::ScaleDelta(const XMFLOAT3& sd)
{
    Scale(m_SVec + sd);
}

void Transformable::ShowGUI(Graphics& g)
{
    if (ImGui::SliderFloat3("Position", reinterpret_cast<FLOAT*>(&m_PVec.x), -1000.f, 1000.f))
        Translate(m_PVec);
    else if (ImGui::SliderFloat3("Rotate", reinterpret_cast<FLOAT*>(&m_RVec.x), 0.f, 2 * M_PI))
        Rotate(m_RVec);
    else if (ImGui::SliderFloat3("Scale", reinterpret_cast<FLOAT*>(&m_SVec.x), -100.f, 100.f))
        Scale(m_SVec);
}