#include "dynamopch.h"
#include "DSState.h"

#include "Binds.h"

DSState::DSState(Graphics& g, DS_MODE mode)
{
	D3D11_DEPTH_STENCIL_DESC dsDesc = CD3D11_DEPTH_STENCIL_DESC{ CD3D11_DEFAULT{} };
	switch (mode) {
	case DS_MODE::DEPTH_FIRST:
		dsDesc.DepthEnable = true;
		dsDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
		dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		break;
	case DS_MODE::DEPTH_DEFAULT:
		dsDesc.DepthEnable = true;
		dsDesc.DepthFunc = D3D11_COMPARISON_LESS;
		dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		break;
	case DS_MODE::STENCIL_WRITE:
		dsDesc.StencilEnable = TRUE;
		dsDesc.StencilWriteMask = 0xFF;
		dsDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
		dsDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_REPLACE;
		break;
	case DS_MODE::STENCIL_MASK:
		dsDesc.StencilEnable = TRUE;
		dsDesc.StencilReadMask = 0xFF;
		dsDesc.FrontFace.StencilFunc = D3D11_COMPARISON_NOT_EQUAL;
		dsDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		break;
	}

	g.Device().CreateDepthStencilState(&dsDesc, m_State.GetAddressOf());
}

void DSState::Bind(Graphics& g)
{
	g.DC().OMSetDepthStencilState(m_State.Get(), 0xFF);
}

std::string DSState::CreateHash(DS_MODE mode)
{
	return typeid(DSState).name() + "#"s + std::to_string(static_cast<UINT>(mode));
}

Shared<DSState> DSState::Evaluate(Graphics& g, DS_MODE mode)
{
	return Binds::Evaluate<DSState>(g, mode);
}
