#pragma once

#include <d3d11.h>
#include "Entities/Camera.h"

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

	inline void SetCamera(const Camera& cam) { m_LookAt = cam.LookAt(); m_Projection = cam.Projection(); }
	inline XMMATRIX LookAt() const { return m_LookAt; }
	inline XMMATRIX Projection() const { return m_Projection; }

	void BeginFrame();
	void EndFrame();

	Shared<class RenderTarget> BackBuffer() const { return m_FinalOutput; }

	inline UINT Width() const { return m_Width; }
	inline UINT Height() const { return m_Height; }
	void SubmitRenderTarget(std::shared_ptr<class RenderTarget> r);

private:
	void OnWindowResize(UINT width, UINT height);

private:
	UINT m_Width;
	UINT m_Height;

	ComPtr<ID3D11Device> m_Device;
	ComPtr<ID3D11DeviceContext> m_DC;
	ComPtr<IDXGISwapChain> m_SC;

	XMMATRIX m_LookAt;
	XMMATRIX m_Projection;

	Shared<RenderTarget> m_FinalOutput;
	std::vector<std::shared_ptr<RenderTarget>> m_Targets;
};