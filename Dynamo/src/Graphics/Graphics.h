#pragma once

#include <d3d11.h>

using Microsoft::WRL::ComPtr;

class Graphics {
public:
	Graphics(HWND hWnd);
	Graphics(const Graphics&) = delete;
	Graphics& operator=(const Graphics&) = delete;
	~Graphics();

	void Render();
private:
	ComPtr<ID3D11Device> m_Device;
	ComPtr<ID3D11DeviceContext> m_DC;
	ComPtr<IDXGISwapChain> m_SC;
	ComPtr<ID3D11RenderTargetView> m_RTV;
};