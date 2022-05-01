#pragma once

#include <wrl.h>
#include <d3d11.h>

class Gui {
public:
	static void Init(HWND hWnd, std::shared_ptr<class GPU> gpu);
	static void BeginFrame();
	static void EndFrame();
	static void Shutdown();
};