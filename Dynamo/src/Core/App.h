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
	std::shared_ptr<class Scene> m_Scene;
	std::shared_ptr<class Camera> m_Camera;
	std::shared_ptr<class Skybox> m_Skybox;
	std::shared_ptr<class Model> m_Sponza;
	std::shared_ptr<class Model> m_GF;
	std::shared_ptr<class PointLight> m_PL;
	//std::unique_ptr<class Shadowpass> m_ShadowPass;
	//std::unique_ptr<class NegativePass> m_NegPass;
	std::unique_ptr<class RenderPass> m_Pass;
	std::shared_ptr<class AnimModel> m_Dragon;
	std::unique_ptr<class Animation> m_Animation;
	std::unique_ptr<class Animator> m_Animator;
};