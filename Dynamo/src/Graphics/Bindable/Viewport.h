#pragma once

#include "Bindable.h"

class Viewport : public Bindable
{
public:
	Viewport(Graphics& g)
		:
		Viewport(g, (float)g.Width(), (float)g.Height())
	{
	}

	Viewport(Graphics& g, float width, float height)
	{
		vp.Width = width;
		vp.Height = height;
		vp.MinDepth = 0.0f;
		vp.MaxDepth = 1.0f;
		vp.TopLeftX = 0.0f;
		vp.TopLeftY = 0.0f;
	}

	virtual void Bind(Graphics& g) override
	{
		g.DC().RSSetViewports(1u, &vp);
	}

private:
	D3D11_VIEWPORT vp{};
};