#pragma once

#include "IndexBuffer.h"
#include "Technique.h"

class Renderable {
public:
	Renderable() = default;
	void Submit(Graphics& g);
	void Bind(Graphics& g) const;
	inline UINT IndexCount() const { return m_IBuff->Count(); }
	virtual XMMATRIX ModelMat() const = 0;
protected:
	void AddTechnique(const class Technique& tech);
	
protected:
	std::vector<Technique> m_Techs;
	Shared<class VertexBuffer> m_VBuff;
	Shared<IndexBuffer> m_IBuff;
	D3D11_PRIMITIVE_TOPOLOGY m_Top;
};