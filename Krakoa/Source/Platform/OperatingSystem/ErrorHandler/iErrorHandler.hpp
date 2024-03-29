#pragma once

#include <cstdint>
#include <functional>
#include <string>

namespace krakoa::os::errors
{
	using EmergencyExitFunc = void();
	
	class iErrorHandler
	{
	public:
		virtual ~iErrorHandler() = default;

		[[nodiscard]] virtual std::uint32_t GetCode() const noexcept = 0;
		[[nodiscard]] virtual std::string_view GetText() const noexcept = 0;
		virtual void Update() = 0;
		virtual void EmergencyExit() = 0;
		virtual void SetEmergencyExitFunc(std::function<EmergencyExitFunc> func) = 0;
	};
}
