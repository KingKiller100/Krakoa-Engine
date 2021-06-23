#pragma once

#include "Library/LibraryStore.hpp"
#include "ErrorHandler/iErrorHandler.hpp"
#include "FileDialog/iFileDialog.hpp"

#include "../../Patterns/SimpleSingleton.hpp"

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
	
	class iOperatingSystem : public patterns::SimpleSingleton<iOperatingSystem>
	{
	public:
		virtual ~iOperatingSystem() = default;

		virtual void Initialize() = 0;
		virtual void Shutdown() = 0;
		USE_RESULT virtual const VersionInfo& GetVersionInfo() const noexcept = 0;
		USE_RESULT virtual library::LibraryStore& GetLibraryStore() noexcept = 0;
		USE_RESULT virtual errors::iErrorHandler& GetErrorHandler() noexcept = 0;
		USE_RESULT virtual iFileDialog& GetFileDialog() noexcept = 0;
	};

	void CreateOperatingSystemInfo();
	void DestroyOperatingSystemInfo();
}
