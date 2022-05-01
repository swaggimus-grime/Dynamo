#pragma once

#include <d3d11.h>

class Graphics {
public:
	Graphics(HWND hWnd);
	Graphics(const Graphics&) = delete;
	Graphics& operator=(const Graphics&) = delete;
	~Graphics();

	void Render();
private:
	std::shared_ptr<class GPU> m_GPU;
};