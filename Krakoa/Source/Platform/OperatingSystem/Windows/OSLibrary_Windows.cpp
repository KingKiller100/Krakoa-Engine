﻿#include "Precompile.hpp"
#include "OSLibrary_Windows.hpp"

#if defined(_WIN32) || defined(KRAKOA_OS_WINDOWS)

namespace krakoa::os::library
{
	OSLibrary_Windows::OSLibrary_Windows( std::string_view name, HMODULE libModule )
		: instance( libModule )
		, name( name )
	{}

	OSLibrary_Windows::~OSLibrary_Windows()
	= default;

	bool OSLibrary_Windows::IsLoaded() const
	{
		return instance != nullptr && instance != INVALID_HANDLE_VALUE;
	}

	void* OSLibrary_Windows::ImportImpl( const char* funcName )
	{
		if ( auto iter = imports.find( funcName ); iter != imports.end() )
		{
			return iter->second;
		}

		const auto func = ::GetProcAddress( instance, funcName );
		imports.emplace( funcName, func );
		return func;
	}

	std::vector<std::string> OSLibrary_Windows::GetImportsID() const
	{
		std::vector<std::string> list(imports.size());

		for (const auto& [id, _] : imports)
		{
			list.emplace_back( id );
		}

		return list;
	}

	HINSTANCE OSLibrary_Windows::GetHandle() const noexcept
	{
		return instance;
	}

	std::string_view OSLibrary_Windows::GetName() const
	{
		return name;
	}
}

#endif
