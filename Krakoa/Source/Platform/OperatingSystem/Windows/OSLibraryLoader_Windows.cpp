#include "Precompile.hpp"

#include "OSLibraryLoader_Windows.hpp"
#include "OSLibrary_Windows.hpp"

namespace krakoa::os::library
{
	OSLibraryLoader_Windows::OSLibraryLoader_Windows()
	= default;

	OSLibraryLoader_Windows::~OSLibraryLoader_Windows()
	= default;

	iOSLibrary* OSLibraryLoader_Windows::Load( const std::string_view& libName ) const
	{
		const auto handle = ::LoadLibraryExA( libName.data(), nullptr, 0 );
		const auto lib = new OSLibrary_Windows( libName, handle );
		return lib;
	}

	bool OSLibraryLoader_Windows::Unload( iOSLibrary* lib ) const
	{
		if ( const auto lib_win32 = dynamic_cast<OSLibrary_Windows*>( lib ) )
		{
			return ::FreeLibrary( lib_win32->GetHandle() ) != 0;
		}
		return false;
	}
}
