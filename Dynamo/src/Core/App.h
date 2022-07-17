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
	Unique<Window> m_Window;
	Shared<class Camera> m_Camera;

	Unique<class TestCube> m_Cube;
	Unique<class Skybox> m_Skybox;
};