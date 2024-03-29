﻿#include "Precompile.hpp"
#include "OperatingSystemWindows.hpp"

#include "../LogOS.hpp"
#include "FileDialogWindows.hpp"
#include "ErrorHandlerWindows.hpp"
#include "VersionLoaderWindows.hpp"
#include "OSLibrary_Windows.hpp"
#include "OSLibraryLoader_Windows.hpp"
#include "EnvironmentVariablesWindows.hpp"

#include <Windows.h>
#include <VersionHelpers.h>


#if defined(_WIN32) || defined(KRAKOA_OS_WINDOWS)

namespace krakoa::os
{
	OperatingSystemWindows::OperatingSystemWindows( Token&& )
		: versionInfo( {"Windows", klib::PlatformOS::WINDOWS, "", "", 0, 0, 0} )
		, libStore( nullptr )
		, errorHandler( nullptr )
	{}

	OperatingSystemWindows::~OperatingSystemWindows()
	{
		errorHandler->Update();
		LogOS( "[{0}]: {1}", errorHandler->GetCode(), errorHandler->GetText() );

		libStore->UnloadAll();
	}

	void OperatingSystemWindows::Initialize()
	{
		errorHandler.reset( new errors::ErrorHandlerWindows() );

		libStore = Make_Solo<library::LibraryStore>( new library::OSLibraryLoader_Windows() );

		fileDialog.reset( new FileDialogWindows() );

		environmentVariables.reset( new EnvironmentVariablesWindows{} );

		const VersionLoaderWindows versionLoader;
		versionInfo = versionLoader.GetVersionInfo( *libStore );
	}

	void OperatingSystemWindows::Shutdown()
	{
		libStore->UnloadAll();
	}

	const VersionInfo& OperatingSystemWindows::GetVersionInfo() const noexcept
	{
		return versionInfo;
	}

	library::LibraryStore& OperatingSystemWindows::GetLibraryStore() noexcept
	{
		return *libStore;
	}

	const library::LibraryStore& OperatingSystemWindows::GetLibraryStore() const noexcept
	{
		return *libStore;
	}

	errors::iErrorHandler& OperatingSystemWindows::GetErrorHandler() noexcept
	{
		return *errorHandler;
	}

	const errors::iErrorHandler& OperatingSystemWindows::GetErrorHandler() const noexcept
	{
		return *errorHandler;
	}

	iFileDialog& OperatingSystemWindows::GetFileDialog() noexcept
	{
		return *fileDialog;
	}

	const iFileDialog& OperatingSystemWindows::GetFileDialog() const noexcept
	{
		return *fileDialog;
	}

	iEnvironmentVariables<char>& OperatingSystemWindows::GetEnvironmentVariables() noexcept
	{
		return *environmentVariables;
	}

	const iEnvironmentVariables<char>& OperatingSystemWindows::GetEnvironmentVariables() const noexcept
	{
		return *environmentVariables;
	}
}

#endif
