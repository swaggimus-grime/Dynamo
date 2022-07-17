#pragma once

#include "Step.h"

class Technique {
public:
	Technique() = default;
	void Submit(class Graphics& g, class Renderable& r);
	void AddStep(const Step& step);

private:
	std::vector<Step> m_Steps;
};