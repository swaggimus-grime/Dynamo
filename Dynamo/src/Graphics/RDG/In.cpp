#include "dynamopch.h"
#include "In.h"

#include "Out.h"

In::In(const std::string& name)
	:Channel(name)
{
	bool valid = std::all_of(Name().begin(), Name().end(), [](char c) {return std::isalnum(c) || c == '_'; });
	DYNAMO_ASSERT(valid, "Invalid In name: " + Name());
}

void In::Target(const std::string& pass, const std::string& out)
{
	Validate(pass);
	bool validChars = std::all_of(pass.begin(), pass.end(), [](char c) {
		return std::isalnum(c) || c == '_';
	});
	DYNAMO_ASSERT(pass == "#" || validChars, "Invalid pass name: " + pass);
	m_PassName = pass;

	Validate(out);
	validChars = std::all_of(out.begin(), out.end(), [](char c) {
		return std::isalnum(c) || c == '_';
	});
	DYNAMO_ASSERT(validChars, "Invalid out name: " + out);
	m_OutName = out;
}

void In::ValidateLink() const
{
	DYNAMO_ASSERT(m_Linked, "Unlinked input: " + Name());
}

//template<class T>
//Unique<BufferIn<T>> BufferIn<T>::Make(const std::string& registeredName, Shared<T>& target)
//{
//	DYNAMO_ASSERT(std::is_base_of_v<Buffer, T>, "In target type must be a Buffer type");
//	return MakeUnique<BufferIn>(std::move(registeredName), target);
//}
//
//template<class T>
//void BufferIn<T>::Link(const Out& out)
//{
//	auto p = std::dynamic_pointer_cast<T>(out.GetBuffer());
//	if (!p) {
//		std::ostringstream oss;
//		oss << "Binding input [" << Name() << "] to output [" << PassName() << "." << OutName() << "] "
//			<< " { " << typeid(T).name() << " } not compatible with { " << typeid(out.GetBuffer().get()).name() << " }";
//		throw DYNAMO_EXCEP(oss.str());
//	}
//
//	m_Buffer = std::move(p);
//}
//
//template<class T>
//BufferIn<T>::BufferIn(std::string registeredName, std::shared_ptr<T>& bind)
//	:In(std::move(registeredName)), m_Buffer(bind)
//{
//}
