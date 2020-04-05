#pragma once
#include "../Core/EngineCore.hpp"

namespace pattern
{
	template <class T>
	class KRAKOA_API SimpleSingleton
	{
	public:
		constexpr SimpleSingleton() noexcept
		{}

		SimpleSingleton(const SimpleSingleton&) = delete;
		SimpleSingleton& operator=(const SimpleSingleton&) = delete;

		constexpr static inline T& Reference()
		{
			return *instance;
		}

		constexpr static inline T* Pointer()
		{
			return instance;
		}

		template<class ThisOrChild>
		constexpr static inline void Create()
		{
			if (!instance)
				instance = new ThisOrChild(Token());
		}

	protected:
		struct Token {};

	protected:
		static T* instance;
	};
}

