#pragma once

#include <d3d11.h>
#include "GPU.h"
#include "Camera.h"
#include "Framebuffer.h"

class Graphics {
public:
	Graphics(HWND hWnd, UINT width, UINT height);
	Graphics(const Graphics&) = delete;
	Graphics& operator=(const Graphics&) = delete;
	~Graphics();

	inline IDXGISwapChain& SC() const { return *m_GPU->GetSwapChain(); }
	inline ID3D11Device& Device() const { return *m_GPU->GetDevice(); }
	inline ID3D11DeviceContext& DC() const { return *m_GPU->GetDC(); }
	void BeginFrame();
	void EndFrame();
private:
	std::unique_ptr<GPU> m_GPU;

};