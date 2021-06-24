#pragma once
#include <HelperMacros.hpp>
#include <Utility/Platform/kPlatform.hpp>

#include <cstdint>
#include <string>

namespace krakoa::os
{
	namespace library {
		class LibraryStore;
	}

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

	class iVersionLoader
	{
	public:
		virtual ~iVersionLoader() = default;
		USE_RESULT virtual VersionInfo GetVersionInfo(library::LibraryStore& libStore) const = 0;
	};
}
