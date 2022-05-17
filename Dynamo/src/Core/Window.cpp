#include "dynamopch.h"
#include "Window.h"
#include <imgui_impl_dx11.h>
#include <imgui_impl_win32.h>
#include <comdef.h>

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

Window::Window(const std::string& name, unsigned int width, unsigned int height)
	:m_Inst(GetModuleHandle(nullptr)), m_Name(name), m_Width(width), m_Height(height)
{
	WNDCLASSEX wc{};
	HINSTANCE hInst = m_Inst;
	wc.cbSize = sizeof(wc);
	wc.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = SetupMessageProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInst;
	wc.hIcon = nullptr;
	wc.hCursor = nullptr;
	wc.hbrBackground = nullptr;
	wc.lpszMenuName = nullptr;
	wc.lpszClassName = m_Name.c_str();
	RegisterClassEx(&wc);

	RECT wr = { 0, 0, m_Width, m_Height };
	AdjustWindowRect(&wr, WS_MINIMIZEBOX | WS_CAPTION | WS_SYSMENU, FALSE);
	m_Handle = CreateWindowEx(NULL, m_Name.c_str(), m_Name.c_str(),
		WS_MINIMIZEBOX | WS_CAPTION | WS_SYSMENU,
		0, 0,
		wr.right - wr.left, wr.bottom - wr.top,
		nullptr, nullptr,
		hInst, this);
	if (!m_Handle)
		throw WIN_PREV_EXCEP;
	ImGui_ImplWin32_Init(m_Handle);
	ShowWindow(m_Handle, SW_SHOW);

	m_Graphics = std::make_unique<Graphics>(m_Handle, m_Width, m_Height);
	RECT rect;
	GetClientRect(m_Handle, &rect);
	MapWindowPoints(m_Handle, nullptr, reinterpret_cast<LPPOINT>(&rect), 2);
	ClipCursor(&rect);
}

Window::~Window()
{
	m_Graphics.release();
	UnregisterClass(m_Name.c_str(), m_Inst);
	ImGui_ImplWin32_Shutdown();
	DestroyWindow(m_Handle);
}

 std::optional<INT> Window::Update()
{
	MSG msg;
	if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
		switch (msg.message) {
		case WM_QUIT:
			return static_cast<INT>(msg.wParam);
		default:
			//translate any virtual key message into WM_CHAR message
			TranslateMessage(&msg);
			//Send the message to the window procedure
			DispatchMessage(&msg);
		}
	}

	return {};
}

void Window::ToggleCursor()
{
	static bool showCursor = true;
	showCursor = !showCursor;
	ShowCursor(showCursor);
}

LRESULT Window::SetupMessageProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (msg != WM_NCCREATE)
		return DefWindowProc(hWnd, msg, wParam, lParam);

	const CREATESTRUCTW createInfo = *reinterpret_cast<CREATESTRUCTW*>(lParam);
	Window* window = static_cast<Window*>(createInfo.lpCreateParams);
	SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(window));
	SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(Window::MessageProc));
	return window->HandleMessage(hWnd, msg, wParam, lParam);
}

LRESULT Window::MessageProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	return reinterpret_cast<Window*>(GetWindowLongPtr(hWnd, GWLP_USERDATA))->HandleMessage(hWnd, msg, wParam, lParam);
}

LRESULT Window::HandleMessage(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
		return true;

	const auto& guiIO = ImGui::GetIO();
	switch (msg) {
	case WM_CLOSE:
		PostQuitMessage(0);
		return 0;
	case WM_KEYDOWN:
		if (guiIO.WantCaptureKeyboard)
			break;
		m_Input.OnKeyPressed(static_cast<UINT>(wParam));
		break;
	case WM_KEYUP:
		if (guiIO.WantCaptureKeyboard)
			break;
		m_Input.OnKeyReleased(static_cast<UINT>(wParam));
		break;
	case WM_MOUSEMOVE:
		POINTS mp = MAKEPOINTS(lParam);
		m_Input.OnMouseMoved(mp.x, mp.y);
		break;
	case WM_MOUSEWHEEL:
		POINTS wp = MAKEPOINTS(lParam);
		m_Input.OnWheelDelta(wp.x, wp.y);
		break;
	case WM_INPUT:
		UINT buffSize = 0;
		if (GetRawInputData(reinterpret_cast<HRAWINPUT>(lParam), RID_INPUT, nullptr, &buffSize, sizeof(RAWINPUTHEADER)) < 0)
			break;
		std::string buff;
		buff.resize(buffSize);
		if (GetRawInputData(reinterpret_cast<HRAWINPUT>(lParam), RID_INPUT, buff.data(), &buffSize, sizeof(RAWINPUTHEADER)) != buffSize)
			break;
		auto ri = reinterpret_cast<const RAWINPUT*>(buff.data());
		if (ri->header.dwType == RIM_TYPEMOUSE &&
			(ri->data.mouse.lLastX != 0 || ri->data.mouse.lLastY != 0))
			m_Input.OnMouseDelta(ri->data.mouse.lLastX, ri->data.mouse.lLastY);
		break;
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
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