#include "Precompile.hpp"
#include "iOperatingSystemInfo.hpp"

#include "Windows/OperatingSystemInfoWindows.hpp"
#include <Utility/Platform/kPlatform.hpp>

namespace krakoa::os
{
	iOperatingSystemInfo* CreateOperatingSystemInfo()
	{
		switch (klib::GetPlatform())
		{
		case klib::PlatformOS::WINDOWS_32:
		case klib::PlatformOS::WINDOWS_64:
		case klib::PlatformOS::WINDOWS:
			return new OperatingSystemInfoWindows();

			default:
		return nullptr;
		}
	}
}
