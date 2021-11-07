#pragma once

#include "../Library/iOSLibraryLoader.hpp"

namespace krakoa::os::library
{
	class OSLibraryLoader_Windows final : public iOSLibraryLoader
	{
	public:
		OSLibraryLoader_Windows();
		~OSLibraryLoader_Windows() override;
		[[nodiscard]] iOSLibrary* Load( const std::string_view& libName ) const override;
		bool Unload( iOSLibrary* lib ) const override;
	};
}
