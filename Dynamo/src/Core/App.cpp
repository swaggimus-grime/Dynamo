#include "dynamopch.h"
#include "App.h"
#include "GUI/Gui.h"
#include "Graphics/Shader.h"
#include "Graphics/Camera.h"
#include "Graphics/Model.h"
#include <chrono>
#include "Graphics/Framebuffer.h"
#include "Graphics/Buffer.h"
#include "Graphics/Skybox.h"
#include "Graphics/Sampler.h"
#include "Graphics/DSState.h"

std::shared_ptr<Framebuffer> fb;

App::App(const std::string& name, UINT32 width, UINT32 height)
{
	Gui::Init();
	m_Window = std::make_unique<Window>(name, width, height);

	m_Camera = std::make_shared<Camera>(XMFLOAT3(0.f, -50.f, -100.f));
	fb = std::make_shared<Framebuffer>(m_Window->GetGraphics(), m_Window->GetWidth(), m_Window->GetHeight(), true);
	m_Skybox = std::make_unique<Skybox>(m_Window->GetGraphics(), L"res/skyboxes/ocean");
	m_GF = std::make_unique<Model>(m_Window->GetGraphics(), "res/models/golden_freddy/scene.gltf");
}

App::~App()
{
	m_Window.release();
	Gui::Shutdown();
}

void App::UserInput(float deltaTime)
{
	if (m_Window->GetInput().IsPressed('W'))
		m_Camera->Move(XMFLOAT3(0.f, 0.f, deltaTime));
	if (m_Window->GetInput().IsPressed('S'))
		m_Camera->Move(XMFLOAT3(0.f, 0.f, -deltaTime));
	if (m_Window->GetInput().IsPressed('A'))
		m_Camera->Move(XMFLOAT3(-deltaTime, 0.f, 0.f));
	if (m_Window->GetInput().IsPressed('D'))
		m_Camera->Move(XMFLOAT3(deltaTime, 0.f, 0.f));
	if (m_Window->GetInput().IsPressed('Q'))
		m_Camera->Move(XMFLOAT3(0.f, deltaTime, 0.f));
	if (m_Window->GetInput().IsPressed('E'))
		m_Camera->Move(XMFLOAT3(0.f, -deltaTime, 0.f));
	if (m_Window->GetInput().IsPressed(VK_ESCAPE))
		PostQuitMessage(0);

	while (const auto delta = m_Window->GetInput().ReadMouseDelta())
		m_Camera->Rotate((float)delta->x, (float)delta->y);
}

INT App::Run()
{
	std::optional<INT> ret;
	using namespace std::chrono;
	const auto startTime = steady_clock::now();
	float prevTime = 0.f;
	while (!(ret = m_Window->Update())) {
		float currentTime = duration<float>(steady_clock::now() - startTime).count();
		float deltaTime = currentTime - prevTime;
		prevTime = currentTime;

		UserInput(deltaTime);
		fb->Bind(m_Window->GetGraphics());
		fb->Clear(m_Window->GetGraphics());
		m_Window->GetGraphics().BeginFrame(*m_Camera);

		m_Skybox->Render(m_Window->GetGraphics());
		m_GF->Render(m_Window->GetGraphics());

		m_Window->GetGraphics().EndFrame();
	}

	return *ret;
}
