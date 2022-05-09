#pragma once

#include "Debug/DynamoException.h"
#include <bitset>

class Input {
public:
	Input();
	bool IsPressed(unsigned int key);
	bool IsReleased(unsigned int key);
	std::optional<XMFLOAT2> ReadMouseDelta();
private:
	friend class Window;
	void OnKeyPressed(unsigned int key);
	void OnKeyReleased(unsigned int key);
	void OnMouseMoved(float x, float y);
	void OnMouseDelta(float dx, float dy);
	void OnWheelDelta(float dx, float dy);
private:
	class InputException : public DynamoException {
	public:
		InputException(const char* file, unsigned int line, const char* msg);
		virtual const char* GetType() const override;
		virtual const char* what() const override;
	};
private:
	static constexpr UINT numKeys = 256;
	std::bitset<numKeys> m_Keys;

	XMFLOAT2 m_MousePos;
	XMFLOAT2 m_MouseDelta;
	std::queue<XMFLOAT2> m_MouseDeltas;
};

#define INPUT_EXCEP(msg) Input::InputException(__FILE__, __LINE__, msg)