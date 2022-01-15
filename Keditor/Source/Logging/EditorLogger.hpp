#pragma once

#include "Util/Fmt.hpp"

namespace krakoa
{
	void LogEditorTrace( const std::string_view& msg );
	void LogEditorDebug( const std::string_view& msg );
	void LogEditorInfo( const std::string_view& msg );
	void LogEditorWarn( const std::string_view& msg );
	void LogEditorError( const std::string_view& msg );

	template <typename T, typename ...Ts>
	void LogEditorTrace( const char* fmt, const T& arg, const Ts& ...args )
	{
		LogEditorTrace( util::Fmt( fmt, arg, args... ) );
	}

	template <typename T, typename ...Ts>
	void LogEditorDebug( const char* fmt, const T& arg, const Ts& ...args )
	{
		LogEditorDebug( util::Fmt( fmt, arg, args... ) );
	}

	template <typename T, typename ...Ts>
	void LogEditorInfo( const char* fmt, const T& arg, const Ts& ...args )
	{
		LogEditorInfo( util::Fmt( fmt, arg, args... ) );
	}

	template <typename T, typename ...Ts>
	void LogEditorWarn( const char* fmt, const T& arg, const Ts& ...args )
	{
		LogEditorWarn( util::Fmt( fmt, arg, args... ) );
	}

	template <typename T, typename ...Ts>
	void LogEditorError( const char* fmt, const T& arg, const Ts& ...args )
	{
		LogEditorError( util::Fmt( fmt, arg, args... ) );
	}
}
