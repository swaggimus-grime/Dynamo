#pragma once

#include "Bindable/IndexBuffer.h"
#include "Technique.h"

class Renderable {
public:
	Renderable() = default;
	void Submit();
	void LinkToRDG(class RDG& graph);
	virtual void Bind(Graphics& g) const;
	inline UINT IndexCount() const { return m_IBuff->Count(); }
	virtual XMMATRIX ModelMat() const = 0;
protected:
	void AddTechnique(class Technique tech);
	
protected:
	std::vector<Technique> m_Techs;
	Shared<class VertexBuffer> m_VBuff;
	Shared<IndexBuffer> m_IBuff;
	Shared<class Topology> m_Top;
};