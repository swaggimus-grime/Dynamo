#pragma once

#include "Bindable.h"
#include "Graphics.h"

enum class RS_MODE {
	CULL_BACK, CULL_FRONT, CULL_NONE, WIREFRAME 
};

class Rasterizer : public Bindable {
public:
	Rasterizer(Graphics& g);
	Rasterizer(Graphics& g, RS_MODE mode);
	virtual void Bind(Graphics& g) override;
private:
	ComPtr<ID3D11RasterizerState> m_State;
};