#include "Precompile.hpp"
#include "OperatingSystemWindows.hpp"

#include "../LogOS.hpp"
#include "../Library/Windows/LibraryInstance_Windows.hpp"
#include "../ErrorHandler/Windows/ErrorHandlerWindows.hpp"
#include "../FileDialog/Windows/FileDialogWindows.hpp"

#include <bcrypt.h>
#include <Windows.h>
#include <VersionHelpers.h>

namespace krakoa::os
{
	OperatingSystemWindows::OperatingSystemWindows(Token&&)
		: versionInfo({ "Windows", klib::PlatformOS::WINDOWS, "", "", 0, 0, 0 })
		, libStore(nullptr)
		, errorHandler(nullptr)
	{
	}

	OperatingSystemWindows::~OperatingSystemWindows()
		= default;

	void OperatingSystemWindows::Initialize()
	{
		errorHandler.reset(new errors::ErrorHandlerWindows());
		
		libStore.reset(new library::LibraryStore([](const char* libName) -> library::iLibraryInstance*
		{
			return new library::LibraryInstance_Windows(libName);
		}));

		fileDialog.reset(new FileDialogWindows());
		
		LoadVersionInfo();
	}

	void OperatingSystemWindows::Shutdown()
	{
		libStore->UnloadAll();
	}

	void OperatingSystemWindows::LoadVersionInfo()
	{
		static constexpr const char* productTypeStrings[]
			= { "Work Station", "Domain Controller", "Server" };

		static constexpr const char* platformTypeStrings[]
			= { "Win32s", "Win32_Windows", "Win32_NT" };

		const auto rtlGetVersionFunc
			= libStore->LoadFunc<NTSTATUS(WINAPI)(LPOSVERSIONINFOEXW)>("ntdll.dll", "RtlGetVersion");

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
				versionInfo.platformID = platformTypeStrings[osVersionInfo.dwPlatformId];
				versionInfo.productType = productTypeStrings[osVersionInfo.wProductType - 1];

				versionInfo.major = osVersionInfo.dwMajorVersion;
				versionInfo.minor = osVersionInfo.dwMinorVersion;
				versionInfo.buildNo = osVersionInfo.dwBuildNumber;
			}

			KRK_DBG("Source function: RtlGetVersionFunc");
		}
		else
		{
			::OSVERSIONINFOEX os_version_info;
			ZeroMemory(&os_version_info, sizeof(os_version_info));
			os_version_info.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);

			if (::GetVersionEx((OSVERSIONINFO*)&os_version_info) != 0)
			{
				LogOSError("Unable to retrieve version info from system");
			}
			else
			{
				versionInfo.platformID = platformTypeStrings[os_version_info.dwPlatformId];
				versionInfo.productType = IsWindowsServer() ? "Server" : "Work Station";

				versionInfo.major = os_version_info.dwMajorVersion;
				versionInfo.minor = os_version_info.dwMinorVersion;
				versionInfo.buildNo = os_version_info.dwBuildNumber;
			}

			KRK_DBG("Source function: GetVersionEx");
		}
	}

	const VersionInfo& OperatingSystemWindows::GetVersionInfo() const noexcept
	{
		return versionInfo;
	}

	library::LibraryStore& OperatingSystemWindows::GetLibraryStore() noexcept
	{
		return *libStore;
	}

	errors::iErrorHandler& OperatingSystemWindows::GetErrorHandler() noexcept
	{
		return *errorHandler;
	}

	iFileDialog& OperatingSystemWindows::GetFileDialog() noexcept
	{
		return *fileDialog;
	}
}
