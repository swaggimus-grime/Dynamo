#pragma once

class Bindable {
public:
	Bindable(const std::string& hash)
		:m_Hash(hash)
	{}

	virtual void Bind(class Graphics& g) = 0;

protected:
	std::string Hash() const { return m_Hash; }
	std::string m_Hash;
};