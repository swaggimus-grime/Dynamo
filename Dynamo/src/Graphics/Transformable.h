#pragma once

#include <d3d11.h>
#include "GUI/GUIable.h"

struct Transform {
	XMFLOAT3 Pos;
	XMFLOAT3 Rot;
	XMFLOAT3 Scale;

	Transform(XMFLOAT3 pos, XMFLOAT3 rot = { 0.f, 0.f, 0.f }, XMFLOAT3 scale = { 1.f, 1.f, 1.f })
		:Pos(pos), Rot(rot), Scale(scale)
	{
	}
};

class Transformable : public GUIable {
public:
	Transformable(const Transform& t);
	virtual void Translate(float x, float y, float z);
	virtual void Rotate(float pitch, float yaw, float roll);
	virtual void Scale(float x, float y, float z);
	virtual void Translate(const XMFLOAT3& t);
	virtual void Rotate(const XMFLOAT3& r);
	virtual void Scale(const XMFLOAT3& s);
	virtual void ScaleDelta(const XMFLOAT3& sd);

	virtual void ShowGUI() override;
protected:
	XMMATRIX GetModelMat() const { return m_Scale * m_Rot * m_Trans; }

private:
	XMMATRIX m_Trans;
	XMMATRIX m_Rot;
	XMMATRIX m_Scale;

	XMFLOAT3 m_PVec;
	XMFLOAT3 m_RVec;
	XMFLOAT3 m_SVec;
};