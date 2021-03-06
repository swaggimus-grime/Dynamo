#pragma once

#include "Graphics.h"
#include "Bindable.h"
#include <d3d11.h>

enum class DS_MODE {
	DEPTH_FIRST,
	DEPTH_DEFAULT,
	STENCIL_WRITE,
	STENCIL_MASK
};

class DSState : public Bindable {
public:
	DSState(Graphics& g);
	DSState(Graphics& g, DS_MODE mode);
	virtual void Bind(Graphics& g) override;
private:
	ComPtr<ID3D11DepthStencilState> m_State;
};