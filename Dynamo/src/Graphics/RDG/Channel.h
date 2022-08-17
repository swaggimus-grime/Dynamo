#pragma once

#include "Editor.h"

class Channel {
public:
	inline const std::string& Name() const { return m_Name; }
	inline UINT PinID() const { return m_PinID; }
	virtual void Unlink() = 0;
	virtual ~Channel() = default;

protected:
	Channel(const std::string& name)
		:m_Name(std::move(name))
	{
		m_PinID = Editor::NextPin();
		Validate(m_Name);
	}

	virtual void Validate(const std::string& name) const
	{
		DYNAMO_ASSERT(!name.empty(), "Empty output name");
		DYNAMO_ASSERT(!std::isdigit(name.front()), "Invalid output name: " + name);
	}

private:
	std::string m_Name;
	UINT m_PinID;
};
