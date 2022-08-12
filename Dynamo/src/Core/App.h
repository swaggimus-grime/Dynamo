#pragma once

#include "Window.h"

#include "RDG/OutlineRDG.h"
#include "Entities/TestCube.h"
#include "Entities/Model.h"
#include "Entities/Light.h"

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
	Window m_Wnd;
	TestCube m_Cube;
	Model m_GF;
	Model m_Sponza;
	PointLight m_Light;
	Camera m_Camera;
	OutlineRDG m_RDG;

};