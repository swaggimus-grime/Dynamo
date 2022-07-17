#pragma once

#include <type_traits>
#include <unordered_map>

class Binds {
public:
	template<class T, typename...Args>
	static Shared<T> Evaluate(Graphics& g, Args&&...args) 
	{
		/*if (std::is_base_of_v<Bindable, T>)
			throw std::exception("Bruh");*/
		//assert(std::is_base_of_v<Bindable, T>, "Can only evaluate classes derived from Bindable");
		return GetInstance().Evaluate_<T>(g, std::forward<Args>(args)...);
	}

private:
	static Binds& GetInstance()
	{
		static Binds b;
		return b;
	}

	template<class T, typename...Args>
	std::shared_ptr<T> Evaluate_(Graphics& g, Args&&...args) 
	{
		const std::string& key = T::CreateHash(std::forward<Args>(args)...);
		const auto val = m_Binds.find(key);
		if (val == m_Binds.end()) {
			auto bind = std::make_shared<T>(g, std::forward<Args>(args)...);
			m_Binds[key] = bind;
			return bind;
		}
		
		return std::static_pointer_cast<T>(val->second);
	}

private:
	std::unordered_map<std::string, Shared<Bindable>> m_Binds;

};