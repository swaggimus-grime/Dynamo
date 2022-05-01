#include "dynamopch.h"
#include "Gui.h"
#include <imgui_impl_win32.h>
#include <imgui_impl_dx11.h>
#include "GPU.h"

void Gui::Init(HWND hWnd, std::shared_ptr<GPU> gpu)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsDark();
	ImGui_ImplWin32_Init(hWnd);
	ImGui_ImplDX11_Init(gpu->GetDevice(), gpu->GetDC());

}

void Gui::Shutdown()
{
	ImGui::DestroyContext();
}

void Gui::BeginFrame()
{
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
}

void Gui::EndFrame()
{
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}
