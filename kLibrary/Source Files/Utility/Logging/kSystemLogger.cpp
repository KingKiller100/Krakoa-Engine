#include <pch.hpp>
#include "kSystemLogger.hpp"


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
		static std::unordered_map<LogLevel, ConsoleColour> kLogs_ConsoleColourMap;
		
		SystemLogger::SystemLogger(const std::string& newName)
			: active(false)
			, name(newName)
			, currentConsoleColour(ConsoleColour::WHITE)
		{
			InitializeConsoleColourMap();
		}

		SystemLogger::~SystemLogger() noexcept
			= default;

		void SystemLogger::InitializeConsoleColourMap()
		{
			if (!kLogs_ConsoleColourMap.empty())
				return;

			kLogs_ConsoleColourMap[LogLevel::BANR] = ConsoleColour::WHITE;
			kLogs_ConsoleColourMap[LogLevel::DBUG] = ConsoleColour::AQUA_BLUE;
			kLogs_ConsoleColourMap[LogLevel::NORM] = ConsoleColour::LIGHT_GREY;
			kLogs_ConsoleColourMap[LogLevel::WARN] = ConsoleColour::YELLOW;
			kLogs_ConsoleColourMap[LogLevel::INFO] = ConsoleColour::LIGHT_GREEN;
			kLogs_ConsoleColourMap[LogLevel::ERRR] = ConsoleColour::SCARLET_RED;
			kLogs_ConsoleColourMap[LogLevel::FATL] = ConsoleColour::RED_BG_WHITE_TEXT;
		}

		std::string_view SystemLogger::GetName() const
		{
			return name;
		}

		void SystemLogger::SetName(const std::string_view& newName)
		{
			name = newName;
		}

		void SystemLogger::OutputInitialized(const std::string& openingMsg)
		{
			const auto format = "************************************************************************\n     {0} activated: "
				+ openingMsg + "\n    " + GetDateInTextFormat(Date::DateTextLength::SHORT) + "    " + GetTimeText()
				+ "\n************************************************************************\n\n";
			const auto startLog = ToString(format, name);
			OutputToConsole(startLog);
		}

		void SystemLogger::AddEntry(const LogEntry& entry)
		{
			if (!active)
				return;

			auto logLine = ToString("[%s] [%s] [%d]:  %s",
				entry.time.ToString().data(),
				name.data(),
				static_cast<WORD>(entry.lvl),
				entry.msg.data());

			if (entry.lvl >= LogLevel::ERRR)
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

		void SystemLogger::AddEntryBanner(const LogEntry& entry, const std::string_view type,
			const std::string frontPadding, const std::string& rearPadding, size_t paddingCount)
		{
			if (!active)
				return;

			std::string front, back;

			for (auto i = 0; i < paddingCount; ++i)
			{
				front.append(frontPadding);
				back.append(rearPadding);
			}

			const auto format = "[%s] [%s] [%s]: " + front + " %s" + back;
			auto bannerLine = ToString(format
				, entry.time.ToString().data()
				, name.data()
				, type.data()
				, entry.msg.data()
			);
			
			bannerLine.push_back('\n');

			currentConsoleColour = kLogs_ConsoleColourMap.at(entry.lvl);
			OutputToConsole(bannerLine);
		}

		bool SystemLogger::Open()
		{
			active = true;
			return active;
		}

		void SystemLogger::Close()
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

		void SystemLogger::OutputToConsole(const std::string& msg)
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
