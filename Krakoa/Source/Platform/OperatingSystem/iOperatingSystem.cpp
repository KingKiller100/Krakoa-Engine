#include "Precompile.hpp"
#include "iOperatingSystem.hpp"

#include "Windows/OperatingSystemWindows.hpp"
#include <Utility/Platform/kPlatform.hpp>

namespace krakoa::os
{
	void CreateOperatingSystemInfo()
	{
		switch (klib::GetPlatform())
		{
		case klib::PlatformOS::WINDOWS_32:
		case klib::PlatformOS::WINDOWS_64:
		case klib::PlatformOS::WINDOWS:
			iOperatingSystem::Create<OperatingSystemWindows>();
			break;

		default:
			break;
		}

		if (!iOperatingSystem::IsActive())
			return;

		const auto& [systemName, platformOS, platformID,
			productType, major, minor, buildNo]
			= iOperatingSystem::Reference().GetVersionInfo();

		LogOS(util::Fmt("System: {0} {1}.{2}.{3}", systemName, major, minor, buildNo));
		LogOS(util::Fmt("Platform: {0}", platformID));
		LogOS(util::Fmt("Product: {0}", productType));
	}

	void DestroyOperatingSystemInfo()
	{
		iOperatingSystem::Destroy();
	}
}
