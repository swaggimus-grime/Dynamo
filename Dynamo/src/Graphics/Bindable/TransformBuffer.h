#pragma once

#include "ConstantBuffer.h"
#include "GUI/GUIable.h"
#include "Animation/Bone.h"

struct Transform {
	XMMATRIX Model = XMMatrixIdentity();
	XMMATRIX ModelView = XMMatrixIdentity();
	XMMATRIX MVP = XMMatrixIdentity();
};

struct ShadowCamTransform {
	XMMATRIX view = XMMatrixIdentity();
};

struct BoneTransform {
	XMMATRIX boneMats[MAX_NUM_BONES];
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

class ShadowCamBuffer : public VertexConstantBuffer<ShadowCamTransform>{
public:
	ShadowCamBuffer(Graphics& g, UINT slot = 1u);
	virtual void Bind(Graphics& g) override;
	void SetCamera(const Camera* camera);

private:
	const Camera* m_Camera;
};

class BoneBuffer : public VertexConstantBuffer<BoneTransform> {
public:
	BoneBuffer(Graphics& g, UINT slot = 2u);
	virtual void Bind(Graphics& g) override;
	inline void SetModel(class AnimModel* model) { m_Model = model; }

private:
	AnimModel* m_Model;
};

class Transformable : public GUIable {
public:
	Transformable();
	virtual void SetPos(const XMFLOAT3& pos);
	virtual void SetRot(const XMFLOAT3& rot);
	virtual void SetScale(const XMFLOAT3& scale);
	virtual void Move(const XMFLOAT3& delta);
	virtual void Rotate(const XMFLOAT3& delta);
	virtual void Rotate(float dx, float dy);

	XMFLOAT3 Pos() const { return m_Pos; }
	XMFLOAT3 Rot() const { return m_Ori; }
	XMFLOAT3 Scale() const { return m_Size; }

	inline XMMATRIX TransformMat() const { return m_Scale * m_Rot * m_Trans; }

	virtual void ShowGUI(Graphics& g) override;

protected:
	XMMATRIX m_Trans = XMMatrixIdentity();
	XMMATRIX m_Rot = XMMatrixIdentity();
	XMMATRIX m_Scale = XMMatrixIdentity();

	XMFLOAT3 m_Pos = { 0.f, 0.f, 0.f };
	XMFLOAT3 m_Ori = { 0.f, 0.f, 0.f };
	XMFLOAT3 m_Size = { 1.f, 1.f, 1.f };
		
};