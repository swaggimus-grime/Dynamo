#pragma once

#include "RenderPass.h"

class NoShadowPass : public RenderPass {
public:
	NoShadowPass();
	virtual void Run(Graphics& g) override;
};