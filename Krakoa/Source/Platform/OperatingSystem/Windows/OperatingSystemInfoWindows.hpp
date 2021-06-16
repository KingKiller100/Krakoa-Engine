#pragma once

#include "../iOperatingSystemInfo.hpp"

namespace krakoa::os
{
	class OperatingSystemInfoWindows final : public iOperatingSystemInfo
	{
	public:
		OperatingSystemInfoWindows();
		~OperatingSystemInfoWindows();

		void Initialize() override;
		void Shutdown() override;
		[[nodiscard]] const VersionInfo& GetVersionInfo() const noexcept override;
		[[nodiscard]] library::LibraryStore& GetLibraryStore() noexcept override;

	private:
		void LoadVersionInfo();
		
	private:
		VersionInfo versionInfo;
		Solo_Ptr<library::LibraryStore> libStore;
	};
}
