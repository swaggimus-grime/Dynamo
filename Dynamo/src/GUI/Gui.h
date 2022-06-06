#pragma once

#include <wrl.h>
#include <d3d11.h>

class Gui {
public:
	static void Init();
	static void ActivateMouse(BOOL active);
	static void BeginFrame();
	static void EndFrame();
	static void Shutdown();
};