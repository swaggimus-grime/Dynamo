#pragma once

#include "Window.h"
#include "Graphics/Graphics.h"

class App {
public:
	App(LPCWSTR name, UINT32 width, UINT32 height);
	~App();
	App(App&) = delete;
	App& operator=(const App&) = delete;

	void Run();
private:
	std::unique_ptr<Window> m_Window;
	std::unique_ptr<Graphics> m_Graphics;
};