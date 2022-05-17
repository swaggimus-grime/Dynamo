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
	std::unique_ptr<class Skybox> m_Skybox;
	std::unique_ptr<class Model> m_GF;
};