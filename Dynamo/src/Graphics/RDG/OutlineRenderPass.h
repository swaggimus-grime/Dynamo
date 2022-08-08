#pragma once

#include "RenderPass.h"

class OutlineRenderPass : public RenderPass {
public:
	OutlineRenderPass(Graphics& g, const std::string& name);
};