#pragma once

#include "Channel.h"
#include "Bindable/DepthStencil.h"
#include "Bindable/RenderTarget.h"
#include "Bindable/Bindable.h"

class Out : public Channel {
public:
	inline virtual Shared<Bindable> GetBindable() { throw DYNAMO_EXCEP("Cannot directly retrieve bindable from Out class"); }
	inline virtual Shared<Buffer> GetBuffer() { throw DYNAMO_EXCEP("Cannot directly retrieve buffer from Out class"); }
	virtual void ValidateLink() const;
protected:
	Out(const std::string& name);

};

template<class T>
class BufferOut : public Out {
public:
	static Unique<BufferOut> Make(const std::string& name, Shared<T>& buffer)
	{
		return MakeUnique<BufferOut>(std::move(name), buffer);
	}

	BufferOut(const std::string& name, Shared<T>& buffer)
		:Out(std::move(name)), m_Buffer(buffer)
	{
	}

	inline virtual Shared<Buffer> GetBuffer() override { 
		DYNAMO_ASSERT(!m_Linked, "Mutable output bound twice: " + Name());
		m_Linked = true;
		return m_Buffer; 
	}

private:
	bool m_Linked = false;
	Shared<T>& m_Buffer;
};

template<class T>
class BindableOut : public Out {
public:
	static Unique<BindableOut> Make(const std::string& name, Shared<T>& bindable)
	{
		return MakeUnique<BindableOut>(std::move(name), bindable);
	}

	BindableOut(const std::string& name, Shared<T>& bindable)
		:Out(std::move(name)), m_Bindable(bindable)
	{
	}

	inline virtual Shared<Bindable> GetBindable() override { return m_Bindable; }

private:
	Shared<T>& m_Bindable;
};