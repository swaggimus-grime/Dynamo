#include "dynamopch.h"
#include "Input.h"

Input::Input()
{
	RAWINPUTDEVICE mouse{};
	mouse.usUsagePage = 1;
	mouse.usUsage = 2;
	if (RegisterRawInputDevices(&mouse, 1, sizeof(mouse)) == FALSE)
		throw INPUT_EXCEP("Failed to register mouse");

	SetCursor(true);
}

void Input::OnKeyPressed(unsigned int key)
{
	m_Keys[key] = true;
	m_KeyBuffer.push(key);
}

void Input::OnKeyReleased(unsigned int key)
{
	m_Keys[key] = false;
}

void Input::OnMouseMoved(float x, float y)
{
	m_MousePos.x = x;
	m_MousePos.y = y;
}

void Input::OnMouseDelta(float dx, float dy)
{
	m_MouseDeltas.emplace(dx, dy);
}

void Input::OnWheelDelta(float dx, float dy)
{

}

bool Input::IsPressed(unsigned int key)
{
	return m_Keys[key] & 1;
}

bool Input::IsReleased(unsigned int key)
{
	return !(m_Keys[key] & 1);
}

std::optional<XMFLOAT2> Input::ReadMouseDelta()
{
	if (m_MouseDeltas.empty())
		return {};

	const auto delta = m_MouseDeltas.front();
	m_MouseDeltas.pop();
	return delta;
}

std::optional<UINT> Input::ReadKey()
{
	if (m_KeyBuffer.empty())
		return {};

	UINT key = m_KeyBuffer.front();
	m_KeyBuffer.pop();
	return key;
}

Input::InputException::InputException(const char* file, unsigned int line, const char* msg)
	:DynamoException(file, line)
{
	std::stringstream s;
	s << __super::what() << std::endl << msg;
	m_What = s.str();
}

const char* Input::InputException::GetType() const
{
	return "Input Exception";
}

const char* Input::InputException::what() const
{
	return m_What.c_str();
}
