#include "Precompile.hpp"
#include "ErrorHandlerWindows.hpp"

#include "Windows.h"
#include "../../../../Logging/EngineLogger.hpp"

namespace krakoa::os::errors
{
	ErrorHandlerWindows::ErrorHandlerWindows()
		: prevErrorCode(0)
		, prevErrorString()
	{
	}

	ErrorHandlerWindows::~ErrorHandlerWindows()
		= default;

	std::uint32_t ErrorHandlerWindows::GetErrorCode() const noexcept
	{
		return prevErrorCode;
	}

	std::string ErrorHandlerWindows::GetErrorString() const noexcept
	{
		return prevErrorString;
	}

	void ErrorHandlerWindows::EmergencyExit()
	{
		KRK_ERR("Terminating application");
		KRK_ERR("EmergencyExit : TerminateProcess");
		::TerminateProcess(::GetCurrentProcess(), 0);
	}

	void ErrorHandlerWindows::UpdateErrorInfo()
	{
		prevErrorCode = ::GetLastError();

		::LPTSTR s;
		::DWORD flags = FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM;
		::DWORD formatRes = ::FormatMessageA(flags, nullptr, prevErrorCode, 0, (LPSTR)&s, 0, NULL);

		if (formatRes != 0)
		{
			prevErrorString = klib::Convert<char>(s);
			::LocalFree(s);
			prevErrorString = klib::Replace(prevErrorString, '\r', ' ');
			prevErrorString = klib::Replace(prevErrorString, '\n', ' ');
		}
		else
		{
			prevErrorString = util::Fmt("Unknown Error : 0x{0:h}", prevErrorCode);
		}
	}
}
