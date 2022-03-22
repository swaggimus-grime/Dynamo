#include "dynamopch.h"
#include "App.h"

App::App(LPCWSTR name, UINT32 width, UINT32 height)
{
	m_Window = std::make_unique<Window>(name, width, height);
	m_Graphics = std::make_unique<Graphics>(m_Window->GetHandle());
}

App::~App()
{

}

void App::Run()
{
	while (!m_Window->IsClosed()) {
		m_Window->Update();
		m_Graphics->Render();
	}
}
