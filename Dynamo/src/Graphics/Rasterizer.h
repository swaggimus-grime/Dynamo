#pragma once

#include "Bindable.h"
#include "Graphics.h"

enum class RS_MODE {
	CULL_BACK, CULL_FRONT, CULL_NONE, WIREFRAME 
};

class Rasterizer : public Bindable {
public:
	Rasterizer(Graphics& g, RS_MODE mode = RS_MODE::CULL_BACK);
	virtual void Bind(Graphics& g) override;
	static std::string CreateHash(RS_MODE mode);
	static Shared<Rasterizer> Evaluate(Graphics& g, RS_MODE mode = RS_MODE::CULL_BACK);
private:
	ComPtr<ID3D11RasterizerState> m_State;
};