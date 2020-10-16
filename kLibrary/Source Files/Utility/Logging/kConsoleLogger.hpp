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
			void AddEntry(const LogEntry& entry) override;
			void AddBanner(const BannerEntry& entry) override;
			bool Open() override;
			void Close() override;

		private:
			void OutputToConsole(const std::string& msg);
			void InitializeConsoleColourMap();
			
		private:
			bool active;
			std::string name;
			ConsoleColour currentConsoleColour;
		};
	}
}