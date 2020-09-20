#pragma once

#include <memory>

namespace krakoa
{
	template<typename  T>
	using Multi_Ptr = std::shared_ptr<T>;

	template<typename  T>
	using Weak_Ptr = std::weak_ptr<T>;

	template<typename  T>
	using Solo_Ptr = std::unique_ptr<T>;

	template<typename T, typename ...Ts,
		class = std::enable_if_t<std::is_constructible_v<T, Ts...>>>
		Solo_Ptr<T> Make_Solo(Ts&& ...params)
	{
		return std::make_unique<T>(std::forward<Ts>(params)...);
	}

	template<typename T, typename ...Ts,
		class = std::enable_if_t<std::is_constructible_v<T, Ts...>>>
		Multi_Ptr<T> Make_Multi(Ts&& ...params)
	{
		return std::make_shared<T>(std::forward<Ts>(params)...);
	}
}
