#pragma once

#include "Library/LibraryStore.hpp"

#include <HelperMacros.hpp>
#include <Utility/Platform/kPlatform.hpp>
#include <cstdint>


namespace krakoa::os
{
	struct VersionInfo
	{
		std::string systemName;
		klib::PlatformOS platformOS;
		std::string platformID;
		std::string productType;
		std::uint32_t major;
		std::uint32_t minor;
		std::uint32_t buildNo;
	};
	
	class iOperatingSystemInfo
	{
	public:
		virtual ~iOperatingSystemInfo() = default;

		virtual void Initialize() = 0;
		virtual void Shutdown() = 0;
		USE_RESULT virtual VersionInfo GetVersionInfo() const noexcept = 0;
		USE_RESULT virtual library::LibraryStore& GetLibraryStore() noexcept = 0;
	};

	iOperatingSystemInfo* CreateOperatingSystemInfo();
}
