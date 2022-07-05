#pragma once

class RenderPass {
public:
	virtual void Run(class Graphics& g) = 0;
};