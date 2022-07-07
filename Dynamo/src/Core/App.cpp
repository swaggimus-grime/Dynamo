#include "dynamopch.h"
#include "App.h"
#include "GUI/Gui.h"
#include "Graphics/Shader.h"
#include "Graphics/Camera.h"
#include "Graphics/Model.h"
#include <chrono>
#include "Graphics/Buffer.h"
#include "Graphics/Skybox.h"
#include "Graphics/Sampler.h"
#include "Graphics/DSState.h"
#include <imgui.h>
#include "Graphics/Light.h"
#include "Graphics/Scene.h"
#include "Graphics/NegativePass.h"
#include "Graphics/ShadowPass.h"
#include "Graphics/Selector.h"
#include "Graphics/Animation.h"
#include "Graphics/Animator.h"
#include "Graphics/AnimModel.h"
#include "Graphics/NoShadowPass.h"

Transform modelTransform(XMFLOAT3(80.f, 0.f, 0.f), XMFLOAT3(M_PI / 2, 0.f, 0.f));

App::App(const std::string& name, UINT32 width, UINT32 height)
{
	Gui::Init();
	m_Window = std::make_unique<Window>(name, width, height);

	m_Camera = std::make_shared<Camera>(XMFLOAT3(0.f, 20.f, -100.f));
	m_Skybox = std::make_shared<Skybox>(m_Window->GetGraphics(), L"res\\skyboxes\\yokohama");
	m_GF = std::make_shared<Model>(m_Window->GetGraphics(), "res\\models\\golden_freddy\\scene.gltf", modelTransform);
	m_Sponza = std::make_shared<Model>(m_Window->GetGraphics(), "res\\models\\sponza\\Sponza.gltf", XMFLOAT3(160.f, 0.f, 0.f));
	m_PL = std::make_shared<PointLight>(m_Window->GetGraphics(), XMFLOAT3(0.f, 10.f, -10.f), XMFLOAT3(10.f, 10.f, 5.f));
	m_Scene = std::make_shared<Scene>();
	//m_ShadowPass = std::make_unique<Shadowpass>(m_Window->GetGraphics(), m_PL);
	//m_NegPass = std::make_unique<NegativePass>(m_Window->GetGraphics());
	m_Pass = std::make_unique<NoShadowPass>();
	m_Selector = std::make_unique<Selector>(m_Window->GetGraphics());

	m_Freddy = std::make_shared<AnimModel>(m_Window->GetGraphics(), "res\\models\\freddy\\scene.gltf",         Transform{ {   0.f, 0.f, 0.f}, {}, {10.f, 10.f, 10.f} });
	m_Chica = std::make_shared<AnimModel>(m_Window->GetGraphics(), "res\\models\\chica\\scene.gltf",           Transform{ {- 80.f, 0.f, 0.f}, {}, {.1f, .1f, .1f} });
	m_Springtrap = std::make_shared<AnimModel>(m_Window->GetGraphics(), "res\\models\\springtrap\\scene.gltf", Transform{ {-160.f, 0.f, 0.f}, {}, {.2f, .2f, .2f} });
	m_Mangle     = std::make_shared<AnimModel>(m_Window->GetGraphics(), "res\\models\\mangle\\scene.gltf",     Transform{ {-240.f, 0.f, 0.f}, {}, {20.f, 20.f, 20.f} });
	m_Marionette = std::make_shared<AnimModel>(m_Window->GetGraphics(), "res\\models\\marionette\\scene.gltf", Transform{ {-320.f, 0.f, 0.f}, {}, {20.f, 20.f, 20.f} });

	m_Scene->Submit("Skybox", m_Skybox);
	m_Scene->Submit("Point Light", m_PL);
	m_Scene->Submit("Model", m_GF);
	m_Scene->Submit("Sponza", m_Sponza);

	m_Scene->Submit("Freddy", m_Freddy);
	m_Scene->Submit("Chica", m_Chica);
	m_Scene->Submit("Springtrap", m_Springtrap);
	m_Scene->Submit("Mangle", m_Mangle);
	m_Scene->Submit("Marionette", m_Marionette);

	m_Window->GetGraphics().SetScene(m_Scene);
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
				m_GF->Reload(m_Window->GetGraphics(), Window::OpenDialogBox());

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

		m_Window->GetGraphics().BeginFrame(*m_Camera);
		m_PL->Bind(m_Window->GetGraphics());
		//m_ShadowPass->Run(m_Window->GetGraphics());

		m_Freddy->Animate(m_Window->GetGraphics(), deltaTime);
		m_Marionette->Animate(m_Window->GetGraphics(), deltaTime);
		m_Springtrap->Animate(m_Window->GetGraphics(), deltaTime);
		m_Chica->Animate(m_Window->GetGraphics(), deltaTime);
		m_Mangle->Animate(m_Window->GetGraphics(), deltaTime);
		
		m_Pass->Run(m_Window->GetGraphics());
		ShowGUI();
		m_Window->GetGraphics().EndFrame();
	}

	return *ret;
}
