#pragma once

#include "Renderpass.h"
#include "RenderTarget.h"

class Negativepass : public Renderpass {
public:
	Negativepass(Graphics& g);
	virtual void Run(Graphics& g) override;

private:
	std::shared_ptr<class Shader> m_Shader;
	std::unique_ptr<class Plane> m_Surface;
	std::shared_ptr<RenderTarget> m_Target;
	std::unique_ptr<DSView> m_DS;
};