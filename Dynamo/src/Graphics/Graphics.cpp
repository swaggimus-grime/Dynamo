#include "dynamopch.h"
#include "Graphics.h"
#include <d3d11.h>
#include "Model.h"
#include "Gui.h"
#include "GPU.h"

Graphics::Graphics(HWND hWnd)
{
    m_GPU = std::make_shared<GPU>(hWnd);

    Gui::Init(hWnd, m_GPU);
}

Graphics::~Graphics()
{
    Gui::Shutdown();
}

void Graphics::Render()
{
    XMVECTOR color = { 1.f, 1.f, 0.f, 1.f };
    m_GPU->Clear(color);
    Gui::BeginFrame();


    m_GPU->Present();
    Gui::EndFrame();
}
