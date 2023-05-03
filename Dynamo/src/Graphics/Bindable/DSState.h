#pragma once

#include "Graphics.h"
#include "Bindable.h"
#include <d3d11.h>

enum class DS_MODE {
	DEPTH_FIRST,
	DEPTH_DEFAULT,
	STENCIL_WRITE,
	STENCIL_MASK,
	STENCIL_OFF
};

class DSState : public Bindable {
public:
	DSState(Graphics& g, DS_MODE mode = DS_MODE::DEPTH_DEFAULT);
	virtual void Bind(Graphics& g) override;
	static std::string CreateHash(DS_MODE mode);
	static Shared<DSState> Evaluate(Graphics& g, DS_MODE mode = DS_MODE::DEPTH_DEFAULT);
private:
	ComPtr<ID3D11DepthStencilState> m_State;
};