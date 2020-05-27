#pragma once

#include "SimpleSingleton.hpp"
#include "MemoryOverrider.hpp"

namespace patterns
{
	template<typename T>
	class MemorySingleton : public SimpleSingleton<T>, protected MemoryOverrider<T>
	{};
}
