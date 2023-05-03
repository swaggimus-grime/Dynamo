#pragma once

#include "Bindable.h"
#include <d3d11.h>

enum class SAMPLER_MODE {
	NEAREST, ANISOTROPIC, TRILINEAR
};

class Sampler : public Bindable {
public:
	Sampler(Graphics& g, SAMPLER_MODE mode = SAMPLER_MODE::NEAREST, UINT slot = 0u, bool mipped = true);
	inline SAMPLER_MODE Mode() const { return m_Mode; }
	inline bool Mipped() const { return m_Mipped; }
	void SetMode(SAMPLER_MODE mode);
	void SetMipped(bool mipped);

	virtual void Bind(Graphics& g) override;
	static std::string CreateHash(SAMPLER_MODE mode, UINT slot, bool mipped);
	static Shared<Sampler> Evaluate(Graphics& g, SAMPLER_MODE mode = SAMPLER_MODE::NEAREST, UINT slot = 0u, bool mipped = true);

private:
	SAMPLER_MODE m_Mode;
	UINT m_Slot;
	UINT m_State;
	ComPtr<ID3D11SamplerState> m_States[6];
	bool m_Mipped;
};

class ShadowSampler : public Bindable
{
public:
	ShadowSampler(Graphics& g);
	virtual void Bind(Graphics& g) override;
	void SetBilinear(bool bilin);
	void SetHwPcf(bool hwPcf);
	bool GetBilinear() const;
	bool GetHwPcf() const;
private:
	UINT GetCurrentSlot() const;
	static size_t ShadowSamplerIndex(bool bilin, bool hwPcf);
	static Microsoft::WRL::ComPtr<ID3D11SamplerState> MakeSampler(Graphics& gfx, bool bilin, bool hwPcf);
protected:
	size_t curSampler;
	Microsoft::WRL::ComPtr<ID3D11SamplerState> samplers[4];
};