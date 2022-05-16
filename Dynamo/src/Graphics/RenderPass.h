#pragma once

#include "Graphics.h"
#include "Bindable.h"
#include "Skybox.h"

class SkyboxPass : public Renderable {
public:
	SkyboxPass(Graphics& g);
	virtual void Render(Graphics& g) const override;
private:
	std::unique_ptr<Skybox> m_Skybox;
	std::vector<std::shared_ptr<Bindable>> m_Bindables;
};