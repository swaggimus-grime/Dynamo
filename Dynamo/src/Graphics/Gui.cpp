#include "dynamopch.h"
#include "Gui.h"
#include <imgui_impl_win32.h>
#include <imgui_impl_dx11.h>
#include "GPU.h"

void Gui::Init()
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsDark();
}

//void Gui::WinInit(HWND& hWnd)
//{
//	ImGui_ImplWin32_Init(hWnd);
//}
//
//void Gui::GraphicsInit(std::shared_ptr<GPU> gpu)
//{
//	ImGui_ImplDX11_Init(gpu->GetDevice(), gpu->GetDC());
//}

void Gui::Shutdown()
{
	ImGui::DestroyContext();
}

void Gui::BeginFrame()
{
	ImGui_ImplWin32_NewFrame();
	ImGui_ImplDX11_NewFrame();
	ImGui::NewFrame();
}

void Gui::EndFrame()
{
	ImGui::EndFrame();
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}
