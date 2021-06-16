#pragma once

#include "../iErrorHandler.hpp"

namespace krakoa::os::errors
{
	class ErrorHandlerWindows final : public iErrorHandler
	{
	public:
		ErrorHandlerWindows();
		~ErrorHandlerWindows() override;
		
		[[nodiscard]] std::uint32_t GetErrorCode() const noexcept override;
		[[nodiscard]] std::string GetErrorString() const noexcept override;
		void EmergencyExit() override;
		void UpdateErrorInfo() override;

	private:
		std::uint32_t prevErrorCode;
		std::string prevErrorString;
	};
}
