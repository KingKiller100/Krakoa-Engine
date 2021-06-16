#include "Precompile.hpp"
#include "OperatingSystemInfoWindows.hpp"

#include "../../../Logging/EngineLogger.hpp"
#include "../Library/Windows/LibraryInstance_Windows.hpp"

#include <bcrypt.h>
#include <Windows.h>
#include <VersionHelpers.h>

#define QUICK_TEXTIFY(x) #x

namespace krakoa::os
{
	OperatingSystemInfoWindows::OperatingSystemInfoWindows()
		: versionInfo({ "Windows", klib::PlatformOS::WINDOWS, "", "", 0, 0, 0 })
		, libStore(nullptr)
	{
	}

	OperatingSystemInfoWindows::~OperatingSystemInfoWindows()
		= default;

	void OperatingSystemInfoWindows::Initialize()
	{
		KRK_LOG("OS", "Operating System Info");
		KRK_INF(util::Fmt("System: {0}", versionInfo.systemName));

		libStore.reset(new library::LibraryStore([](const char* libName) -> library::iLibraryInstance*
		{
			return new library::LibraryInstance_Windows(libName);
		}));

		LoadVersionInfo();

		KRK_INF(util::Fmt("Version No: {0}.{1}.{2}", versionInfo.major, versionInfo.minor, versionInfo.buildNo));
		KRK_LOG("OS", "Operating System Info Concluded");
	}

	void OperatingSystemInfoWindows::Shutdown()
	{
		libStore->UnloadAll();
	}

	void OperatingSystemInfoWindows::LoadVersionInfo()
	{
		static constexpr const char* productTypeStrings[]
			= { "Work Station", "Domain Controller", "Server" };

		static constexpr const char* platformTypeStrings[]
			= { "Win32s", "Win32_Windows", "Win32_NT" };

		const auto rtlGetVersionFunc
			= libStore->LoadFunc<NTSTATUS(WINAPI)(LPOSVERSIONINFOEXW)>("ntdll.dll", "RtlGetVersion");

		if (rtlGetVersionFunc != nullptr)
		{
			RTL_OSVERSIONINFOEXW osInfo;
			ZeroMemory(&osInfo, sizeof(osInfo));
			osInfo.dwOSVersionInfoSize = sizeof(osInfo);
			if (rtlGetVersionFunc(&osInfo) != 0) // 0 denotes success?
			{
				KRK_ERR("Unable to retrieve version info from system: ");
			}
			else
			{
				versionInfo.platformID = platformTypeStrings[osInfo.dwPlatformId];
				versionInfo.productType = productTypeStrings[osInfo.wProductType];

				versionInfo.major = osInfo.dwMajorVersion;
				versionInfo.minor = osInfo.dwMinorVersion;
				versionInfo.buildNo = osInfo.dwBuildNumber;
			}

			KRK_DBG("Source: "  QUICK_TEXTIFY(RtlGetVersionFunc(&osInfo)));
		}
		else
		{
			::OSVERSIONINFOEX os_version_info;
			ZeroMemory(&os_version_info, sizeof(os_version_info));
			os_version_info.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);

			if (::GetVersionEx((OSVERSIONINFO*)&os_version_info) != 0)
			{
				KRK_ERR("Unable to retrieve version info from system");
			}
			else
			{
				versionInfo.platformID = platformTypeStrings[os_version_info.dwPlatformId];
				versionInfo.productType = IsWindowsServer() ? "Server" : "Work Station";

				versionInfo.major = os_version_info.dwMajorVersion;
				versionInfo.minor = os_version_info.dwMinorVersion;
				versionInfo.buildNo = os_version_info.dwBuildNumber;
			}

			KRK_DBG("Source: "  QUICK_TEXTIFY(GetVersionEx((OSVERSIONINFO*)&os_version_info)));
		}
	}

	VersionInfo OperatingSystemInfoWindows::GetVersionInfo() const noexcept
	{
		return versionInfo;
	}

	library::LibraryStore& OperatingSystemInfoWindows::GetLibraryStore() noexcept
	{
		return *libStore;
	}

}
