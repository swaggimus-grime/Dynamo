#pragma once

#include "ConstantBuffer.h"
#include "GUI/GUIable.h"

struct Transform {
	XMMATRIX Model = XMMatrixIdentity();
	XMMATRIX ModelView = XMMatrixIdentity();
	XMMATRIX MVP = XMMatrixIdentity();
};

class TransformBuffer : public VertexConstantBuffer<Transform> {
public:
	TransformBuffer(Graphics& g, UINT slot = 0);
	virtual void Bind(Graphics& g) override;
	virtual void SetParent(const class Renderable& parent) override
	{
		m_Renderable = &parent;
	}
private:
	Transform m_Mats;
	const Renderable* m_Renderable;
};

class Transformable : public GUIable {
public:
	Transformable();
	virtual void SetPos(const XMFLOAT3& pos);
	virtual void SetRot(const XMFLOAT3& rot);
	virtual void SetScale(const XMFLOAT3& scale);

	XMFLOAT3 Pos() const { return m_Pos; }
	XMFLOAT3 Rot() const { return m_Ori; }
	XMFLOAT3 Scale() const { return m_Size; }

	inline XMMATRIX TransformMat() const { return m_Scale * m_Rot * m_Trans; }

	virtual void ShowGUI() override;

protected:
	XMMATRIX m_Trans = XMMatrixIdentity();
	XMMATRIX m_Rot = XMMatrixIdentity();
	XMMATRIX m_Scale = XMMatrixIdentity();

	XMFLOAT3 m_Pos = { 0.f, 0.f, 0.f };
	XMFLOAT3 m_Ori = { 0.f, 0.f, 0.f };
	XMFLOAT3 m_Size = { 1.f, 1.f, 1.f };
		
};