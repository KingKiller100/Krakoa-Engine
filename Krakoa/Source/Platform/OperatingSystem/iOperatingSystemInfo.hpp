#pragma once

#include <HelperMacros.hpp>
#include "../Library/LibraryStore.hpp"

#include <cstdint>


namespace krakoa::os
{
	struct VersionInfo
	{
		std::string systemName;
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
