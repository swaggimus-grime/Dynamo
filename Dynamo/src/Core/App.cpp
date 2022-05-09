#include "dynamopch.h"
#include "App.h"
#include "Graphics/Gui.h"
#include "Graphics/Shader.h"
#include "Graphics/Camera.h"
#include "Graphics/Model.h"
#include <chrono>

App::App(const std::string& name, UINT32 width, UINT32 height)
{
	Gui::Init();
	m_Window = std::make_unique<Window>(name, width, height);
	m_Graphics = std::make_unique<Graphics>(m_Window->GetHandle());

	m_ModelShader = std::make_shared<Shader>(m_Graphics->GetGPU(),
		L"res/shaders/Modelvs.cso", L"res/shaders/Modelps.cso");
	m_Model = std::make_shared<Model>(m_Graphics->GetGPU(), "res/models/golden_freddy/scene.gltf", m_ModelShader);
	m_Camera = std::make_shared<Camera>(XMFLOAT3(0.f, 0.f, -10.f));
}

App::~App()
{
}

void App::UserInput(float deltaTime)
{
	if (m_Window->GetInput().IsPressed('W'))
		m_Camera->Move(XMFLOAT3(0.f, 0.f, deltaTime));
	else if (m_Window->GetInput().IsPressed('S'))
		m_Camera->Move(XMFLOAT3(0.f, 0.f, -deltaTime));
	else if (m_Window->GetInput().IsPressed('A'))
		m_Camera->Move(XMFLOAT3(-deltaTime, 0.f, 0.f));
	else if (m_Window->GetInput().IsPressed('D'))
		m_Camera->Move(XMFLOAT3(deltaTime, 0.f, 0.f));
	else if (m_Window->GetInput().IsPressed('Q'))
		m_Camera->Move(XMFLOAT3(0.f, deltaTime, 0.f));
	else if (m_Window->GetInput().IsPressed('E'))
		m_Camera->Move(XMFLOAT3(0.f, -deltaTime, 0.f));
	else if (m_Window->GetInput().IsPressed(VK_ESCAPE))
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
		m_Graphics->BeginFrame(m_Camera);
		m_Graphics->Render(m_Model);
		m_Graphics->EndFrame();
	}

	return *ret;
}
