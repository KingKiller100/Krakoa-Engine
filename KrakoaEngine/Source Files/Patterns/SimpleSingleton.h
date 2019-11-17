#pragma once

#include "../Core/EngineCore.h"

#include <memory>

namespace Pattern
{
	template<class T>
	class SimpleSingleton
	{
	public:
		virtual ~SimpleSingleton(void) {}

		SimpleSingleton(const SimpleSingleton&) = delete;
		SimpleSingleton& operator=(const SimpleSingleton&) = delete;

		static T& Reference()
		{
			return *instance;
		}

		static std::unique_ptr<T>& Pointer()
		{
			return instance;
		}

	protected:
		SimpleSingleton()
		{
			instance = std::make_unique<T>(Token());
		}

	protected:
		struct Token {};

	private:
		static std::unique_ptr<T> instance;
	};
}

template<class T>
std::unique_ptr<T> Pattern::SimpleSingleton<T>::instance;