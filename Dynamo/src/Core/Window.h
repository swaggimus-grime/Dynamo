#pragma once

#include <Windows.h>
#include "Debug/DynamoException.h"
#include "Input.h"
#include "Graphics/Graphics.h"

class Window {
public:
	Window(const std::string& name, unsigned int width, unsigned int height);
	Window() = delete;
	Window(const Window&) = delete;
	~Window();

	std::optional<INT> Update();
	inline HWND GetHandle() const { return m_Handle; }
	inline Input& GetInput() { return m_Input; }
	inline UINT GetWidth() const { return m_Width; }
	inline UINT GetHeight() const { return m_Height; }
	inline Graphics& GetGraphics() const { return *m_Graphics; }
public:
	class WindowException : public DynamoException {
	public:
		WindowException(const char* file, unsigned int line, HRESULT result);
		virtual const char* GetType() const override;
		virtual const char* what() const override;
	};

private:
	static LRESULT CALLBACK SetupMessageProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK MessageProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	LRESULT HandleMessage(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
private:
	HWND m_Handle;
	HINSTANCE m_Inst;
	std::string m_Name;
	Input m_Input;
	UINT m_Width;
	UINT m_Height;
	std::unique_ptr<Graphics> m_Graphics;
};

#define WIN_EXCEP(r) Window::WindowException(__FILE__, __LINE__, r)
#define WIN_PREV_EXCEP Window::WindowException(__FILE__, __LINE__, GetLastError())