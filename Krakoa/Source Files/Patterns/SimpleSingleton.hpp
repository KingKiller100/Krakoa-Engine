#pragma once
#include "../Core/EngineCore.hpp"

namespace pattern
{
	template <class T>
	class KRAKOA_API SimpleSingleton
	{
	public:
		SimpleSingleton() {}

		SimpleSingleton(const SimpleSingleton&) = delete;
		SimpleSingleton& operator=(const SimpleSingleton&) = delete;

		static inline T& Reference()
		{
			return *instance;
		}

		static inline T* Pointer() noexcept
		{
			return instance;
		}

		template<class ThisOrChild>
		static inline void Create()
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

