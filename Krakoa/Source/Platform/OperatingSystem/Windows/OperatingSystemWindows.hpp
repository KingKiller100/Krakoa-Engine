#pragma once

#include "../iOperatingSystem.hpp"

#if defined(_WIN32) || defined(KRAKOA_OS_WINDOWS)

namespace krakoa::os
{
	class OperatingSystemWindows final : public iOperatingSystem
	{
	public:
		OperatingSystemWindows(Token&&);
		~OperatingSystemWindows() override;

		void Initialize() override;
		void Shutdown() override;
		void ConnectWindow( std::shared_ptr<iWindow> window ) override;
		[[nodiscard]] const VersionInfo& GetVersionInfo() const noexcept override;
		[[nodiscard]] library::LibraryStore& GetLibraryStore() noexcept override;
		[[nodiscard]] errors::iErrorHandler& GetErrorHandler() noexcept override;
		[[nodiscard]] iFileDialog& GetFileDialog() noexcept override;
		[[nodiscard]] iEnvironmentVariables<char>& GetEnvironmentVariables() noexcept override;

		[[nodiscard]] const library::LibraryStore& GetLibraryStore() const noexcept override;
		[[nodiscard]] const errors::iErrorHandler& GetErrorHandler() const noexcept override;
		[[nodiscard]] const iFileDialog& GetFileDialog() const noexcept override;
		[[nodiscard]] const iEnvironmentVariables<char>& GetEnvironmentVariables() const noexcept override;
	private:
		VersionInfo versionInfo;
		Solo_Ptr<library::LibraryStore> libStore;
		Solo_Ptr<errors::iErrorHandler> errorHandler;
		Solo_Ptr<iFileDialog> fileDialog;
		Solo_Ptr<iEnvironmentVariables<char>> environmentVariables;
	};
}

#endif