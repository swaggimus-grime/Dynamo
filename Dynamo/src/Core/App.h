#pragma once

#include "Window.h"
#include "Graphics/Graphics.h"

class App {
public:
	App(const std::string& name, UINT32 width, UINT32 height);
	~App();
	App(App&) = delete;
	App& operator=(const App&) = delete;
	INT Run();

private:
	void UserInput(float deltaTime);
	void ShowGUI();
private:
	std::unique_ptr<Window> m_Window;
	std::unique_ptr<class Selector> m_Selector;
	std::unique_ptr<class Scene> m_Scene;
	std::shared_ptr<class Camera> m_Camera;
	std::shared_ptr<class Skybox> m_Skybox;
	std::shared_ptr<class Model> m_Sponza;
	std::shared_ptr<class Model> m_GF;
	std::shared_ptr<class PointLight> m_PL;
};