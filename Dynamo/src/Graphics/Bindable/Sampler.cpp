#include "dynamopch.h"
#include "Sampler.h"

#include "Graphics.h"
#include "Binds.h"

Sampler::Sampler(Graphics& g, SAMPLER_MODE mode, UINT slot, bool mipped)
	:m_Mipped(mipped), m_Mode(mode), m_Slot(slot)
{
	for (UINT i = 0; i <= (UINT)SAMPLER_MODE::TRILINEAR; i++)
	{
		UCHAR mip = 0;
		do {
			D3D11_SAMPLER_DESC sampDesc = CD3D11_SAMPLER_DESC(CD3D11_DEFAULT{});
			sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
			sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;

			switch ((SAMPLER_MODE)i) {
			case SAMPLER_MODE::TRILINEAR:
				sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
				break;
			case SAMPLER_MODE::NEAREST:
				sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
				break;
			case SAMPLER_MODE::ANISOTROPIC:
				sampDesc.Filter = D3D11_FILTER_ANISOTROPIC;
				sampDesc.MaxAnisotropy = D3D11_REQ_MAXANISOTROPY;
				break;
			default:
				break;
			}

			if (mipped) {
				sampDesc.MipLODBias = 0.f;
				sampDesc.MinLOD = 0;
				sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
			}

			g.Device().CreateSamplerState(&sampDesc, &m_States[i * 2 + mip]);
		} while (mip++ == false);
	}

	SetMode(mode);
	SetMipped(mipped);
}

void Sampler::SetMode(SAMPLER_MODE mode)
{
	m_State = (UINT)mode * 2 + (UINT)m_Mipped;
	m_Mode = mode;
}

void Sampler::SetMipped(bool mipped)
{
	m_State = (UINT)m_Mode * 2 + (UINT)mipped;
	m_Mipped = mipped;
}

void Sampler::Bind(Graphics& g)
{
	g.DC().PSSetSamplers(m_Slot, 1, m_States[m_State].GetAddressOf());
}

std::string Sampler::CreateHash(SAMPLER_MODE mode, UINT slot, bool mipped)
{
	return typeid(Sampler).name() + "#"s + std::to_string((static_cast<UINT>(mode))) + std::to_string(slot) + std::to_string(mipped);
}

Shared<Sampler> Sampler::Evaluate(Graphics& g, SAMPLER_MODE mode, UINT slot, bool mipped)
{
	return Binds::Evaluate<Sampler>(g, mode, slot, mipped);
}

ShadowSampler::ShadowSampler(Graphics& gfx)
{
	for (size_t i = 0; i < 4; i++)
	{
		curSampler = i;
		samplers[i] = MakeSampler(gfx, GetBilinear(), GetHwPcf());
	}
	SetBilinear(true);
	SetHwPcf(true);
}

void ShadowSampler::SetBilinear(bool bilin)
{
	curSampler = (curSampler & ~0b01) | (bilin ? 0b01 : 0);
}

void ShadowSampler::SetHwPcf(bool hwPcf)
{
	curSampler = (curSampler & ~0b10) | (hwPcf ? 0b10 : 0);
}

bool ShadowSampler::GetBilinear() const
{
	return curSampler & 0b01;
}

bool ShadowSampler::GetHwPcf() const
{
	return curSampler & 0b10;
}

UINT ShadowSampler::GetCurrentSlot() const
{
	return GetHwPcf() ? 1 : 2;
}

size_t ShadowSampler::ShadowSamplerIndex(bool bilin, bool hwPcf)
{
	return 0b01 + 0b10;
}

Microsoft::WRL::ComPtr<ID3D11SamplerState> ShadowSampler::MakeSampler(Graphics& g, bool bilin, bool hwPcf)
{

	D3D11_SAMPLER_DESC samplerDesc = CD3D11_SAMPLER_DESC{ CD3D11_DEFAULT{} };

	samplerDesc.BorderColor[0] = 1.0f;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;

	if (hwPcf)
	{
		samplerDesc.Filter = bilin ? D3D11_FILTER_COMPARISON_MIN_MAG_MIP_LINEAR : D3D11_FILTER_COMPARISON_MIN_MAG_MIP_POINT;
		samplerDesc.ComparisonFunc = D3D11_COMPARISON_LESS_EQUAL;
	}
	else
	{
		samplerDesc.Filter = bilin ? D3D11_FILTER_MIN_MAG_MIP_LINEAR : D3D11_FILTER_MIN_MAG_MIP_POINT;
	}

	Microsoft::WRL::ComPtr<ID3D11SamplerState> pSampler;
	g.Device().CreateSamplerState(&samplerDesc, &pSampler);
	return std::move(pSampler);
}

void ShadowSampler::Bind(Graphics& g) 
{
	g.DC().PSSetSamplers(GetCurrentSlot(), 1, samplers[curSampler].GetAddressOf());
}