#pragma once

#include <Windows.h>
#include "Debug/DynamoException.h"

class Window {
public:
	Window(LPCWSTR name, unsigned int width, unsigned int height);
	Window() = delete;
	Window(const Window&) = delete;
	~Window();

	void Update();
	inline bool IsClosed() const { return m_Closed; }
	inline HWND GetHandle() const { return m_Handle; }

public:
	class WindowException : public DynamoException {
	public:
		WindowException(const char* file, unsigned int line, HRESULT result);
		virtual const char* GetType() const override;
		virtual const char* what() const override;
	};
private:
	HWND m_Handle;
	HINSTANCE m_Inst;
	bool m_Closed;
	LPCWSTR m_Name;
};

#define WIN_EXCEP(r) Window::WindowException(__FILE__, __LINE__, r)
#define WIN_PREV_EXCEP Window::WindowException(__FILE__, __LINE__, GetLastError())