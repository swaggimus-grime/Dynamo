#include "dynamopch.h"
#include "App.h"

#include <chrono>
#include "GUI/Gui.h"

App::App(const std::string& name, UINT32 width, UINT32 height)
	:m_Wnd(name, width, height),
	m_RDG(m_Wnd.GetGraphics()),
	m_Cube(m_Wnd.GetGraphics(), 4.f),
	m_Light(m_Wnd.GetGraphics()),
	m_GF(m_Wnd.GetGraphics(), "res/models/golden_freddy/scene.gltf"),
	m_Sponza(m_Wnd.GetGraphics(), "res/models/sponza/Sponza.gltf"),
	m_Camera(XMFLOAT3(0.f, 20.f, -100.f))
{
	m_Cube.LinkToRDG(m_RDG);
	m_Cube.SetPos({ -5, -2, -1 });
	m_GUIables.insert({ "Test Cube", &m_Cube });

	m_Sponza.LinkToRDG(m_RDG);
	m_GF.LinkToRDG(m_RDG);
	m_GUIables.insert({ "Golden Freddy", &m_GF });
	m_Light.LinkToRDG(m_RDG);
	m_GUIables.insert({ "Point Light", &m_Light });
}

App::~App()
{
}

void App::UserInput(float deltaTime)
{
	if (m_Wnd.GetInput().IsPressed('W'))
		m_Camera.Move(XMFLOAT3(0.f, 0.f, deltaTime));
	if (m_Wnd.GetInput().IsPressed('S'))
		m_Camera.Move(XMFLOAT3(0.f, 0.f, -deltaTime));
	if (m_Wnd.GetInput().IsPressed('A'))
		m_Camera.Move(XMFLOAT3(-deltaTime, 0.f, 0.f));
	if (m_Wnd.GetInput().IsPressed('D'))
		m_Camera.Move(XMFLOAT3(deltaTime, 0.f, 0.f));
	if (m_Wnd.GetInput().IsPressed('Q'))
		m_Camera.Move(XMFLOAT3(0.f, deltaTime, 0.f));
	if (m_Wnd.GetInput().IsPressed('E'))
		m_Camera.Move(XMFLOAT3(0.f, -deltaTime, 0.f));
	if (m_Wnd.GetInput().IsPressed(VK_ESCAPE))
		PostQuitMessage(0);

	while (const auto key = m_Wnd.GetInput().ReadKey()) {
		switch (*key) {
		case VK_TAB:
			if (m_Wnd.GetInput().RawDeltaEnabled()) {
				m_Wnd.ClipCursor(false);
				m_Wnd.GetInput().SetCursor(false);
			}
			else {
				m_Wnd.ClipCursor(true);
				m_Wnd.GetInput().SetCursor(true);
			}
			break;
		}
	}

	while (const auto delta = m_Wnd.GetInput().ReadMouseDelta())
		m_Camera.Rotate((float)delta->x, (float)delta->y);

	while (const auto mouse = m_Wnd.GetInput().ReadMouseLPress()) {
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
	if (ImGui::BeginMainMenuBar()) {
		if (ImGui::BeginMenu("File")) {
			if (ImGui::MenuItem("Exit"))
				PostQuitMessage(0);

			ImGui::EndMenu();
		}

		ImGui::EndMainMenuBar();
	}

	ImGui::Begin("Scene");
	auto pos = ImGui::GetCursorScreenPos();
	ImGui::GetWindowDrawList()->AddImage(
		(void*)m_RDG.RTTexture(), pos, { pos.x + m_Wnd.GetWidth(), pos.y + m_Wnd.GetHeight() }, 
		ImVec2(0, 0), ImVec2(1, 1));

	for (auto& p : m_GUIables) {
		if (ImGui::TreeNode(p.first.c_str())) {
			p.second->ShowGUI();
			ImGui::TreePop();
			ImGui::Separator();
		}
	}
	
	ImGui::End();

	m_RDG.ShowGUI();
}

INT App::Run()
{
	std::optional<INT> ret;
	using namespace std::chrono;
	const auto startTime = steady_clock::now();
	float prevTime = 0.f;
	while (!(ret = m_Wnd.Update())) {
		float currentTime = duration<float>(steady_clock::now() - startTime).count();
		float deltaTime = currentTime - prevTime;
		prevTime = currentTime;

		UserInput(deltaTime);

		auto& g = m_Wnd.GetGraphics();
		g.BeginFrame();
		m_Light.Bind(g);
		m_RDG.SetCamera(m_Camera);
		m_Sponza.Submit();
		m_Cube.Submit();
		m_GF.Submit();
		m_Light.Submit();
		m_RDG.Run(g);
		ShowGUI();

		g.BackBuffer()->BindBuffer(g);
		g.BackBuffer()->Clear(g);
		m_Wnd.GetGraphics().EndFrame();
		m_RDG.Clear();
	}

	return *ret;
}
