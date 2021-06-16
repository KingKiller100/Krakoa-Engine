#pragma once

#include <cstdint>
#include <string>

namespace krakoa::os::errors
{
	class iErrorHandler
	{
	public:
		virtual ~iErrorHandler() = default;

		[[nodiscard]] virtual std::uint32_t GetErrorCode() const noexcept = 0;
		[[nodiscard]] virtual std::string GetErrorString() const noexcept = 0;
		virtual void UpdateErrorInfo() = 0;
		virtual void EmergencyExit() = 0;
	};
}
