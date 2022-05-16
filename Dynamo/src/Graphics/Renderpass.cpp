#include "dynamopch.h"
#include "Renderpass.h"
#include "Shader.h"
#include "Sampler.h"
#include "DSState.h"

SkyboxPass::SkyboxPass(Graphics& g)
{
	//Shader
	auto shader = std::make_shared<Shader>(g, L"res/shaders/Skyboxvs.cso", L"res/shaders/Skyboxps.cso");

	//Sampler
	auto sampler = std::make_shared<CubeSampler>(g);

	//Skybox
	m_Skybox = std::make_unique<Skybox>(g, L"res/skyboxes/ocean", shader);

	m_Bindables.push_back(std::move(shader));
	m_Bindables.push_back(std::move(sampler));
}

void SkyboxPass::Render(Graphics& g) const
{
	for (const auto& b : m_Bindables)
		b->Bind(g);

	m_Skybox->Render(g);
}
