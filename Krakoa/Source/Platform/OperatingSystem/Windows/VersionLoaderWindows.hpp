#pragma once

#include "../VersionInfo/iVersionInfoLoader.hpp"

#if defined(_WIN32) || defined(KRAKOA_OS_WINDOWS)

namespace krakoa::os
{
	class VersionLoaderWindows : public iVersionLoader
	{
	public:
		~VersionLoaderWindows() override = default;
		[[nodiscard]] VersionInfo GetVersionInfo(library::LibraryStore& libStore) const override;
	};
}

#endif

