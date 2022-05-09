#pragma once

#include <d3d11.h>

class GPU {
public:
	GPU(HWND& hWnd);
	
	inline void Present() const { m_SC->Present(0, 0); }
	inline void Clear(XMVECTOR color) const { m_DC->ClearRenderTargetView(m_RTV.Get(), color.m128_f32); }
	inline ID3D11Device* GetDevice() const { return m_Device.Get(); }
	inline ID3D11DeviceContext* GetDC() const { return m_DC.Get(); }
	inline void SetTopology(D3D11_PRIMITIVE_TOPOLOGY top) { m_DC->IASetPrimitiveTopology(top); }
private:
	ComPtr<ID3D11Device> m_Device;
	ComPtr<ID3D11DeviceContext> m_DC;
	ComPtr<IDXGISwapChain> m_SC;
	ComPtr<ID3D11RenderTargetView> m_RTV;
};