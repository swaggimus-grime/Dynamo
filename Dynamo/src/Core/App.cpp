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
#include <imgui.h>
#include "Graphics/Light.h"
#include "Graphics/Scene.h"

std::shared_ptr<Framebuffer> fb;

App::App(const std::string& name, UINT32 width, UINT32 height)
{
	Gui::Init();
	m_Window = std::make_unique<Window>(name, width, height);

	m_Camera = std::make_shared<Camera>(XMFLOAT3(0.f, -50.f, -100.f));
	fb = std::make_shared<Framebuffer>(m_Window->GetGraphics(), m_Window->GetWidth(), m_Window->GetHeight(), true);
	m_Skybox = std::make_shared<Skybox>(m_Window->GetGraphics(), L"res\\skyboxes\\yokohama");
	m_GF = std::make_shared<Model>(m_Window->GetGraphics(), "res\\models\\golden_freddy\\scene.gltf");
	m_PL = std::make_shared<PointLight>(m_Window->GetGraphics(), XMFLOAT3(0.f, 5.f, 10.f), XMFLOAT3(1.f, 1.f, 0.f));
	m_Scene = std::make_unique<Scene>();

	m_Scene->Submit(m_Skybox, "Skybox");
	m_Scene->Submit(m_PL, "Point Light");
	m_Scene->Submit(m_GF, "Model");
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

	while (const auto key = m_Window->GetInput().ReadKey()) {
		switch (*key) {
		case VK_TAB:
			if (m_Window->GetInput().RawDeltaEnabled()) {
				m_Window->ClipCursor(false);
				m_Window->GetInput().SetCursor(false);
			}
			else {
				m_Window->ClipCursor(true);
				m_Window->GetInput().SetCursor(true);
			}
			break;
		}
	}

	while (const auto delta = m_Window->GetInput().ReadMouseDelta())
		m_Camera->Rotate((float)delta->x, (float)delta->y);
}

void App::ShowGUI()
{
	static bool showCamera = false;
	if (ImGui::BeginMainMenuBar()) {
		if (ImGui::BeginMenu("File")) {
			if (ImGui::MenuItem("Open File")) {
				m_GF.reset();
				m_GF = std::make_unique<Model>(m_Window->GetGraphics(), m_Window->OpenDialogBox());
			}

			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("View")) {
			if (ImGui::MenuItem("Camera"))
				showCamera = !showCamera;

			ImGui::EndMenu();
		}

		ImGui::EndMainMenuBar();
	}

	if(showCamera)
		m_Camera->ShowGUI();

	m_Scene->ShowGUI(m_Window->GetGraphics());
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

		m_PL->Bind(m_Window->GetGraphics());
		m_Skybox->Render(m_Window->GetGraphics());
		m_GF->Render(m_Window->GetGraphics());
		m_PL->Render(m_Window->GetGraphics());

		ShowGUI();
		m_Window->GetGraphics().EndFrame();
	}

	return *ret;
}
