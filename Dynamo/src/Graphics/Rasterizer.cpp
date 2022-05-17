#include "dynamopch.h"
#include "Rasterizer.h"

Rasterizer::Rasterizer(Graphics& g)
{
	D3D11_RASTERIZER_DESC rastDesc = CD3D11_RASTERIZER_DESC(CD3D11_DEFAULT{});
	g.Device().CreateRasterizerState(&rastDesc, &m_State);
}

Rasterizer::Rasterizer(Graphics& g, RS_MODE mode)
{
	D3D11_RASTERIZER_DESC rastDesc = CD3D11_RASTERIZER_DESC(CD3D11_DEFAULT{});
	switch (mode) {
	case RS_MODE::CULL_FRONT:
		rastDesc.CullMode = D3D11_CULL_FRONT;
		break;
	case RS_MODE::CULL_NONE:
		rastDesc.CullMode = D3D11_CULL_NONE;
		break;
	case RS_MODE::WIREFRAME:
		rastDesc.FillMode = D3D11_FILL_WIREFRAME;
		break;
	default:
		break;
	}

	g.Device().CreateRasterizerState(&rastDesc, &m_State);
}

void Rasterizer::Bind(Graphics& g) const
{
	g.DC().RSSetState(m_State.Get());
}
