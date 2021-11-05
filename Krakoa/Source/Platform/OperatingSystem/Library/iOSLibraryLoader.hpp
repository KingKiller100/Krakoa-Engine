#pragma once

#include "iOSLibrary.hpp"
#include <string>

namespace krakoa::os::library
{
	class iOSLibraryLoader
	{
	public:
		virtual ~iOSLibraryLoader() = default;

		[[nodiscard]] virtual iOSLibrary* Load(const std::string_view& libName) const;
		void Unload(iOSLibrary* lib) const;
	};
}
