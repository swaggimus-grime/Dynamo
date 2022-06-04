#pragma once

#include "Bindable.h"
#include <d3d11.h>

enum class SAMPLER_MODE {
	LINEAR_WRAP, NEAREST_WRAP, LINEAR_MIRROR, LINEAR_CLAMP, ANISO_WRAP
};

class Sampler : public Bindable {
public:
	Sampler(Graphics& g, SAMPLER_MODE mode);
	virtual void Bind(Graphics& g) override;

private:
	ComPtr<ID3D11SamplerState> m_State;
};