﻿#pragma once
#include "../Core/EngineConfig.hpp"

namespace pattern
{
	template <class T>
	class SimpleSingleton
	{
	public:
		constexpr SimpleSingleton() noexcept
		{}

		SimpleSingleton(const SimpleSingleton&) = delete;
		SimpleSingleton& operator=(const SimpleSingleton&) = delete;

		constexpr static T& Reference()
		{
			return *instance;
		}

		constexpr static T* Pointer()
		{
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
		static T* instance;
	};
}

template<class T>
T* pattern::SimpleSingleton<T>::instance = nullptr;
