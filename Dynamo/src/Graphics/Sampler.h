#pragma once

#include "Bindable.h"
#include <d3d11.h>

class CubeSampler : public Bindable {
public:
	CubeSampler(Graphics& g);
	virtual void Bind(Graphics& g) const override;

private:
	ComPtr<ID3D11SamplerState> m_State;
};