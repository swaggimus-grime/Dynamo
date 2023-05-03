#pragma once

#include "Window.h"

#include "RDG/OutlineRDG.h"
#include "Entities/TestPlane.h"
#include "Entities/Model.h"
#include "Animation/AnimModel.h"
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

private:
	Window m_Wnd;
	Model m_Venator;
	Model m_Sus;
	AnimModel m_MC;

	PointLight m_Light;
	Camera m_Camera;
	OutlineRDG m_RDG;

	std::unordered_map<std::string, GUIable*> m_GUIables;
};