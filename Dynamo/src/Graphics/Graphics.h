#pragma once

#include <d3d11.h>
#include "GPU.h"
#include "Camera.h"
#include "Renderable.h"

class Graphics {
public:
	Graphics(HWND hWnd);
	Graphics(const Graphics&) = delete;
	Graphics& operator=(const Graphics&) = delete;
	~Graphics();

	inline std::shared_ptr<GPU> GetGPU() { return m_GPU; }
	void BeginFrame(std::shared_ptr<Camera> camera);
	void Render(std::shared_ptr<Renderable> r);
	void DrawTriangle();
	void EndFrame();
private:
	std::shared_ptr<GPU> m_GPU;
	std::shared_ptr<Camera> m_Camera;
};