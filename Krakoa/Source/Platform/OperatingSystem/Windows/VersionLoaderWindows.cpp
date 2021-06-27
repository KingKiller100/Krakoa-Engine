#include "Precompile.hpp"

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
	VersionInfo VersionLoaderWindows::GetVersionInfo(library::LibraryStore& libStore) const
	{
		static constexpr const char* productTypeStrings[]
			= { "Work Station", "Domain Controller", "Server" };

		static constexpr const char* platformTypeStrings[]
			= { "Win32s", "Win32_Windows", "Win32_NT" };

		const auto rtlGetVersionFunc
			= libStore.LoadFunc<NTSTATUS(WINAPI)(LPOSVERSIONINFOEXW)>("ntdll.dll", "RtlGetVersion");

		VersionInfo vi{"Windows", klib::PlatformOS::WINDOWS, "", ""
			, 0, 0, 0};

		if (rtlGetVersionFunc != nullptr)
		{
			RTL_OSVERSIONINFOEXW osVersionInfo;
			ZeroMemory(&osVersionInfo, sizeof(osVersionInfo));
			osVersionInfo.dwOSVersionInfoSize = sizeof(osVersionInfo);
			if (rtlGetVersionFunc(&osVersionInfo) != 0) // 0 denotes success?
			{
				LogOSError("Unable to retrieve version info from system: ");
			}
			else
			{
				vi.platformID = platformTypeStrings[osVersionInfo.dwPlatformId];
				vi.productType = productTypeStrings[osVersionInfo.wProductType - 1];

				vi.major = osVersionInfo.dwMajorVersion;
				vi.minor = osVersionInfo.dwMinorVersion;
				vi.buildNo = osVersionInfo.dwBuildNumber;
			}

			KRK_DBG("Version source function: RtlGetVersionFunc");
		}
		else
		{
			OSVERSIONINFOEX os_version_info;
			ZeroMemory(&os_version_info, sizeof(os_version_info));
			os_version_info.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);

			if (::GetVersionEx((OSVERSIONINFO*)&os_version_info) != 0)
			{
				LogOSError("Unable to retrieve version info from system");
			}
			else
			{
				vi.platformID = platformTypeStrings[os_version_info.dwPlatformId];
				vi.productType = IsWindowsServer() ? "Server" : "Work Station";

				vi.major = os_version_info.dwMajorVersion;
				vi.minor = os_version_info.dwMinorVersion;
				vi.buildNo = os_version_info.dwBuildNumber;
			}

			KRK_DBG("Version source function: GetVersionEx");
		}

		return vi;
	}
}
#	pragma warning(pop)

#endif

