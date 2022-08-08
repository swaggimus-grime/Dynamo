#pragma once

#include "Graphics.h"
#include "BindPass.h"

class ScreenPass : public BindPass {
public:
	ScreenPass(Graphics& g, const std::string& name);
	void Run(Graphics& g);
};