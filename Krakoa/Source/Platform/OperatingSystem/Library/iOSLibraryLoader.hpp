#pragma once

#include "iOSLibrary.hpp"
#include <string>

namespace krakoa::os::library
{
	class iOSLibraryLoader
	{
	public:
		virtual ~iOSLibraryLoader() = default;

		[[nodiscard]] virtual iOSLibrary* Load( std::string_view libName ) const = 0;
		virtual bool Unload( iOSLibrary* lib ) const = 0;
	};
}
