#pragma once
#include "../Core/EngineMacros.hpp"
#include "../Debug/Debug.hpp"


namespace patterns
{
	class iSingleton
	{
	public:
		virtual ~iSingleton() = default;
	};
	
	template <class T>
	class SimpleSingleton : public iSingleton
	{
	public:
		using Type = T;
		
	protected:
		struct Token {};
		
	public:
		constexpr SimpleSingleton() noexcept
			= default;

		SimpleSingleton(const SimpleSingleton&) = delete;
		SimpleSingleton& operator=(const SimpleSingleton&) = delete;

		USE_RESULT static constexpr T& Reference()
		{
			KRK_ASSERT(instance != nullptr, 
				"Reference to uninitialized singleton");
			return *instance;
		}

		USE_RESULT static constexpr T* Pointer()
		{
			if (!instance)
				return nullptr;

			return instance;
		}

		template<class ThisOrDerived = T, typename ...Args, typename = std::enable_if_t<
			std::is_base_of_v<T, ThisOrDerived>
			&& std::is_constructible_v<ThisOrDerived, Token, Args...>
		>>
		static constexpr void Create(Args&& ...params)
		{
			if ( !IsActive() )
				instance = new ThisOrDerived(Token(), std::forward<Args>(params)...);
		}

		static constexpr void Destroy() noexcept
		{
			delete instance;
			instance = nullptr;
		}
		
		USE_RESULT static constexpr bool IsActive() noexcept
		{
			return instance != nullptr;
		}

	protected:
		inline static T* instance = nullptr;
	};
}

