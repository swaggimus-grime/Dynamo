#include "dynamopch.h"
#include "App.h"
#include "Graphics/Gui.h"
#include "Graphics/Shader.h"
#include "Graphics/Camera.h"
#include "Graphics/Model.h"
#include <chrono>
#include "Graphics/Framebuffer.h"
#include "Graphics/Buffer.h"
#include "Graphics/Skybox.h"
#include "Graphics/Sampler.h"
#include "Graphics/Renderpass.h"
#include "Graphics/DSState.h"

struct SkyboxTransform {
	XMMATRIX VP;
};

SkyboxTransform skyTransform;

static struct Data2D {
	struct VERTEX {
		float x;
		float y;
	};

	const std::vector<VERTEX> vertices = { {-0.5f, -0.5f }, {0.f, 0.5f}, {0.5f, -0.5f} };
	std::shared_ptr<Shader> flatShader;
	VertexLayout layout;
	std::shared_ptr<VertexBuffer<VERTEX>> vbuff;
	Data2D(Graphics& g) {
		flatShader = std::make_shared<Shader>(g, L"res/shaders/Flatvs.cso", L"res/shaders/Flatps.cso");
		layout.AddAttrib("Pos", DXGI_FORMAT_R32G32_FLOAT);
		vbuff = std::make_shared<VertexBuffer<VERTEX>>(g, vertices, flatShader->GetVSCode(), layout);
	}
} *data2D;

struct Transform {
	XMMATRIX MVP;
	XMMATRIX ModelView;
};

Transform triTransform;

std::shared_ptr<Framebuffer> fb;

App::App(const std::string& name, UINT32 width, UINT32 height)
{
	Gui::Init();
	m_Window = std::make_unique<Window>(name, width, height);

	m_Camera = std::make_shared<Camera>(XMFLOAT3(0.f, 0.f, -10.f));
	m_DSState = std::make_unique<DSState>(m_Window->GetGraphics(), DS_MODE::FIRST);
	fb = std::make_shared<Framebuffer>(m_Window->GetGraphics(), m_Window->GetWidth(), m_Window->GetHeight(), true);
	m_Skybox = std::make_unique<SkyboxPass>(m_Window->GetGraphics());

	m_TriTransform = std::make_unique<ConstantBuffer>(m_Window->GetGraphics(), SHADER_TYPE::VS, sizeof(triTransform), &triTransform);
	m_SkyTransform = std::make_unique<ConstantBuffer>(m_Window->GetGraphics(), SHADER_TYPE::VS, sizeof(skyTransform), &skyTransform);

	D3D11_RASTERIZER_DESC rastDesc = CD3D11_RASTERIZER_DESC(CD3D11_DEFAULT{});
	rastDesc.CullMode = D3D11_CULL_NONE;
	m_Window->GetGraphics().Device().CreateRasterizerState(&rastDesc, &m_Rast);
	m_Window->GetGraphics().DC().RSSetState(m_Rast.Get());
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
		fb->Bind(m_Window->GetGraphics());
		fb->Clear(m_Window->GetGraphics());
		m_Window->GetGraphics().BeginFrame();

		const auto camPos = m_Camera->GetPos();
		skyTransform.VP = XMMatrixTranspose( m_Camera->LookAt() * m_Camera->Projection());
		m_SkyTransform->Update(m_Window->GetGraphics(), sizeof(skyTransform), &skyTransform);
		m_SkyTransform->Bind(m_Window->GetGraphics());
		m_DSState->Bind(m_Window->GetGraphics());
		m_Skybox->Render(m_Window->GetGraphics());

		/*const auto mv = XMMatrixRotationRollPitchYaw(-M_PI / 2.f, M_PI, 0.f) * m_Camera->LookAt();
		triTransform.ModelView = XMMatrixTranspose(mv);
		triTransform.MVP = XMMatrixTranspose(mv * m_Camera->Projection());
		m_TriTransform->Update(m_Window->GetGraphics(), sizeof(triTransform), &triTransform);
		data2D->flatShader->Bind();
		data2D->vbuff->Bind();
		m_GPU->GetDC()->Draw(data2D->vertices.size(), 0);*/

		m_Window->GetGraphics().EndFrame();
	}

	return *ret;
}
