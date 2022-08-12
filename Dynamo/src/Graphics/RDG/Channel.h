#pragma once

class Channel {
public:
	inline const std::string& Name() const { return m_Name; }
	inline INT PinID() const { return m_PinID; }
	virtual ~Channel() = default;

protected:
	Channel(const std::string& name)
		:m_Name(std::move(name))
	{
		m_PinID = m_NextPinID++;
		Validate(m_Name);
	}

	virtual void Validate(const std::string& name) const
	{
		DYNAMO_ASSERT(!name.empty(), "Empty output name");
		DYNAMO_ASSERT(!std::isdigit(name.front()), "Invalid output name: " + name);
	}

private:
	std::string m_Name;
	INT m_PinID;
	static INT m_NextPinID;
};
