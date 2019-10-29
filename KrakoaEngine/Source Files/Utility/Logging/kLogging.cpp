#include "kLogging.h"

#include "../Calendar/kCalendar.h"
#include "../File System/kFileSystem.h"
#include "../Format To String/kFormatToString.h"

#include <cstdio>

namespace krakoa::kLogs
{
	using namespace kCalendar;
	using namespace kFileSystem;
	using namespace kFormatToString;
		
	
	const LogQueue::value_type logBegin = "Logging Initialized";
	LogQueue::value_type startOfkLogFile = "***********************************************************************\n" + logBegin + "\t" + GetDateInTextFormat() + "\t\t" + GetTimeText() + "\n***********************************************************************\n\n";
	LogQueue::value_type endOfkCurrentLog = "\n***********************************************************************\n\n***********************************************************************\n";
	LogQueue::value_type endOfkLogFileLine = "\n\n***********************************************************************\n\t\t Logging Concluded \n***********************************************************************\n";

	
	Logging::Logging()
		: directory(GetCurrentWorkingDirectory() + "Logs\\"),
		filename(FormatToString("Krakoa Engine Log %s %02d-00-00.log", GetDateInNumericalFormat(false).c_str(), GetComponentOfTime(TimeComponent::hour))),
		initialized_kLogging(false)
	{
		ResolveLogLevel();
	}

	Logging::Logging(const std::string& filename, const std::string& directory)
		: directory(directory),
		filename(filename),
		initialized_kLogging(false)
	{
		ResolveLogLevel();
	}

	Logging::~Logging()
	= default;

	void Logging::InitializeLogging()
	{
		if (initialized_kLogging) { return; }

		initialized_kLogging = true;
		auto startLog = startOfkLogFile;
		AddToLogBuffer(startLog, LogLevel::NORM);
	}

	void Logging::ChangeOutputDirectory(const std::string& dir)
	{
		directory = dir;
	}

	void Logging::ChangeFileName(const std::string& file)
	{
		filename = file;

		if (file.find('.') == std::string::npos)
			filename += ".log";
	}

	void Logging::AppendLogFile()
	{
		auto conclusionCurrentLog = endOfkCurrentLog;
		AddToLogBuffer(conclusionCurrentLog,LogLevel::INFO);
		OutputLogToFile("Master Logs");
	}

	void Logging::Output()
	{
		AddToLogBuffer(endOfkLogFileLine, LogLevel::INFO);
		OutputLogToFile("Master Logs");
	}

	void Logging::OutputToFatalFile(const std::string& msg)
	{
		AddEntry(msg, LogLevel::FATL);
		auto fatalEOF = endOfkLogFileLine;
		AddToLogBuffer(fatalEOF, LogLevel::FATL);
		OutputLogToFile("Fatal Logs");
	}

	void Logging::AddEntry(const std::string& msg, const LogLevel lvl /* = NORM */)
	{
		if (!(initialized_kLogging)) return;

		auto logLine = FormatToString("[%s]\t[%s]:\t%s\n", GetTimeText().c_str(), logLevelMap.at(lvl), msg.c_str());
		AddToLogBuffer(logLine, lvl);
	}

	void Logging::AddEntryBanner(const std::string& msg, const std::string& type)
	{
		if (!(initialized_kLogging)) return;

		auto logBanner = FormatToString("[%s]\t[%s]:\t[%s]\n", GetTimeText().c_str(), ResolveTypeClassification(type).c_str(), msg.c_str());
		AddToLogBuffer(logBanner, LogLevel::INFO);
	}

	void Logging::ResolveLogLevel() noexcept
	{
		logLevelMap.insert(std::make_pair(LogLevel::NORM, "NORM"));
		logLevelMap.insert(std::make_pair(LogLevel::INFO, "INFO"));
		logLevelMap.insert(std::make_pair(LogLevel::WARN, "WARN"));
		logLevelMap.insert(std::make_pair(LogLevel::ERRR, "ERRR"));
		logLevelMap.insert(std::make_pair(LogLevel::FATL, "FATL"));			
	}

	void Logging::OutputToSystem(const LogQueue::value_type& logline, const LogLevel lvl)
	{
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(hConsole, ResolveOutputColour(lvl));
		
		printf_s("%s", logline.c_str());

		SetConsoleTextAttribute(hConsole, 15);
	}

	LoggingConsoleColour Logging::ResolveOutputColour(const LogLevel lvl)
	{
		switch (lvl)
		{
		case LogLevel::NORM:
			return LoggingConsoleColour::WHITE;
		case LogLevel::INFO:
			return LoggingConsoleColour::LIGHT_GREEN;
		case LogLevel::FATL:
			return LoggingConsoleColour::SCARLET_RED;
		case LogLevel::ERRR:
			return LoggingConsoleColour::MAROON_RED;
		case LogLevel::WARN:
			return LoggingConsoleColour::YELLOW;
		}
	}

	std::string Logging::GetFullLogText()
	{
		LogQueue::value_type fullLog;

		if (!(initialized_kLogging))
		{
			return "\t\tLOGGING UNINITIALIZED!\n\tTO USE CALL THE 'INITIALIZE_LOGGING' MACRO BEFORE USES";
		}

		while (!(logBufferQueue_.empty()))
		{
			fullLog += logBufferQueue_.front();
			logBufferQueue_.pop_front();
		}

		return fullLog;
	}

	LogQueue::value_type Logging::GetLastLoggedEntry()
	{
		if (!(logBufferQueue_.empty()))
			return logBufferQueue_.back();

		return "EMPTY";
	}

	void Logging::ErasePreviousEntries(const size_t numOfEntries)
	{
		const auto lastLoggedItem = logBufferQueue_.cend();
		const auto startPosition = lastLoggedItem - (numOfEntries - 1);

		logBufferQueue_.erase(startPosition, lastLoggedItem);
	}

	void Logging::Clear()
	{
		if (!(logBufferQueue_.empty()))
			logBufferQueue_.clear();
	}

	std::string Logging::ResolveTypeClassification(const std::string& type)
	{
		const WORD wantedTypeLength = 4;
		auto temp = std::string();

		for (auto iterator = type.cbegin(); iterator != (type.cbegin() + wantedTypeLength); ++iterator)
		{
			temp += toupper(*iterator);
		}

		return temp;
	}

	void Logging::OutputLogToFile(const std::string& logFileName)
	{
		const auto fullPath = directory + filename;		
		CreateNewDirectories(directory);
		OutputToFile(fullPath.c_str(), GetFullLogText().c_str());
	}
	
	void Logging::AddToLogBuffer(std::string& logLine, const LogLevel lvl)
	{
		OutputToSystem(logLine, lvl);
		logBufferQueue_.emplace_back(std::move(logLine));
	}
}
