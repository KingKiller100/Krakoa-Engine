#pragma once

#include "../iOperatingSystem.hpp"

namespace krakoa::os
{
	class OperatingSystemWindows final : public iOperatingSystem
	{
	public:
		OperatingSystemWindows(Token&&);
		~OperatingSystemWindows() override;

		void Initialize() override;
		void Shutdown() override;
		[[nodiscard]] const VersionInfo& GetVersionInfo() const noexcept override;
		[[nodiscard]] library::LibraryStore& GetLibraryStore() noexcept override;
		[[nodiscard]] errors::iErrorHandler& GetErrorHandler() noexcept override;
		[[nodiscard]] iFileDialog& GetFileDialog() noexcept override;

	private:
		void LoadVersionInfo();

	private:
		VersionInfo versionInfo;
		Solo_Ptr<library::LibraryStore> libStore;
		Solo_Ptr<errors::iErrorHandler> errorHandler;
		Solo_Ptr<iFileDialog> fileDialog;
	};
}
