#pragma once

#include <memory>

namespace krakoa
{
	template<typename  T>
	using Multi_Ptr = std::shared_ptr<T>;
	
	template<typename  T>
	using Weak_Ptr = std::weak_ptr<T>;
	
	template<typename  T>
	using Single_Ptr = std::unique_ptr<T>;

	template<typename T, typename ...Ts>
	Single_Ptr<T> Make_Unique(Ts&& ...params)
	{
		return std::make_unique<T>(std::forward<Ts>(params)...);
	}

	template<typename T, typename ...Ts>
	Multi_Ptr<T> Make_Shared(Ts&& ...params)
	{
		return std::make_shared<T>(std::forward<Ts>(params)...);
	}

	
}
