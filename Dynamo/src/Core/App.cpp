#include "dynamopch.h"
#include "App.h"

#include <chrono>
#include "GUI/Gui.h"
#include <thread>

App::App(const std::string& name, UINT32 width, UINT32 height)
	:m_Wnd(name, width, height),
	m_Light(m_Wnd.GetGraphics(), XMFLOAT3(-10.f, 0.f, -5.5f)),
	m_RDG(m_Wnd.GetGraphics()),
	m_Sus(m_Wnd.GetGraphics(), "res/models/amogus/scene.gltf"),
	m_Venator(m_Wnd.GetGraphics(), "res/models/venator/scene.gltf"),
	m_MC(m_Wnd.GetGraphics(), "res/models/mkv/scene.gltf", false),
	m_Camera("Main", XMFLOAT3(0.f, 0.f, 0.f))
{
	m_Venator.LinkToRDG(m_RDG);
	m_Sus.LinkToRDG(m_RDG);
	m_MC.LinkToRDG(m_RDG);
	m_Light.LinkToRDG(m_RDG);

	m_GUIables.insert({ "Point Light", &m_Light });

	m_GUIables.insert({ "Venator", &m_Venator });

	m_GUIables.insert({ "Amogus drip", &m_Sus });
	m_Sus.SetPos(XMFLOAT3(-9.f, -1.25f, 1.f));

	m_GUIables.insert({ "Master Chief", &m_MC });
	m_MC.SetPos(XMFLOAT3(-10.f, -1.3f, 1.5f));
	m_MC.SetScale(XMFLOAT3(1 / 2.f, 1 / 2.f, 1 / 2.f));
	m_MC.SetRot(XMFLOAT3(0, M_PI, 0));

	m_RDG.SetLightCamera(*m_Light.GetCamera());
}

App::~App()
{
}

void App::UserInput(float deltaTime)
{
	auto& input = m_Wnd.GetInput();
	auto& cam = m_Camera;
	if (input.IsPressed('W'))
		cam.Move(XMFLOAT3(0.f, 0.f, deltaTime));
	if (input.IsPressed('S'))
		cam.Move(XMFLOAT3(0.f, 0.f, -deltaTime));
	if (input.IsPressed('A'))
		cam.Move(XMFLOAT3(-deltaTime, 0.f, 0.f));
	if (input.IsPressed('D'))
		cam.Move(XMFLOAT3(deltaTime, 0.f, 0.f));
	if (input.IsPressed('Q'))
		cam.Move(XMFLOAT3(0.f, deltaTime, 0.f));
	if (input.IsPressed('E'))
		cam.Move(XMFLOAT3(0.f, -deltaTime, 0.f));
	if (input.IsPressed(VK_ESCAPE))
		PostQuitMessage(0);
	while (const auto key = input.ReadKey()) {
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

	while (const auto delta = input.ReadMouseDelta())
		cam.Rotate((float)delta->x, (float)delta->y);
}

INT App::Run()
{
	std::optional<INT> ret;
	using namespace std::chrono;
	const auto startTime = steady_clock::now();
	float prevTime = 0.f;
	bool running = true;

	std::thread renderer([&]() {
		auto& g = m_Wnd.GetGraphics();
		D3D11_TEXTURE2D_DESC tempRTDesc;
		tempRTDesc.Width = g.Width();
		tempRTDesc.Height = g.Height();
		tempRTDesc.MipLevels = 1;
		tempRTDesc.ArraySize = 1;
		tempRTDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		tempRTDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		tempRTDesc.MiscFlags = 0;
		tempRTDesc.SampleDesc.Count = 1;
		tempRTDesc.SampleDesc.Quality = 0;
		tempRTDesc.Usage = D3D11_USAGE_DEFAULT;
		tempRTDesc.CPUAccessFlags = 0;

		ComPtr<ID3D11Texture2D> tempTex;
		DX_ASSERT(g.Device().CreateTexture2D(&tempRTDesc, 0, &tempTex));

		ComPtr<ID3D11ShaderResourceView> view;
		D3D11_SHADER_RESOURCE_VIEW_DESC viewDesc;
		SecureZeroMemory(&viewDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
		viewDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		viewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		viewDesc.Texture2D.MostDetailedMip = 0;
		viewDesc.Texture2D.MipLevels = 1;
		DX_ASSERT(g.Device().CreateShaderResourceView(tempTex.Get(), &viewDesc, view.GetAddressOf()));
		while (running) {
			g.BeginFrame();
			m_RDG.ShowGUI(g);
			if (ImGui::BeginMainMenuBar()) {
				if (ImGui::BeginMenu("File")) {
					if (ImGui::MenuItem("Exit"))
						PostQuitMessage(0);

					ImGui::EndMenu();
				}

				ImGui::EndMainMenuBar();
			}
			if (ImGui::Begin("Scene")) {
				auto pos = ImGui::GetCursorScreenPos();

				auto* msaaTexture = m_RDG.RTTextureFrFr();
				
				/*ComPtr<ID3D11Resource> msaa;
				msaaTexture->GetResource(&msaa);
				ComPtr<ID3D11Resource> temp;
				view->GetResource(&temp);*/
				g.DC().ResolveSubresource(tempTex.Get(), 0, msaaTexture, 0, DXGI_FORMAT_R8G8B8A8_UNORM);

				ImGui::GetWindowDrawList()->AddImage(
					(void*)view.Get(), pos, {pos.x + m_Wnd.GetWidth(), pos.y + m_Wnd.GetHeight()},
					ImVec2(0, 0), ImVec2(1, 1));
				/*ImGui::GetWindowDrawList()->AddImage(
					(void*)m_RDG.DSTexture(), pos, { pos.x + m_Wnd.GetWidth(), pos.y + m_Wnd.GetHeight() },
					ImVec2(0, 0), ImVec2(1, 1));*/

				for (auto& p : m_GUIables) {
					if (ImGui::TreeNode(p.first.c_str())) {
						p.second->ShowGUI(g);
						ImGui::TreePop();
						ImGui::Separator();
					}
				}

				m_Light.Bind(g);
				m_RDG.SetCamera(m_Camera);
				m_Venator.Submit();
				m_Sus.Submit();
				m_MC.Submit();
				m_Light.Submit();
				m_RDG.Run(g);
			}
			ImGui::End();

			g.BackBuffer()->BindBuffer(g);
			g.BackBuffer()->Clear(g);
			m_Wnd.GetGraphics().EndFrame();
			m_RDG.Clear();
		}
	});

	while (!(ret = m_Wnd.Update())) {
		float currentTime = duration<float>(steady_clock::now() - startTime).count();
		float deltaTime = currentTime - prevTime;
		prevTime = currentTime;

		UserInput(deltaTime);
		m_MC.Update(deltaTime);
	}
	running = false;
	renderer.join();

	return *ret;
}
