#pragma once

#include "../ErrorHandler/iErrorHandler.hpp"

#if defined(_WIN32) || defined(KRAKOA_OS_WINDOWS)

#include <Windows.h>

namespace krakoa::os::errors
{
	class ErrorHandlerWindows final : public iErrorHandler
	{
	public:
		ErrorHandlerWindows();
		~ErrorHandlerWindows() override;

		[[nodiscard]] std::uint32_t GetCode() const noexcept override;
		[[nodiscard]] std::string_view GetText() const noexcept override;
		void EmergencyExit() override;
		void CheckForNewError() override;
		void UpdateCode();
		bool UpdateText(DWORD code);

		void SetEmergencyExitFunc(std::function<EmergencyExitFunc> func) override;

	private:
		DWORD errorCode;
		std::string errorText;
		std::function<EmergencyExitFunc> exitFunc;
	};
}

#endif

