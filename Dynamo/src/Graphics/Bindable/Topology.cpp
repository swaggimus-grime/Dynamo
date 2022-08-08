#include "dynamopch.h"
#include "Topology.h"

#include "Graphics.h"
#include "Binds.h"

Topology::Topology(Graphics& g, D3D11_PRIMITIVE_TOPOLOGY top)
	:m_Top(top)
{
}

void Topology::Bind(Graphics& g)
{
	g.DC().IASetPrimitiveTopology(m_Top);
}

std::string Topology::CreateHash(D3D11_PRIMITIVE_TOPOLOGY top)
{
	return typeid(Topology).name() + std::to_string((UINT)top);
}

Shared<Topology> Topology::Evaluate(Graphics& g, D3D11_PRIMITIVE_TOPOLOGY top)
{
	return Binds::Evaluate<Topology>(g, top);
}
