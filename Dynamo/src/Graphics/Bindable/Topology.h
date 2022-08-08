#pragma once 

#include "Bindable.h"
#include <d3d11.h>

class Topology : public Bindable {
public:
	Topology(Graphics& g, D3D11_PRIMITIVE_TOPOLOGY top = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	virtual void Bind(Graphics& g) override;
	static std::string CreateHash(D3D11_PRIMITIVE_TOPOLOGY top);
	static Shared<Topology> Evaluate(Graphics& g, D3D11_PRIMITIVE_TOPOLOGY top = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
private:
	D3D11_PRIMITIVE_TOPOLOGY m_Top;
};