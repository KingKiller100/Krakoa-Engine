#pragma once

#include "SimpleSingleton.hpp"

namespace patterns
{
	template<typename T>
	class iManager : public SimpleSingleton<iManager<T>>
	{
	public:
		static void Initialize() { T::Create<T>(); };
		virtual void ShutDown() = 0;
	};
}
