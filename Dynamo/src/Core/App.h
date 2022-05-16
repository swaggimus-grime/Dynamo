#pragma once

#include "Window.h"
#include "Graphics/Graphics.h"

class App {
public:
	App(const std::string& name, UINT32 width, UINT32 height);
	~App();
	App(App&) = delete;
	App& operator=(const App&) = delete;

	void UserInput(float deltaTime);
	INT Run();
private:
	std::unique_ptr<Window> m_Window;
	std::shared_ptr<class Camera> m_Camera;
	std::unique_ptr<class SkyboxPass> m_Skybox;
	std::unique_ptr<class ConstantBuffer> m_SkyTransform;
	std::unique_ptr<class ConstantBuffer> m_TriTransform;
	std::unique_ptr<class DSState> m_DSState;
	ComPtr<ID3D11RasterizerState> m_Rast;
};