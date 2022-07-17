#pragma once

#include "Work.h"

class RenderPass {
public:
	RenderPass() = default;
	void AddWork(const Work& work);
	void Run(class Graphics& g) const;

private:
	std::vector<Work> m_Works;
};