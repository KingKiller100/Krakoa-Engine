#include "Precompile.hpp"
#include "LibraryStore.hpp"

#include "../LogOS.hpp"

#include "../../../Debug/Debug.hpp"
#include "../../../Debug/Instrumentor.hpp"

namespace krakoa::os::library
{
	LibraryStore::LibraryStore( iOSLibraryLoader* libraryLoader )
		: libLoader( libraryLoader )
	{}

	LibraryStore::~LibraryStore()
	{
		UnloadAll();
	}

	Multi_Ptr<iOSLibrary> LibraryStore::Request( const char* libName )
	{
		if ( const auto iter = libraries.find( libName );
			iter != libraries.end() )
		{
			return iter->second;
		}

		if ( Load( libName ) )
		{
			return Request( libName );
		}

		return nullptr;
	}

	void LibraryStore::Unload( Multi_Ptr<iOSLibrary> lib )
	{
		KRK_PROFILE_FUNCTION();

		/**
		 * use_count() <= 2 means that "lib" is the last known usage of
		 * this library by the system. This makes it safe to delete from
		 * the list and trigger its unload
		 */
		if ( lib.use_count() <= 2 )
		{
			libraries.erase( lib->GetName().data() );
		}

		lib.reset();
	}

	void LibraryStore::UnloadAll()
	{
		KRK_PROFILE_FUNCTION();

		LogOS( "Unloading all libraries" );

		for ( auto iter = libraries.begin(); iter != libraries.end(); iter = libraries.begin() )
		{
			Unload( iter->second );
		}

		libraries.clear();
	}

	bool LibraryStore::Exists( std::string_view libName )
	{
		KRK_PROFILE_FUNCTION();
		return libraries.find( libName.data() ) != libraries.end();
	}

	size_t LibraryStore::Size() const noexcept
	{
		return libraries.size();
	}

	size_t LibraryStore::Uses( std::string_view libName ) const noexcept
	{
		if ( const auto iter = libraries.find( libName.data() );
			iter != libraries.end() )
		{
			return iter->second.use_count();
		}

		return 0;
	}

	std::vector<LibraryStore::Report> LibraryStore::GetReports() const
	{
		std::vector<Report> reports;
		for ( const auto& [name, lib] : libraries )
		{
			const auto importsIDs = lib->GetImportsID();
			reports.emplace_back( Report{name, importsIDs} );
		}

		return reports;
	}

	bool LibraryStore::Load( std::string_view libName )
	{
		KRK_PROFILE_FUNCTION();

		LogOS( "Loading library: {0}", libName );

		const auto dtor = [this]( iOSLibrary* lib )
		{
			if ( !lib )
				return;

			const auto name = lib->GetName();
			LogOS( "Unloading library: {0}", name );

			if ( !libLoader->Unload( lib ) )
			{
				LogOSError( "{0} failed to unload!", name );
			}
		};

		const auto lib = libLoader->Load( libName );
		const auto loaded = lib->IsLoaded();

		if ( loaded )
		{
			libraries.emplace( libName, Multi_Ptr<iOSLibrary>( lib, dtor ) );
		}
		else
		{
			LogOSError( "Library load failed" );
		}

		return loaded;
	}
}
