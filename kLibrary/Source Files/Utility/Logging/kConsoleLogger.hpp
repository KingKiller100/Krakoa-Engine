#pragma once

#include "iLogger.hpp"

namespace klib
{
	namespace kLogs
	{
		enum class ConsoleColour : unsigned short
		{
			NAVY_BLUE = 1,
			DARK_GREEN,
			AQUA_BLUE,
			DARK_RED,
			PURPLE,
			MUSTARD_YELLOW,
			LIGHT_GREY,
			GREY,
			DARK_BLUE,
			LIGHT_GREEN,
			LIGHT_BLUE,
			SCARLET_RED,
			VIOLET_PURPLE,
			YELLOW,
			WHITE,
			RED_BG_WHITE_TEXT = 79
		};
		
		class ConsoleLogger final : public iLogger
		{
		public:
			ConsoleLogger(const std::string& newName);
			~ConsoleLogger() noexcept;
			
			USE_RESULT std::string_view GetName() const override;
			void SetName(const std::string_view& newName) override;
			
			void OutputInitialized(const std::string_view& openingMsg) override;
			void AddEntry(const LogEntry& entry, const LogLevel& lvl) override;
			void AddBanner(const LogEntry& entry, const std::string_view& type
			               , const std::string_view& frontPadding, const std::string_view& backPadding, const std::uint16_t paddingCount) override;
			bool Open() override;
			void Close() override;

		private:
			void UpdateConsoleColour(const LogLevel lvl);

			void Flush(const std::string_view& msg);
			void OutputToDebugString(const std::string_view& msg);
			void OutputToConsole(const std::string_view& msg);
			
		private:
			bool active;
			std::string name;
			ConsoleColour currentConsoleColour;
		};
	}
}