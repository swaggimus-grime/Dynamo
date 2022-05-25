#include "dynamopch.h"
#include "DSState.h"

DSState::DSState(Graphics& g)
{
	D3D11_DEPTH_STENCIL_DESC dsDesc{};
	dsDesc.DepthEnable = true;
	dsDesc.DepthFunc = D3D11_COMPARISON_LESS;
	dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	g.Device().CreateDepthStencilState(&dsDesc, m_State.GetAddressOf());
}

DSState::DSState(Graphics& g, DS_MODE mode)
{
	D3D11_DEPTH_STENCIL_DESC dsDesc = CD3D11_DEPTH_STENCIL_DESC{ CD3D11_DEFAULT{} };
	switch (mode) {
	case DS_MODE::FIRST:
		dsDesc.DepthEnable = true;
		dsDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
		dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		break;
	case DS_MODE::DEFAULT:
		dsDesc.DepthEnable = true;
		dsDesc.DepthFunc = D3D11_COMPARISON_LESS;
		dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		break;
	}

	g.Device().CreateDepthStencilState(&dsDesc, m_State.GetAddressOf());
}

void DSState::Bind(Graphics& g)
{
	g.DC().OMSetDepthStencilState(m_State.Get(), 0xFF);
}
