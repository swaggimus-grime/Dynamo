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

	inline void SetCamera(const Camera& cam) { m_CamPos = cam.GetPos();  m_LookAt = cam.LookAt(); m_Projection = cam.Projection(); }
	inline XMFLOAT3 CamPos() const { return m_CamPos; }
	inline XMMATRIX LookAt() const { return m_LookAt; }
	inline XMMATRIX Projection() const { return m_Projection; }
	inline void SetLookAt(const XMMATRIX& lookAt) { m_LookAt = lookAt; }
	inline void SetProjection(const XMMATRIX& proj) { m_Projection = proj; }

	inline bool& Mipped() { return m_Mipped; }
	inline void SetMipped(bool mipped) { m_Mipped = mipped; }

	inline bool& HWPCF() { return m_HWPCF; }
	inline bool& Bilinear() { return m_Bilinear; }

	void BeginFrame();
	void EndFrame();

	Shared<class RenderTarget> BackBuffer() const { return m_FinalOutput; }

	inline UINT Width() const { return m_Width; }
	inline UINT Height() const { return m_Height; }
	void SubmitRenderTarget(std::shared_ptr<class RenderTarget> r);

	inline UINT MaxSamples() const { return m_MaxSamples; }
	inline UINT QualityLevel() const { return m_QualityLevel; }

private:
	void OnWindowResize(UINT width, UINT height);

private:
	UINT m_Width;
	UINT m_Height;
	bool m_Mipped = false;

	ComPtr<ID3D11Device> m_Device;
	ComPtr<ID3D11DeviceContext> m_DC;
	ComPtr<IDXGISwapChain> m_SC;

	XMFLOAT3 m_CamPos = { 0, 0, 0 };
	XMMATRIX m_LookAt = XMMatrixIdentity();
	XMMATRIX m_Projection = XMMatrixIdentity();

	Shared<RenderTarget> m_FinalOutput;
	std::vector<std::shared_ptr<RenderTarget>> m_Targets;

	UINT m_MaxSamples;
	UINT m_QualityLevel;

	bool m_HWPCF = true;
	bool m_Bilinear = true;
};