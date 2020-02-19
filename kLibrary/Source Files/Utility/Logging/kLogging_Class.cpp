#include <pch.hpp>
#include <Utility/Logging/kLogging_Class.hpp>

#include <Utility/Calendar/kCalendar.hpp>
#include <Utility/Format/kFormatToString.hpp>
#include <Utility/File System/kFileSystem.hpp>

#include <cstdio>
#include <string_view>

namespace klib::kLogs
{
	using namespace kCalendar;
	using namespace kFileSystem;
	using namespace kFormat;

	const char* Logging::kLogs_Empty = "NO ENTRIES! LOGS ARE EMPTY";

	Logging::Logging()
		: directory(GetCurrentWorkingDirectory<char>() + "Logs\\"),
		filename(ToString("Log - %s.log", GetDateInNumericalFormat(false).c_str())),
		enable_kLogging(false)
	{	}

	Logging::Logging(std::string& filename, std::string& directory)
		: directory(std::move(directory)),
		filename(std::move(filename)),
		enable_kLogging(false)
	{	}

	Logging::~Logging()
	{
		if (GetLastLoggedEntry() != Logging::kLogs_Empty)
			FinalOutput();
	}

	void Logging::InitializeLogging(const LogLevel initialMinLevel)
	{
		if (enable_kLogging) { return; }

		minimumLoggingLevel = initialMinLevel;

		enable_kLogging = true;

		InitializeLogLevelMap();
		InitializeOutputToConsoleColourMap();
		
		const auto startLog 
			= "***********************************************************************\n    Logging Initialized:    " 
			+ GetDateInTextFormat() + "    " + GetTimeText() 
			+ "\n***********************************************************************\n\n";;
		AddToLogBuffer(startLog, LogLevel::NORM);
	}

	void Logging::SetMinimumLoggingLevel(const LogLevel&& newMin)
	{
		minimumLoggingLevel = newMin;
	}

	void Logging::ToggleLogging() noexcept
	{
			enable_kLogging = !enable_kLogging;
	}

	void Logging::InitializeLogLevelMap()
	{
		kLogs_LogLevelMap.insert(std::make_pair(LogLevel::NORM, "NORM"));
		kLogs_LogLevelMap.insert(std::make_pair(LogLevel::INFO, "INFO"));
		kLogs_LogLevelMap.insert(std::make_pair(LogLevel::WARN, "WARN"));
		kLogs_LogLevelMap.insert(std::make_pair(LogLevel::ERRR, "ERRR"));
		kLogs_LogLevelMap.insert(std::make_pair(LogLevel::FATL, "FATL"));
	}

	void Logging::InitializeOutputToConsoleColourMap()
	{
		kLogs_ConsoleColourMap.insert(std::make_pair(LogLevel::NORM, LoggingConsoleColour::WHITE));
		kLogs_ConsoleColourMap.insert(std::make_pair(LogLevel::WARN, LoggingConsoleColour::YELLOW));
		kLogs_ConsoleColourMap.insert(std::make_pair(LogLevel::BANR, LoggingConsoleColour::LIGHT_GREY));
		kLogs_ConsoleColourMap.insert(std::make_pair(LogLevel::ERRR, LoggingConsoleColour::SCARLET_RED));
		kLogs_ConsoleColourMap.insert(std::make_pair(LogLevel::INFO, LoggingConsoleColour::LIGHT_GREEN));
		kLogs_ConsoleColourMap.insert(std::make_pair(LogLevel::FATL, LoggingConsoleColour::RED_BG_WHITE_TEXT));
	}
	
	void Logging::AddToLogBuffer(const std::string_view & logLine, const LogLevel lvl)
	{
		OutputToConsole(logLine, lvl);
		logEntryQueue.emplace_back(logLine.data());
	}

	void Logging::ChangeOutputDirectory(const std::string_view dir)
	{
		directory = dir;
		OutputToConsole("New directory:\t " + directory + "\n", LogLevel::INFO);
	}

