#pragma once

#include <wrl.h>
#include <d3d11.h>

namespace Gui {
	void Init();
	void ActivateMouse(BOOL active);
	void BeginFrame();
	void EndFrame();
	void Shutdown();
};