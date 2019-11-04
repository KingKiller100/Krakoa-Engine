#include "kLogging.h"

#include "../Calendar/kCalendar.h"
#include "../File System/kFileSystem.h"
#include "../Format To String/kFormatToString.h"

#include <cstdio>
#include <string_view>

namespace util::kLogs
{
	using namespace kCalendar;
	using namespace kFileSystem;
	using namespace kFormatToString;
		
	
	LogQueue::value_type startOfkLogFile = "***********************************************************************\nLogging Initialized\t" + GetDateInTextFormat() + "\t" + GetTimeText() + "\n***********************************************************************\n\n";
	LogQueue::value_type endOfkCurrentLog = "\n***********************************************************************\n\n***********************************************************************\n";
	LogQueue::value_type endOfkLogFileLine = "\n\n***********************************************************************\n\t\t Logging Concluded \n***********************************************************************\n";

	
	Logging::Logging()
		: directory(GetCurrentWorkingDirectory() + "Logs\\"),
		filename(FormatToString("Krakoa Engine Log %s %02d-00-00.log", GetDateInNumericalFormat(false).c_str(), GetComponentOfTime(TimeComponent::hour))),
		initialized_kLogging(false)
	{	}

	Logging::Logging(std::string& filename, std::string& directory)
		: directory(std::move(directory)),
		filename(std::move(filename)),
		initialized_kLogging(false)
	{	}

	Logging::~Logging()
	= default;

	void Logging::InitializeLogging()
	{
		if (initialized_kLogging) { return; }

		initialized_kLogging = true;

		InitializeLogLevelMap();
		InitializeOutputToConsoleColourMap();
		
		std::string_view startLog = startOfkLogFile;
		AddToLogBuffer(startLog, LogLevel::NORM);
	}

	void Logging::InitializeLogLevelMap()
	{
		logLevelMap.insert(std::make_pair(LogLevel::NORM, "NORM"));
		logLevelMap.insert(std::make_pair(LogLevel::INFO, "INFO"));
		logLevelMap.insert(std::make_pair(LogLevel::WARN, "WARN"));
		logLevelMap.insert(std::make_pair(LogLevel::ERRR, "ERRR"));
		logLevelMap.insert(std::make_pair(LogLevel::FATL, "FATL"));
	}

	void Logging::InitializeOutputToConsoleColourMap()
	{
		consoleColourMap.insert(std::make_pair(LogLevel::NORM, LoggingConsoleColour::WHITE));
		consoleColourMap.insert(std::make_pair(LogLevel::WARN, LoggingConsoleColour::YELLOW));
		consoleColourMap.insert(std::make_pair(LogLevel::BANR, LoggingConsoleColour::LIGHT_GREY));
		consoleColourMap.insert(std::make_pair(LogLevel::ERRR, LoggingConsoleColour::SCARLET_RED));
		consoleColourMap.insert(std::make_pair(LogLevel::INFO, LoggingConsoleColour::LIGHT_GREEN));
		consoleColourMap.insert(std::make_pair(LogLevel::FATL, LoggingConsoleColour::FATAL_RED_BG_WHITE_TEXT));
	}
	
	void Logging::AddToLogBuffer(const std::string_view & logLine, const LogLevel lvl)
	{
		OutputToConsole(logLine, lvl);
		logCacheQueue.emplace_back(logLine.data());
	}

	/*void Logging::AddToLogBuffer(std::string& logLine, const LogLevel lvl)
	{
		OutputToConsole(logLine, lvl);
		logCacheQueue.emplace_back(std::move(logLine));
	}*/

	void Logging::ChangeOutputDirectory(const std::string& dir)
	{
		directory = dir;

		OutputToConsole("New directory:\t " + directory + "\n", LogLevel::INFO);
	}

	void Logging::ChangeFilename(const char* fname)
	{
		const std::string_view newFilename = fname;
		const auto pos = newFilename.find('.');
		
		if (pos != std::string_view::npos)
		{
			if (pos != newFilename.size() - 1)
			{
				filename = newFilename;
			}
			else
			{
				filename = newFilename;
				filename += "log";
			}
		}
		else
		{
			filename = newFilename;
			filename += ".log";
		}

		OutputToConsole("new filename:\t " + filename + "\n", LogLevel::INFO);
	}

	
	void Logging::AddEntry(const std::string& msg, const LogLevel lvl /* = NORM */)
	{
		if (!(initialized_kLogging)) return;

		
		AddToLogBuffer(FormatToString("[%s]\t[%s]:\t%s\n", GetTimeText().c_str(), 
			logLevelMap.at(lvl), msg.c_str()),
			lvl);
	}

	void Logging::AddEntryBanner(const std::string& msg, const std::string& type)
	{
		if (!(initialized_kLogging)) return;

		AddToLogBuffer(FormatToString("[%s]\t[%s]:\t[%s]\n", GetTimeText().c_str(),
			ResolveTypeClassification(type).c_str(), msg.c_str()), 
			LogLevel::BANR);
	}

	void Logging::AppendLogFile()
	{
		std::string_view conclusionCurrentLog = endOfkCurrentLog;
		AddToLogBuffer(conclusionCurrentLog,LogLevel::NORM);
		OutputLogToFile();
	}

	void Logging::Output()
	{
		std::string_view endLogLine = endOfkLogFileLine;
		AddToLogBuffer(endLogLine, LogLevel::NORM);
		OutputLogToFile();
	}

	void Logging::OutputToFatalFile(const std::string& msg)
	{
		AddEntry(msg, LogLevel::FATL);
		std::string_view fatalEOF = endOfkLogFileLine;		
		AddToLogBuffer(fatalEOF, LogLevel::FATL);
		OutputLogToFile();
	}

	void Logging::OutputToConsole(const std::string_view& logLine, const LogLevel lvl) noexcept
	{
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(hConsole, consoleColourMap.at(lvl));
		
		printf_s("%s", logLine.data());
	}

	std::string Logging::GetFullLogText()
	{
		if (!(initialized_kLogging))
		{
			return "\t\tLOGGING NOT INITIALIZED!\n\tTO USE CALL THE 'INITIALIZE' METHOD BEFORE USES";
		}
		
		LogQueue::value_type fullLog;
		while (!(logCacheQueue.empty()))
		{
			fullLog += logCacheQueue.front();
			logCacheQueue.pop_front();
		}
		return fullLog;
	}

	LogQueue::value_type Logging::GetLastLoggedEntry()
	{
		if (!(logCacheQueue.empty()))
			return logCacheQueue.back();

		OutputToConsole("NO ENTRIES! LOGS ARE EMPTY", LogLevel::ERRR);
		
		return "EMPTY";
	}

	void Logging::ErasePreviousEntries(const size_t numOfEntries)
	{
		const auto lastLoggedItem = logCacheQueue.cend();
		const auto startPosition = lastLoggedItem - (numOfEntries - 1);

		logCacheQueue.erase(startPosition, lastLoggedItem);
	}

	void Logging::Clear()
	{
		if (!(logCacheQueue.empty()))
			logCacheQueue.clear();
	}

	std::string Logging::ResolveTypeClassification(const std::string& type)
	{
		const WORD wantedTypeLength = 4;
		std::string temp;

		for (auto iterator = type.cbegin(); iterator != (type.cbegin() + wantedTypeLength); ++iterator)
		{
			temp += toupper(*iterator);
		}

		return temp;
	}

	void Logging::OutputLogToFile()
	{				
		CreateNewDirectories(directory);
		OutputToFile((directory + filename).c_str(), GetFullLogText().c_str());
	}
}
