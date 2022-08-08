#pragma once

#include "Pass.h"

class ClearPass : public Pass {
public:
	ClearPass(const std::string& name);
	virtual void Run(Graphics& g) override;

private:
	Shared<class Buffer> m_Buffer;
};