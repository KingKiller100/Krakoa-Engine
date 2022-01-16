#include "Precompile.hpp"
#include "ErrorHandlerWindows.hpp"

#include "../LogOS.hpp"
#include "../../../Debug/Debug.hpp"

#include <Windows.h>

#if defined(_WIN32) || defined(KRAKOA_OS_WINDOWS)

namespace krakoa::os::errors
{
	ErrorHandlerWindows::ErrorHandlerWindows()
	= default;

	ErrorHandlerWindows::~ErrorHandlerWindows()
	= default;

	void ErrorHandlerWindows::EmergencyExit()
	{
		LogOSError( "Force termination" );
		LogOSError( "EmergencyExit via \"::TerminateProcess(::GetCurrentProcess(), -1)\"" );
		Update();
		LogOSError( util::Fmt( "#{0}: {1}", GetCode(), GetText() ) );
		onExitEvent_();
		::TerminateProcess( ::GetCurrentProcess(), -1 );
	}

	void ErrorHandlerWindows::Update()
	{
		UpdateCode();
	}

	void ErrorHandlerWindows::UpdateCode()
	{
		errorCode_ = ::GetLastError();
	}

	std::string ErrorHandlerWindows::TranslateErrorCode( const DWORD code ) const
	{
		static constexpr ::DWORD flags = FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM;

		::LPWSTR formattedMessage = nullptr;
		const ::DWORD formatRes = ::FormatMessageW( flags, nullptr, code, 0, ( LPWSTR )&formattedMessage, 0, nullptr );

		if ( formatRes == 0 )
			return "";

		std::string errorText = klib::Convert<char>( formattedMessage );

		::LocalFree( formattedMessage );
		formattedMessage = nullptr;

		errorText = klib::Replace( errorText, '\r', ' ' );
		errorText = klib::Replace( errorText, '\n', ' ' );

		return errorText;
	}

	void ErrorHandlerWindows::SetEmergencyExitFunc( std::function<EmergencyExitFunc> func )
	{
		KRK_ASSERT( func != nullptr, "Termination function cannot be set to null" );
		onExitEvent_ = std::move( func );
	}

	std::uint32_t ErrorHandlerWindows::GetCode() const noexcept
	{
		return errorCode_;
	}

	std::string_view ErrorHandlerWindows::GetText() const noexcept
	{
		return TranslateErrorCode( GetCode() );
	}
}

#endif
