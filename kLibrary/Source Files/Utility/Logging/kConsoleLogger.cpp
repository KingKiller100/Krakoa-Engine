#include <pch.hpp>
#include "kConsoleLogger.hpp"


#include <consoleapi2.h>
#include <processenv.h>
#include <winbase.h>

#include "../Calendar/kCalendar.hpp"
#include "../Format/kFormatToString.hpp"

#include <unordered_map>


namespace klib
{
	using namespace kFormat;
	
	namespace kLogs
	{
		static std::unordered_map<LogEntry::LogLevel, ConsoleColour> kLogs_ConsoleColourMap;
		
		ConsoleLogger::ConsoleLogger(const std::string& newName)
			: active(false)
			, name(newName)
			, currentConsoleColour(ConsoleColour::WHITE)
		{
			InitializeConsoleColourMap();
		}

		ConsoleLogger::~ConsoleLogger() noexcept
			= default;

		void ConsoleLogger::InitializeConsoleColourMap()
		{
			using kLogs::LogEntry;
			
			if (!kLogs_ConsoleColourMap.empty())
				return;

			kLogs_ConsoleColourMap[LogEntry::LogLevel::DBUG] = ConsoleColour::AQUA_BLUE;
			kLogs_ConsoleColourMap[LogEntry::LogLevel::NORM] = ConsoleColour::LIGHT_GREY;
			kLogs_ConsoleColourMap[LogEntry::LogLevel::WARN] = ConsoleColour::YELLOW;
			kLogs_ConsoleColourMap[LogEntry::LogLevel::INFO] = ConsoleColour::LIGHT_GREEN;
			kLogs_ConsoleColourMap[LogEntry::LogLevel::ERRR] = ConsoleColour::SCARLET_RED;
			kLogs_ConsoleColourMap[LogEntry::LogLevel::FATL] = ConsoleColour::RED_BG_WHITE_TEXT;
		}

		std::string_view ConsoleLogger::GetName() const
		{
			return name;
		}

		void ConsoleLogger::SetName(const std::string_view& newName)
		{
			name = newName;
		}

		void ConsoleLogger::OutputInitialized(const std::string_view& openingMsg)
		{
			const std::string msg(openingMsg);
			const auto format = "************************************************************************\n     {0} activated: "
				+ msg + "\n    " + GetDateInTextFormat(Date::DateTextLength::SHORT) + "    " + GetTimeText()
				+ "\n************************************************************************\n\n";
			const auto startLog = ToString(format, name);
			OutputToConsole(startLog);
		}

		void ConsoleLogger::AddEntry(const LogEntry& entry)
		{
			if (!active)
				return;

			auto logLine = ToString("[%s] [%s] [%d]:  %s",
				entry.time.ToString().data(),
				name.data(),
				static_cast<WORD>(entry.lvl),
				entry.msg.data());

			if (entry.lvl >= LogEntry::LogLevel::ERRR)
			{
				logLine.append(ToString(R"(
               [FILE]: %s
               [LINE]: %d)",
					entry.file,
					entry.line)
				);
			}

			logLine.push_back('\n');
			
			currentConsoleColour = kLogs_ConsoleColourMap.at(entry.lvl);
			OutputToConsole(logLine);
		}

		void ConsoleLogger::AddBanner(const BannerEntry& entry)
		{
			if (!active)
				return;

			std::string front, back;

			for (auto i = 0; i < entry.paddingCount; ++i)
			{
				front.append(entry.frontPadding);
				back.append(entry.backPadding);
			}

			const auto format = "[%s] [%s] [%s]: " + front + " %s" + back;
			auto bannerLine = ToString(format
				, entry.time.ToString().data()
				, name.data()
				, entry.type.data()
				, entry.msg.data()
			);
			
			bannerLine.push_back('\n');

			currentConsoleColour = ConsoleColour::WHITE;
			OutputToConsole(bannerLine);
		}

		bool ConsoleLogger::Open()
		{
			active = true;
			return active;
		}

		void ConsoleLogger::Close()
		{
			const auto endLogLine
				= ToString(R"(
************************************************************************
               {0} System Logger Concluded                            
************************************************************************
)"
,  name
);
			OutputToConsole(endLogLine);
			active = false;
		}

		void ConsoleLogger::OutputToConsole(const std::string& msg)
		{
#if defined(KLIB_DEBUG) || defined(_DEBUG)
			OutputDebugStringA(msg.c_str());
#endif
			
			static constexpr auto whiteText = CAST(WORD, ConsoleColour::WHITE);
				
			const auto msgColour = CAST(WORD, currentConsoleColour);

			const HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
			SetConsoleTextAttribute(handle, msgColour);
			std::printf("%s", msg.data());
			SetConsoleTextAttribute(handle, whiteText);

			currentConsoleColour = ConsoleColour::WHITE;
		}
	}
}
