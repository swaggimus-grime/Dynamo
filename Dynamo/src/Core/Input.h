#pragma once

#include "Debug/DynamoException.h"
#include <bitset>

class Input {
public:
	Input();
	bool IsPressed(unsigned int key);
	bool IsReleased(unsigned int key);
	std::optional<XMFLOAT2> ReadMouseDelta();
	std::optional<UINT> ReadKey();

	inline void SetCursor(BOOL enabled) { SetRawDeltaEnabled(enabled); ShowCursor(!enabled); }
	inline void SetRawDeltaEnabled(BOOL enabled) { m_RawDeltaEnabled = enabled; }
	inline BOOL RawDeltaEnabled() const { return m_RawDeltaEnabled; }
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
	std::queue<UINT> m_KeyBuffer;

	XMFLOAT2 m_MousePos;
	XMFLOAT2 m_MouseDelta;
	std::queue<XMFLOAT2> m_MouseDeltas;
	BOOL m_RawDeltaEnabled = true;
};

#define INPUT_EXCEP(msg) Input::InputException(__FILE__, __LINE__, msg)