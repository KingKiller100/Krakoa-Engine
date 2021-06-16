#include "Precompile.hpp"
#include "iErrorHandler.hpp"

#include "Windows/ErrorHandlerWindows.hpp"
#include <Utility/Platform/kPlatform.hpp>

namespace krakoa::os::errors
{
	iErrorHandler* CreateErrorHandler()
	{
		switch (klib::GetPlatform())
		{
		case klib::PlatformOS::WINDOWS_32:
		case klib::PlatformOS::WINDOWS_64:
		case klib::PlatformOS::WINDOWS:
			return new ErrorHandlerWindows();
			
		default:
			return nullptr;
		}
	}
}
