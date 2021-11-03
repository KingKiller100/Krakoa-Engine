#include "Precompile.hpp"
#include "iOperatingSystem.hpp"

#include "Windows/OperatingSystemWindows.hpp"
#include <Utility/Platform/kPlatform.hpp>

namespace krakoa::os
{
	void CreateOperatingSystemInterface()
	{
		switch ( klib::GetPlatform() )
		{
		case klib::PlatformOS::WINDOWS_32:
		case klib::PlatformOS::WINDOWS_64:
		case klib::PlatformOS::WINDOWS:
#if defined(_WIN32) || defined(KRAKOA_OS_WINDOWS)
			iOperatingSystem::Create<OperatingSystemWindows>();
			break;
#endif

		default:
			break;
		}

		if ( !iOperatingSystem::IsActive() )
		{
			LogOSError( "Cannot initialize OS instance" );
			return;
		}

		auto& os = iOperatingSystem::Reference();
		os.Initialize();

		const auto& [systemName, platformOS, platformID,
				productType, major, minor, buildNo]
			= os.GetVersionInfo();

		LogOS( util::Fmt( "System: {0} {1}.{2}.{3}", systemName, major, minor, buildNo ) );
		LogOS( util::Fmt( "Platform: {0}", platformID ) );
		LogOS( util::Fmt( "Product: {0}", productType ) );
	}

	void DestroyOperatingSystemInfo()
	{
		if ( iOperatingSystem::IsActive() )
			iOperatingSystem::Destroy();
	}
}
