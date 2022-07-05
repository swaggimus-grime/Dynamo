#pragma once

#include <d3d11.h>
#include "Camera.h"
#include "Scene.h"

class Graphics {
public:
	friend class Window;
	Graphics(HWND hWnd, UINT width, UINT height);
	Graphics(const Graphics&) = delete;
	Graphics& operator=(const Graphics&) = delete;
	~Graphics();

	inline Scene& GetScene() const { return *m_Scene; }
	inline void SetScene(std::shared_ptr<Scene> s) { m_Scene = s; }
	inline IDXGISwapChain& SC() const { return *m_SC.Get(); }
	inline ID3D11Device& Device() const { return *m_Device.Get(); }
	inline ID3D11DeviceContext& DC() const { return *m_DC.Get(); }
	inline XMMATRIX LookAt() const { return m_LookAt; }
	inline XMMATRIX Projection() const { return m_Projection; }
	void BeginFrame(Camera& cam);
	void EndFrame();
	class WriteRenderTarget& BackBuffer() const { return *m_FinalOutput; }
	void BindBackBuffer();
	inline void SetCamera(Camera& cam) { m_LookAt = cam.LookAt(); m_Projection = cam.Projection(); }
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
	std::unique_ptr<class WriteRenderTarget> m_FinalOutput;
	std::unique_ptr<class DSView> m_DepthStencil;
	std::shared_ptr<Scene> m_Scene;
	XMMATRIX m_LookAt;
	XMMATRIX m_Projection;
	std::vector<std::shared_ptr<RenderTarget>> m_Targets;
};