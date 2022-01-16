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
		void Update() override;
		void UpdateCode();
		[[nodiscard]] std::string TranslateErrorCode( DWORD code ) const;

		void SetEmergencyExitFunc(std::function<EmergencyExitFunc> func) override;

	private:
		DWORD errorCode_;
		std::function<EmergencyExitFunc> onExitEvent_;
	};
}

#endif

