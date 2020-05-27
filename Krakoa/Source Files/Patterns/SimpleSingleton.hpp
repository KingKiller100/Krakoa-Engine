#pragma once
#include "../Core/EngineConfig.hpp"
#include "../Core/Logging/CoreLogger.hpp"

namespace patterns
{
	template <class T>
	class SimpleSingleton
	{
	public:
		constexpr SimpleSingleton() noexcept
			= default;
		virtual ~SimpleSingleton() = default;

		SimpleSingleton(const SimpleSingleton&) = delete;
		SimpleSingleton& operator=(const SimpleSingleton&) = delete;
		
		constexpr static T& Reference()
		{
			KRK_FATAL(instance, "Refernce to uninitialized singleton")
			return *instance;
		}

		constexpr static T* Pointer()
		{
			if (!instance)
				return nullptr;

			return instance;
		}

		template<class ThisOrChild = T>
		constexpr static void Create()
		{
			if (!instance)
				instance = new ThisOrChild(Token());
		}

	protected:
		struct Token {};

	protected:
		inline static T* instance = nullptr;
	};
}

