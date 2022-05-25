#pragma once

#include <d3d11.h>

class Transformable {
public:
	Transformable();
	virtual void Translate(float x, float y, float z);
	virtual void Rotate(float pitch, float yaw, float roll);
	virtual void Scale(float x, float y, float z);

protected:
	XMMATRIX GetModelMat() const { return m_Scale * m_Rot * m_Trans; }

private:
	XMMATRIX m_Trans;
	XMMATRIX m_Rot;
	XMMATRIX m_Scale;
};