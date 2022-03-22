#include "dynamopch.h"
#include "Window.h"
#include "imgui_impl_dx11.h"
#include <comdef.h>

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	if (ImGui_ImplWin32_WndProcHandler(hwnd, uMsg, wParam, lParam))
		return true;
	switch (uMsg) {
	case WM_CLOSE:
		PostQuitMessage(1);
		return 0;
	}

	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

Window::Window(LPCWSTR name, unsigned int width, unsigned int height)
	:m_Inst(GetModuleHandle(nullptr)), m_Closed(false), m_Name(name)
{
	WNDCLASSEXW wc{};
	HINSTANCE hInst = m_Inst;
	wc.cbSize = sizeof(wc);
	wc.style = CS_OWNDC;
	wc.lpfnWndProc = WindowProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInst;
	wc.hIcon = nullptr;
	wc.hCursor = nullptr;
	wc.hbrBackground = nullptr;
	wc.lpszMenuName = nullptr;
	wc.lpszClassName = m_Name;
	RegisterClassExW(&wc);

	RECT wr = { 0, 0, width, height };
	AdjustWindowRect(&wr, WS_MINIMIZEBOX | WS_CAPTION | WS_SYSMENU, FALSE);
	m_Handle = CreateWindowExW(NULL, m_Name, m_Name,
		WS_MINIMIZEBOX | WS_CAPTION | WS_SYSMENU,
		0, 0,
		wr.right - wr.left, wr.bottom - wr.top,
		nullptr, nullptr,
		hInst, nullptr);
	if (!m_Handle)
		throw WIN_PREV_EXCEP;
	ShowWindow(m_Handle, SW_SHOW);
}

Window::~Window()
{
	UnregisterClassW(m_Name, m_Inst);
}

void Window::Update()
{
	static MSG msg;
	if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
		switch (msg.message) {
		case WM_QUIT:
			m_Closed = true;
			break;
		default:
			//translate any virtual key message into WM_CHAR message
			TranslateMessage(&msg);
			//Send the message to the window procedure
			DispatchMessage(&msg);
		}
	}
}

Window::WindowException::WindowException(const char* file, unsigned int line, HRESULT result)
	:DynamoException(file, line)
{
	_com_error err(result);
	std::stringstream s;
	s << __super::what() << std::endl << err.ErrorMessage();
	m_What = s.str();
}

const char* Window::WindowException::GetType() const 
{
	return "Window Exception";
}

const char* Window::WindowException::what() const
{
	return m_What.c_str();
}