	void Logging::ChangeFilename(const std::string_view newFileName)
	{
		const auto pos = newFileName.find('.');
		
		if (pos != std::string_view::npos)
		{
			if (pos != newFileName.size() - 1)
			{
				filename = newFileName;
			}
			else
			{
				filename = newFileName;
				filename += "log";
			}
		}
		else
		{
			filename = newFileName;
			filename += ".log";
		}

		OutputToConsole("new filename:\t " + filename + "\n", LogLevel::INFO);
	}

	
	void Logging::AddEntry(const std::string_view msg, const LogLevel lvl /* = NORM */, const char* file /* = "" */, const unsigned line /* = 0 */)
	{
		if (!enable_kLogging) return;
		if (lvl < minimumLoggingLevel) return;

		if (lvl < LogLevel::ERRR)
		{
			AddToLogBuffer(ToString("[%s]\t[%s]: %s\n", GetTimeText().c_str(),
				kLogs_LogLevelMap.at(lvl), 
				msg.data()),
				lvl);
		}
		else
		{
			AddToLogBuffer(ToString("[%s]\t[%s]:\t%s\n\t\t[FILE]:\t%s\n\t\t[LINE]:\t%d\n",
				GetTimeText().c_str(),
				kLogs_LogLevelMap.at(lvl), 
				msg.data(), 
				file, 
				line),
				lvl);
		}
	}

	void Logging::AddEntryBanner(const std::string_view msg, const std::string_view type)
	{
		if (!(enable_kLogging)) return;

		AddToLogBuffer(ToString("[%s]\t[%s]: [%s]\n", GetTimeText().c_str(),
			type.data(), msg.data()), 
			LogLevel::BANR);
	}

	void Logging::AppendLogFile()
	{
		const auto conclusionCurrentLog = "\n***********************************************************************\n";
		AddToLogBuffer(conclusionCurrentLog,LogLevel::NORM);
		OutputLogToFile();
	}

	void Logging::FinalOutput()
	{
		const auto endLogLine = "\n***********************************************************************\n\t\t Logging Concluded \n***********************************************************************\n\n";
		AddToLogBuffer(endLogLine, LogLevel::NORM);
		OutputLogToFile();
		enable_kLogging = false;
	}

	void Logging::OutputToFatalFile(const std::string_view& msg, const char* file, const unsigned line)
	{
		AddEntry(msg, LogLevel::FATL, file, line);
		const auto fatalEOF = "\n\n***********************************************************************\n\t\t Logging Concluded \n***********************************************************************\n\n";;
		AddToLogBuffer(fatalEOF, LogLevel::FATL);
		OutputLogToFile();
	}

	void Logging::OutputToConsole(const std::string_view& logLine, const LogLevel lvl) noexcept
	{
#ifdef _DEBUG
		OutputDebugStringA(logLine.data());
#endif
		
		const HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(hConsole, kLogs_ConsoleColourMap.at(lvl));
		
		printf_s("%s", logLine.data());

		SetConsoleTextAttribute(hConsole, kLogs_ConsoleColourMap.at(LogLevel::NORM));
	}

	std::string Logging::GetFullLogText()
	{
		if (!(enable_kLogging))
		{
			return "\t\tLOGGING NOT INITIALIZED!\n\tTO USE CALL THE 'INITIALIZE' METHOD BEFORE USES";
		}
		
		LogQueue::value_type fullLog;
		while (!(logEntryQueue.empty()))
		{
			fullLog += logEntryQueue.front();
			logEntryQueue.pop_front();
		}
		return fullLog;
	}

	LogQueue::value_type Logging::GetLastLoggedEntry()
	{
		if (!(logEntryQueue.empty()))
			return logEntryQueue.back();

		OutputToConsole(kLogs_Empty, LogLevel::ERRR);
		
		return kLogs_Empty;
	}

	void Logging::ErasePreviousEntries(const size_t numOfEntries)
	{
		const auto AfterLastEntryIter = logEntryQueue.cend();
		const auto startPosition = AfterLastEntryIter - numOfEntries;

		logEntryQueue.erase(startPosition, AfterLastEntryIter);
	}

	void Logging::Clear()
	{
		if (!(logEntryQueue.empty()))
			logEntryQueue.clear();
	}
	
	void Logging::OutputLogToFile()
	{				
		CreateNewDirectories(directory.c_str());
		OutputToFile((directory + filename).c_str(), GetFullLogText().c_str());
	}
}
