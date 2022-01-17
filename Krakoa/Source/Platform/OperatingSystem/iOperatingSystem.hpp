#pragma once

#include "Library/LibraryStore.hpp"
#include "FileDialog/iFileDialog.hpp"
#include "ErrorHandler/iErrorHandler.hpp"
#include "VersionInfo/iVersionInfoLoader.hpp"
#include "EnvironmentVariables/iEnvironmentVariables.hpp"

#include "../../Patterns/SimpleSingleton.hpp"

#include <HelperMacros.hpp>

namespace krakoa
{
	class iWindow;
	
	namespace os
	{
		class iOperatingSystem : public patterns::SimpleSingleton<iOperatingSystem>
		{
		public:
			virtual ~iOperatingSystem() = default;

			virtual void Initialize() = 0;
			virtual void Shutdown() = 0;
			virtual void ConnectWindow( std::shared_ptr<iWindow> window );
			USE_RESULT virtual const VersionInfo& GetVersionInfo() const noexcept = 0;
			USE_RESULT virtual library::LibraryStore& GetLibraryStore() noexcept = 0;
			USE_RESULT virtual const library::LibraryStore& GetLibraryStore() const noexcept = 0;
			USE_RESULT virtual errors::iErrorHandler& GetErrorHandler() noexcept = 0;
			USE_RESULT virtual const errors::iErrorHandler& GetErrorHandler() const noexcept = 0;
			USE_RESULT virtual iFileDialog& GetFileDialog() noexcept = 0;
			USE_RESULT virtual const iFileDialog& GetFileDialog() const noexcept = 0;
			USE_RESULT virtual iEnvironmentVariables<char>& GetEnvironmentVariables() noexcept = 0;
			USE_RESULT virtual const iEnvironmentVariables<char>& GetEnvironmentVariables() const noexcept = 0;
		};

		void CreateOperatingSystemInterface();
		void DestroyOperatingSystemInfo();
	}
}
