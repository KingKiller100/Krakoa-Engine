#include "Precompile.hpp"
#include "EnvironmentVariablesWindows.hpp"

#if defined(_WIN32) || defined(KRAKOA_OS_WINDOWS)
#include "../LogOS.hpp"

#include <Utility/String/Tricks/kStringFind.hpp>

#include <Windows.h>

#ifdef GetEnvironmentStrings
#	undef GetEnvironmentStrings
#endif

namespace krakoa::os
{
	EnvironmentVariablesWindows::EnvironmentVariablesWindows()
	{
		ReadInVariables();
	}

	std::basic_string_view<EnvironmentVariablesWindows::CharT> EnvironmentVariablesWindows::GetVariable( const std::basic_string_view<CharT>& varKey ) const
	{
		return variables.at( ResolveKey( varKey ) );
	}

	std::vector<EnvironmentVariablesWindows::StringView> EnvironmentVariablesWindows::GetKeys() const
	{
		std::vector<StringView> keys;

		for ( const auto& [key, _] : variables )
		{
			keys.emplace_back( key );
		}

		return keys;
	}

	void EnvironmentVariablesWindows::ReadInVariables()
	{
		auto* envStr = GetEnvironmentStrings();

		if ( envStr == nullptr )
		{
			static constexpr auto msg = "GetEnvironmentStrings() returned nullptr";
			LogOSError( msg );
			throw std::runtime_error( msg );
		}

		while ( *envStr != '\0' )
		{
			++envStr;
		}

		++envStr;

		while ( *envStr != '\0' )
		{
			if ( *envStr == '=' )
			{}
			else
			{
				const auto eqPos = klib::Find_First_Of( envStr, '=' );
				if ( eqPos != Terminator )
				{
					String key( envStr, eqPos );
					envStr += eqPos + 1;
					String value = envStr;
					envStr += value.size();
					variables.emplace( ResolveKey( key ), value );
				}
			}

			++envStr;
		}
	}

	EnvironmentVariablesWindows::String EnvironmentVariablesWindows::ResolveKey( StringView key ) const
	{
		return klib::ToLower( key );
	}
}

#endif

