#pragma once

#include "Channel.h"
#include "Bindable/Bindable.h"

class Out;

class In : public Channel {
public:
	inline const std::string& PassName() const { return m_PassName; }
	inline const std::string& OutName() const { return m_OutName; }
	void Target(const std::string& pass, const std::string& out);
	virtual void Link(Out& out) = 0;
	virtual void ValidateLink() const;
protected:
	In(const std::string& name);
	bool m_Linked = false;
private:
	std::string m_PassName;
	std::string m_OutName;
};

template<class T>
class BufferIn : public In {
public:
	static Unique<BufferIn> Make(const std::string& registeredName, Shared<T>& target)
	{
		static_assert(std::is_base_of_v<Buffer, T>, "In target type must be a Buffer type");
		return MakeUnique<BufferIn>(std::move(registeredName), target);
	}

	BufferIn(std::string registeredName, std::shared_ptr<T>& bind)
		:In(std::move(registeredName)), m_Buffer(bind)
	{
	}

	virtual void Link(Out& out) override
	{
		auto p = std::dynamic_pointer_cast<T>(out.GetBuffer());
		if (!p) {
			std::ostringstream oss;
			oss << "Binding input [" << Name() << "] to output [" << PassName() << "." << OutName() << "] "
				<< " { " << typeid(T).name() << " } not compatible with { " << typeid(out.GetBuffer().get()).name() << " }";
			throw DYNAMO_EXCEP(oss.str());
		}

		m_Buffer = std::move(p);
		m_Linked = true;
	}

private:
	Shared<T>& m_Buffer;
};

template<class T>
class BindableIn : public In {
public:
	static Unique<BindableIn> Make(const std::string& registeredName, Shared<T>& target)
	{
		static_assert(std::is_base_of_v<Bindable, T>, "In target type must be a Bindable type");
		return MakeUnique<BindableIn>(std::move(registeredName), target);
	}

	BindableIn(std::string registeredName, std::shared_ptr<T>& bind)
		:In(std::move(registeredName)), m_Bindable(bind)
	{
	}

	virtual void Link(Out& out) override
	{
		auto p = std::dynamic_pointer_cast<T>(out.GetBindable());
		if (!p) {
			std::ostringstream oss;
			oss << "Binding input [" << Name() << "] to output [" << PassName() << "." << OutName() << "] "
				<< " { " << typeid(T).name() << " } not compatible with { " << typeid(out.GetBindable().get()).name() << " }";
			throw DYNAMO_EXCEP(oss.str());
		}

		m_Bindable = std::move(p);
		m_Linked = true;
	}

private:
	Shared<T>& m_Bindable;
};

template<class T>
class VectorBindableIn : public In
{
	static_assert(std::is_base_of_v<Bindable, T>, "BindableIn target type must be a Bindable type");
public:
	void Link(Out& out) override
	{
		auto p = std::dynamic_pointer_cast<T>(out.GetBindable());
		if (!p) {
			std::ostringstream oss;
			oss << "Binding input [" << GetRegisteredName() << "] to output [" << GetPassName() << "." << GetOutputName() << "] "
				<< " { " << typeid(T).name() << " } does not match { " << typeid(*out.GetBindable().get()).name() << " }";
			throw DYNAMO_EXCEP(oss.str());
		}

		m_Vector[m_Idx] = std::move(p);
		m_Linked = true;
	}

	VectorBindableIn(const std::string& name, std::vector<std::shared_ptr<Bindable>>& vec, size_t idx)
		:In(std::move(name)), m_Vector(vec), m_Idx(idx)
	{
	}

private:
	std::vector<Shared<Bindable>>& m_Vector;
	size_t m_Idx;
};