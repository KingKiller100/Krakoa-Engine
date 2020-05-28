#pragma once
#include "../EngineConfig.hpp"
#include "../Core/Logging/CoreLogger.hpp"

namespace patterns
{
	template <class T>
	class SimpleSingleton
	{
	protected:
		struct Token {};
		
	public:
		constexpr SimpleSingleton() noexcept
			= default;
		virtual ~SimpleSingleton() noexcept = default;

		SimpleSingleton(const SimpleSingleton&) = delete;
		SimpleSingleton& operator=(const SimpleSingleton&) = delete;

		static constexpr T& Reference()
		{
			KRK_FATAL(instance, "Refernce to uninitialized singleton");
			return *instance;
		}

		static constexpr T* Pointer()
		{
			if (!instance)
				return nullptr;

			return instance;
		}

		template<class ThisOrDerivedType = T, typename ...Args, typename = std::enable_if_t<
			std::is_base_of_v<T, ThisOrDerivedType>
			&& std::is_constructible_v<ThisOrDerivedType, Token, Args...>>>
		static constexpr void Create(Args&& ...params)
		{
			if (!instance)
				instance = new ThisOrDerivedType(Token(), std::forward<Args>(params)...);
		}

	protected:
		inline static T* instance = nullptr;
	};
}

