#pragma once

#include <d3d11.h>
#include "Camera.h"
#include "Framebuffer.h"

class Graphics {
public:
	friend class Window;
	Graphics(HWND hWnd, UINT width, UINT height);
	Graphics(const Graphics&) = delete;
	Graphics& operator=(const Graphics&) = delete;
	~Graphics();

	inline IDXGISwapChain& SC() const { return *m_SC.Get(); }
	inline ID3D11Device& Device() const { return *m_Device.Get(); }
	inline ID3D11DeviceContext& DC() const { return *m_DC.Get(); }
	inline XMMATRIX LookAt() const { return m_LookAt; }
	inline XMMATRIX Projection() const { return m_Projection; }
	void BeginFrame(Camera& cam);
	void EndFrame();

	inline void AddFramebuffer(std::shared_ptr<Framebuffer> fb) { m_FBs.push_back(std::move(fb)); }
private:
	void OnWindowResize(UINT width, UINT height);

private:
	ComPtr<ID3D11Device> m_Device;
	ComPtr<ID3D11DeviceContext> m_DC;

	ComPtr<IDXGISwapChain> m_SC;
	std::vector<std::shared_ptr<Framebuffer>> m_FBs;

	XMMATRIX m_LookAt;
	XMMATRIX m_Projection;

};