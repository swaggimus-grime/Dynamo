#include "dynamopch.h"
#include "Binds.h"

#include "Bindable.h"

//Binds& Binds::GetInstance()
//{
//	static Binds b;
//	return b;
//}
//
//template<class T, typename ...Args>
//inline Shared<T> Binds::Evaluate(Args && ...args)
//{
//	/*if (std::is_base_of_v<Bindable, T>)
//		throw std::exception("Bruh");*/
//		//assert(std::is_base_of_v<Bindable, T>, "Can only evaluate classes derived from Bindable");
//	return GetInstance().Evaluate_<T>(std::forward<Args>(args)...);
//}
//
//template<class T, typename ...Args>
//inline std::shared_ptr<T> Binds::Evaluate_(Args && ...args)
//{
//	const std::string& key = T::CreateHash(std::forward<Args>(args)...);
//	const auto val = m_Binds.find(key);
//	if (val == m_Binds.end()) {
//		auto bind = std::make_shared<T>(std::forward<Args>(args)...);
//		m_Binds[key] = bind;
//		return bind;
//	}
//
//	return std::static_pointer_cast<T>(val->second);
//}

