#pragma once

#include <memory>

namespace Pattern
{
	template<class T>
	class KLIB_API SimpleSingleton
	{
	public:
		virtual ~SimpleSingleton(void) = default;

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

		template<typename ThisOrChildType>
		static void Create()
		{
			instance = std::unique_ptr<T>(new ThisOrChildType());
		}

	protected:
		SimpleSingleton()
			= default;

	private:
		static std::unique_ptr<T> instance;
	};
}

template<class T>
std::unique_ptr<T> KLIB_API Pattern::SimpleSingleton<T>::instance;

