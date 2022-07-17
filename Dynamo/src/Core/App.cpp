#include "dynamopch.h"
#include "App.h"
#include "GUI/Gui.h"
#include "Graphics/Shader.h"
#include "Graphics/Camera.h"
#include "Graphics/Model.h"
#include <chrono>
#include "Graphics/Skybox.h"
#include "Graphics/Sampler.h"
#include "Graphics/DSState.h"
#include <imgui.h>
#include "Graphics/Light.h"
#include "Graphics/Scene.h"
#include "Graphics/NegativePass.h"
#include "Graphics/ShadowPass.h"
#include "Graphics/Animation.h"
#include "Graphics/Animator.h"
#include "Graphics/AnimModel.h"
#include "Graphics/NoShadowPass.h"

#include "Graphics/TestCube.h"

App::App(const std::string& name, UINT32 width, UINT32 height)
{
	Gui::Init();
	m_Window = std::make_unique<Window>(name, width, height);
	auto& g = m_Window->GetGraphics();

	m_Camera = std::make_shared<Camera>(XMFLOAT3(0.f, 20.f, -100.f));
	m_Cube = MakeUnique<TestCube>(g, 4.f);
	m_Skybox = MakeUnique<Skybox>(g, "res\\skyboxes\\yokohama");

	m_Cube->Submit(g);
	m_Skybox->Submit(g);
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

	while (const auto mouse = m_Window->GetInput().ReadMouseLPress()) {
		/*float x = (2.0f * mouse->x) / 1280.f - 1.0f;
		float y = 1.0f - (2.0f * mouse->y) / 720.f;
		float z = 1.0f;
		XMFLOAT3 ray_nds = XMFLOAT3(x, y, z);
		XMFLOAT4 ray_clip = XMFLOAT4(ray_nds.x, ray_nds.y, 1.0, 1.0);
		XMFLOAT4 ray_eye;
		XMStoreFloat4(&ray_eye, XMVector4Transform(XMLoadFloat4(&ray_clip), XMMatrixInverse(nullptr, m_Camera->Projection())));
		ray_eye = XMFLOAT4(ray_eye.x, ray_eye.y, 1.0, 0.0);
		XMFLOAT3 ray_world;
		XMStoreFloat3(&ray_world, XMVector4Transform(XMLoadFloat4(&ray_eye), XMMatrixInverse(nullptr, m_Camera->LookAt())));
		XMStoreFloat3(&ray_world, XMVector3Normalize(XMLoadFloat3(&ray_world)));*/
		
	}
}

void App::ShowGUI()
{
	static bool showCamera = false;
	if (ImGui::BeginMainMenuBar()) {
		if (ImGui::BeginMenu("File")) {
			if (ImGui::MenuItem("Open File")) 
				//m_GF->Reload(m_Window->GetGraphics(), Window::OpenDialogBox());

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
		m_Camera->ShowGUI(m_Window->GetGraphics());

	//m_Scene->ShowGUI(m_Window->GetGraphics());
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

		m_Window->GetGraphics().BeginFrame(*m_Camera);
		m_Window->GetGraphics().Run();
		ShowGUI();
		m_Window->GetGraphics().EndFrame();
	}

	return *ret;
}
