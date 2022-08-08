#pragma once

#include "BindPass.h"
#include "Render/Work.h"

class RenderPass : public BindPass {
public:
	using BindPass::BindPass;
	void AddWork(const Work& work);
	virtual void Clear() override;
	void Run(class Graphics& g);

private:
	std::vector<Work> m_Works;
};