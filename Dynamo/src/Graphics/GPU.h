#pragma once

#include <d3d11.h>

class GPU {
public:
	GPU(HWND& hWnd);
	GPU(const GPU&) = delete;
	GPU& operator=(const GPU&) = delete;

	inline void Present() const { m_SC->Present(0, 0); }
	inline ID3D11Device* GetDevice() const { return m_Device.Get(); }
	inline ID3D11DeviceContext* GetDC() const { return m_DC.Get(); }
	inline void SetTopology(D3D11_PRIMITIVE_TOPOLOGY top) { m_DC->IASetPrimitiveTopology(top); }
	inline void SetViewport(D3D11_VIEWPORT& vp) { m_DC->RSSetViewports(1, &vp); }
	inline IDXGISwapChain* GetSwapChain() const { return m_SC.Get(); }
private:
	ComPtr<ID3D11Device> m_Device;
	ComPtr<ID3D11DeviceContext> m_DC;
	ComPtr<IDXGISwapChain> m_SC;
};