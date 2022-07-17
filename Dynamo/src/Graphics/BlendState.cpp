//#include "dynamopch.h"
//#include "BlendState.h"
//#include "Graphics.h"
//
//BlendState::BlendState(Graphics& g, BLEND_MODE mode, const std::optional<float>& blendFactor)
//	:m_Factor(std::move(blendFactor))
//{
//	D3D11_BLEND_DESC blendDesc = CD3D11_BLEND_DESC(CD3D11_DEFAULT{});
//	auto& rt = blendDesc.RenderTarget[0];
//	switch (mode) {
//	case BLEND_MODE::NONE:
//		rt.BlendEnable = FALSE;
//		break;
//	default:
//		rt.BlendEnable = TRUE;
//		if (m_Factor) {
//			rt.SrcBlend = D3D11_BLEND_BLEND_FACTOR;
//			rt.DestBlend = D3D11_BLEND_INV_BLEND_FACTOR;
//		}
//		else {
//			rt.SrcBlend = D3D11_BLEND_SRC_ALPHA;
//			rt.DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
//		}
//		break;
//	}
//
//	g.Device().CreateBlendState(&blendDesc, m_State.GetAddressOf());
//}
//
//void BlendState::Bind(Graphics& g)
//{
//	g.DC().OMSetBlendState(m_State.Get(), m_Factor ? &(*m_Factor) : nullptr, 0xFFFFFFFF);
//}
