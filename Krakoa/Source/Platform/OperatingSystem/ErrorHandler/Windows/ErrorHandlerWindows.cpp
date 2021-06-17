#include "Precompile.hpp"
#include "ErrorHandlerWindows.hpp"

#include "../../LogOS.hpp"
#include "../../../../Debug/Debug.hpp"

#include <Windows.h>

namespace krakoa::os::errors
{
	ErrorHandlerWindows::ErrorHandlerWindows()
		: errorCode(0)
		, errorText()
	{}

	ErrorHandlerWindows::~ErrorHandlerWindows()
		= default;

	std::uint32_t ErrorHandlerWindows::GetCode() const noexcept
	{
		return errorCode;
	}

	std::string_view ErrorHandlerWindows::GetText() const noexcept
	{
		return errorText;
	}

	void ErrorHandlerWindows::EmergencyExit()
	{
		LogOSError("Force termination");
		LogOSError("EmergencyExit via \"::TerminateProcess(::GetCurrentProcess(), -1)\"");
		CheckForNewError();
		LogOSError(util::Fmt("{0}: {1}", GetCode(), GetText()));
		exitFunc();
		::TerminateProcess(::GetCurrentProcess(), -1);
	}

	void ErrorHandlerWindows::CheckForNewError()
	{
		UpdateCode();
		if (!UpdateText(GetCode()))
		{
			errorText = util::Fmt("Unknown Error : 0x{0:h}", errorCode);
		}
	}

	void ErrorHandlerWindows::UpdateCode()
	{
		errorCode = ::GetLastError();
	}

	bool ErrorHandlerWindows::UpdateText(const DWORD code)
	{
		static constexpr ::DWORD flags = FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM;

		::LPWSTR s = nullptr;
		const ::DWORD formatRes = ::FormatMessageW(flags, nullptr, code, 0, (LPWSTR)&s, 0, nullptr);

		if (formatRes == 0)
			return false;

		errorText = klib::Convert<char>(s);

		::LocalFree(s);
		s = nullptr;

		errorText = klib::Replace(errorText, '\r', ' ');
		errorText = klib::Replace(errorText, '\n', ' ');

		return true;
	}

	void ErrorHandlerWindows::SetEmergencyExitFunc(std::function<EmergencyExitFunc> func)
	{
		KRK_ASSERT(func != nullptr, "Termination function cannot be set to null");
		exitFunc = std::move(func);
	}
}
