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
#include "Graphics/Selector.h"

std::shared_ptr<Framebuffer> fb;

Transform modelTransform(XMFLOAT3(0.f, 0.f, 0.f));

App::App(const std::string& name, UINT32 width, UINT32 height)
{
	Gui::Init();
	m_Window = std::make_unique<Window>(name, width, height);

	m_Camera = std::make_shared<Camera>(XMFLOAT3(0.f, 20.f, -100.f));
	fb = std::make_shared<Framebuffer>(m_Window->GetGraphics(), m_Window->GetWidth(), m_Window->GetHeight(), true);
	m_Skybox = std::make_shared<Skybox>(m_Window->GetGraphics(), L"res\\skyboxes\\yokohama");
	m_GF = std::make_shared<Model>(m_Window->GetGraphics(), "res\\models\\golden_freddy\\scene.gltf", XMFLOAT3(0.f, 0.f, 0.f));
	m_Sponza = std::make_shared<Model>(m_Window->GetGraphics(), "res\\models\\sponza\\sponza.glb", XMFLOAT3(0.f, 0.f, 0.f));
	m_PL = std::make_shared<PointLight>(m_Window->GetGraphics(), XMFLOAT3(0.f, 10.f, -10.f), XMFLOAT3(1.f, 1.f, 0.f));
	m_Scene = std::make_unique<Scene>();
	m_Selector = std::make_unique<Selector>(m_Window->GetGraphics());

	m_Scene->Submit(m_Skybox, "Skybox");
	m_Scene->Submit(m_PL, "Point Light");
	m_Scene->Submit(m_GF, "Model");
	m_Scene->Submit(m_Sponza, "Sponza");
	//m_Selector->Select(m_GF);
}

App::~App()
{
	m_Window.release();
	Gui::Shutdown();
}

bool IntersectTriangle(const XMFLOAT3& orig, const XMFLOAT3& dir,
	XMFLOAT3& v0, XMFLOAT3& v1, XMFLOAT3& v2,
	FLOAT* t, FLOAT* u, FLOAT* v)
{
	// Find vectors for two edges sharing vert0
	XMFLOAT3 edge1 = v1 - v0;
	XMFLOAT3 edge2 = v2 - v0;

	// Begin calculating determinant - also used to calculate U parameter
	XMFLOAT3 pvec = cross(dir, edge2);

	// If determinant is near zero, ray lies in plane of triangle
	FLOAT det = dot(edge1, pvec);

	XMFLOAT3 tvec;
	if (det > 0)
	{
		tvec = orig - v0;
	}
	else
	{
		tvec = v0 - orig;
		det = -det;
	}

	if (det < 0.0001f)
		return FALSE;

	// Calculate U parameter and test bounds
	*u = dot(tvec, pvec);
	if (*u < 0.0f || *u > det)
		return FALSE;

	// Prepare to test V parameter
	XMFLOAT3 qvec = cross(tvec, edge1);

	// Calculate V parameter and test bounds
	*v = dot(dir, qvec);
	if (*v < 0.0f || *u + *v > det)
		return FALSE;

	// Calculate t, scale parameters, ray intersects triangle
	*t = dot(edge2, qvec);
	FLOAT fInvDet = 1.0f / det;
	*t *= fInvDet;
	*u *= fInvDet;
	*v *= fInvDet;

	return TRUE;
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
		m_Sponza->Render(m_Window->GetGraphics());
		//m_Selector->Render(m_Window->GetGraphics());
		m_GF->Render(m_Window->GetGraphics());
		m_PL->Render(m_Window->GetGraphics());

		ShowGUI();
		m_Window->GetGraphics().EndFrame();
	}

	return *ret;
}
