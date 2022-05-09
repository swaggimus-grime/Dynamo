#pragma once

#include <wrl.h>
#include <d3d11.h>

class Gui {
public:
	static void Init();
	//static void WinInit(HWND& hWnd);
	//static void GraphicsInit(std::shared_ptr<class GPU> gpu);
	static void BeginFrame();
	static void EndFrame();
	static void Shutdown();
};