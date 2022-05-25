#include "dynamopch.h"
#include "Sampler.h"
#include "Graphics.h"

Sampler::Sampler(Graphics& g, SAMPLER_MODE mode)
{
	D3D11_SAMPLER_DESC sampDesc = CD3D11_SAMPLER_DESC(CD3D11_DEFAULT{});
	switch (mode) {
	case SAMPLER_MODE::LINEAR_WRAP:
		sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		break;
	case SAMPLER_MODE::NEAREST_WRAP:
		sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
		sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		break;
	case SAMPLER_MODE::LINEAR_MIRROR:
		sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_MIRROR;
		sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_MIRROR;
		break;
	default:
		break;
	}
	
	sampDesc.MaxAnisotropy = D3D11_REQ_MAXANISOTROPY;
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
	g.Device().CreateSamplerState(&sampDesc, &m_State);
}

void Sampler::Bind(Graphics& g)
{
	g.DC().PSSetSamplers(0, 1, m_State.GetAddressOf());
}

