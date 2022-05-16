#include "dynamopch.h"
#include "Sampler.h"
#include "Graphics.h"

CubeSampler::CubeSampler(Graphics& g)
{
	D3D11_SAMPLER_DESC sampDesc{};
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.MaxAnisotropy = D3D11_REQ_MAXANISOTROPY;
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
	g.Device().CreateSamplerState(&sampDesc, &m_State);
}

void CubeSampler::Bind(Graphics& g) const
{
	g.DC().PSSetSamplers(0, 1, m_State.GetAddressOf());
}

