#include <pch.hpp>
#include "kLogLevel.hpp"
#include "kConsoleLogger.hpp"
#include "kLogDescriptor.hpp"

#include "../Calendar/kCalendar.hpp"
#include "../Format/kFormatToString.hpp"

#include <Windows.h>
#include <unordered_map>


namespace klib
{
	using namespace kFormat;

	namespace kLogs
	{
		ConsoleLogger::ConsoleLogger(const std::string& newName)
			: active(false)
			, name(newName)
			, currentConsoleColour(ConsoleColour::WHITE)
		{}

		ConsoleLogger::~ConsoleLogger() noexcept
			= default;

		void ConsoleLogger::UpdateConsoleColour(const LogLevel lvl)
		{

			switch (lvl.ToEnum())
			{
			case LogLevel::DBUG:
				currentConsoleColour = ConsoleColour::AQUA_BLUE;
				break;
			case LogLevel::NORM:
				currentConsoleColour = ConsoleColour::LIGHT_GREY;
				break;
			case LogLevel::INFO:
				currentConsoleColour = ConsoleColour::LIGHT_GREEN;
				break;
			case LogLevel::WARN:
				currentConsoleColour = ConsoleColour::YELLOW;
				break;
			case LogLevel::BANR:
				currentConsoleColour = ConsoleColour::WHITE;
				break;
			case LogLevel::ERRR:
				currentConsoleColour = ConsoleColour::SCARLET_RED;
				break;
			case LogLevel::FATL:
				currentConsoleColour = ConsoleColour::RED_BG_WHITE_TEXT;
				break;
			default:
				throw std::runtime_error("Unknown log level! Cannot map to a known console colour: " + std::string(lvl.ToString()));
			}
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
			if (!IsOpen())
				return;

			constexpr char newLine[] = "\n";
			const std::string msg(openingMsg);
			const std::string padding(73, '*');
			const std::string prefixSpaces(19, ' ');
			const auto date = GetDateInTextFormat(Date::DateTextLength::SHORT);
			const auto time = GetTimeText();
			
			std::string format = newLine + padding + newLine;
			format += prefixSpaces + "{0} - ";
			format += openingMsg;
			format += newLine + prefixSpaces + date + "    " + time;
			format += newLine + padding + newLine;
			const auto opener = ToString(format, name);

			Flush(opener);
		}

		void ConsoleLogger::AddEntry(const LogEntry& entry, const LogDescriptor& desc)
		{
			if (!IsOpen())
				return;

			const auto timeStr = entry.time.ToString();
			const auto dateStr = entry.date.ToString(Date::SLASH);
			
			auto logLine = ToString("[{0}] [{1}] [{2}] [{3}]:  {4}",
				dateStr,
				timeStr,
				name,
				desc.lvl.ToUnderlying(),
				entry.msg);

			if (desc.lvl >= LogLevel::ERRR)
			{
				logLine.append(ToString(R"(
               [FILE]: {0}
               [LINE]: {1})",
					entry.file,
					entry.line)
				);
			}

			logLine.push_back(type_trait::s_NewLine<char>);

			UpdateConsoleColour(desc.lvl);
			Flush(logLine);
		}

		// void ConsoleLogger::AddBanner(const LogEntry& entry, const LogDescriptor& desc)
		// {
		// 	if (!IsOpen())
		// 		return;
		//
		// 	constexpr char format[] = "[%s] [%s] [%s]: %s";
		//
		// 	auto bannerLine = ToString(format
		// 		, entry.time.ToString().data()
		// 		, name.data()
		// 		, desc.info.data()
		// 		, entry.msg.data()
		// 	);
		//
		// 	bannerLine.push_back('\n');
		//
		// 	Flush(bannerLine);
		// }

		bool ConsoleLogger::Open()
		{
			active = true;
			return active;
		}

		bool ConsoleLogger::IsOpen()
		{
			return active;
		}

		void ConsoleLogger::Close(const bool outputClosingMsg)
		{
			if (outputClosingMsg)
			{
				const std::string padding(73, '*');
				const auto msg
					= ToString(R"(
               {0}: Console Logger Concluded                            
)"
, name
);

				Flush(padding);
				Flush(msg);
				Flush(padding);
			}

			active = false;
		}

		void ConsoleLogger::Flush(const std::string_view& msg)
		{
			if (!active)
				return;

			OutputToConsole(msg);
			OutputToDebugString(msg);
		}

		void ConsoleLogger::OutputToDebugString(const std::string_view& msg)
		{
#if defined(KLIB_DEBUG) || defined(_DEBUG)
			OutputDebugStringA(msg.data());
#endif
		}

		void ConsoleLogger::OutputToConsole(const std::string_view& msg) const
		{
			static constexpr auto whiteText = CAST(WORD, ConsoleColour::WHITE);

			auto* handle = GetStdHandle(STD_OUTPUT_HANDLE);

			SetConsoleTextAttribute(handle, currentConsoleColour);
			std::printf("%s", msg.data());
			
			if (whiteText != currentConsoleColour)
				SetConsoleTextAttribute(handle, whiteText);
		}
	}
}
