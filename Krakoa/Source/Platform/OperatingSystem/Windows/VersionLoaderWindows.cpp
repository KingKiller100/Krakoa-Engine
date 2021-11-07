#include "Precompile.hpp"

#include "../LogOS.hpp"
#include "VersionLoaderWindows.hpp"
#include "../Library/LibraryStore.hpp"

#if defined(_WIN32) || defined(KRAKOA_OS_WINDOWS)

#	pragma warning(push)
#	pragma warning(disable : 4996)

#include <bcrypt.h>
#include <Windows.h>
#include <VersionHelpers.h>

namespace krakoa::os
{
	VersionInfo VersionLoaderWindows::GetVersionInfo( library::LibraryStore& libStore ) const
	{
		static constexpr const char* productTypeStrings[]
			= {"Work Station", "Domain Controller", "Server"};

		static constexpr const char* platformTypeStrings[]
			= {"Win32s", "Win32_Windows", "Win32_NT"};

		const auto ntdll = libStore.Request( "ntdll.dll" );

		const auto rtlGetVersionFunc
			= ntdll->Import<NTSTATUS(WINAPI)( LPOSVERSIONINFOEXW )>( "RtlGetVersion" );

		VersionInfo vi{
			"Windows", klib::PlatformOS::WINDOWS, "", ""
			, 0, 0, 0
		};

		if ( rtlGetVersionFunc != nullptr )
		{
			RTL_OSVERSIONINFOEXW vi_win32;
			ZeroMemory( &vi_win32, sizeof(vi_win32) );
			vi_win32.dwOSVersionInfoSize = sizeof( vi_win32 );
			if ( rtlGetVersionFunc( &vi_win32 ) != 0 ) // 0 denotes success?
			{
				LogOSError( "Unable to retrieve version info from system" );
			}
			else
			{
				vi.platformID = platformTypeStrings[vi_win32.dwPlatformId];
				vi.productType = productTypeStrings[vi_win32.wProductType - 1];

				vi.major = vi_win32.dwMajorVersion;
				vi.minor = vi_win32.dwMinorVersion;
				vi.buildNo = vi_win32.dwBuildNumber;
			}

			KRK_DBG( "Version source function: RtlGetVersionFunc" );
		}
		else
		{
			OSVERSIONINFOEX vi_win32;
			ZeroMemory( &vi_win32, sizeof(vi_win32) );
			vi_win32.dwOSVersionInfoSize = sizeof( OSVERSIONINFOEX );

			if ( ::GetVersionEx( ( OSVERSIONINFO* )&vi_win32 ) != 0 )
			{
				LogOSError( "Unable to retrieve version info from system" );
			}
			else
			{
				vi.platformID = platformTypeStrings[vi_win32.dwPlatformId];
				vi.productType = IsWindowsServer() ? "Server" : "Work Station";

				vi.major = vi_win32.dwMajorVersion;
				vi.minor = vi_win32.dwMinorVersion;
				vi.buildNo = vi_win32.dwBuildNumber;
			}

			KRK_DBG( "Version source function: GetVersionEx" );
		}

		return vi;
	}
}

#	pragma warning(pop)

#endif
