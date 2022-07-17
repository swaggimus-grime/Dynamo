#pragma once

#include "Bindable.h"
#include <d3d11.h>

enum class SAMPLER_MODE {
	LINEAR_WRAP, NEAREST_WRAP, LINEAR_MIRROR, LINEAR_CLAMP, ANISO_WRAP
};

class Sampler : public Bindable {
public:
	Sampler(Graphics& g, SAMPLER_MODE mode = SAMPLER_MODE::LINEAR_WRAP);
	virtual void Bind(Graphics& g) override;
	static std::string CreateHash(SAMPLER_MODE mode);
	static Shared<Sampler> Evaluate(Graphics& g, SAMPLER_MODE mode = SAMPLER_MODE::LINEAR_WRAP);
private:
	ComPtr<ID3D11SamplerState> m_State;
};