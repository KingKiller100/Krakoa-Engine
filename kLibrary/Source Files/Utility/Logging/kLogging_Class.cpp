#include <pch.hpp>
#include <Utility/Logging/kLogging_Class.hpp>

#include <Utility/Calendar/kCalendar.hpp>
#include <Utility/Format/kFormatToString.hpp>
#include <Utility/File System/kFileSystem.hpp>

#include <cstdio>
#include <string_view>

namespace klib::kLogs
{
	using namespace kFormat;
	using namespace kCalendar;
	using namespace kFileSystem;

	const char* Logging::kLogs_Empty = "NO ENTRIES! LOGS ARE EMPTY";
	std::unordered_map<LLevel, const char*> kLogs_LLevelMap;
	std::unordered_map<LLevel, Logging::LConsoleColour> kLogs_ConsoleColourMap;

	Logging::Logging()
		: minimumLoggingLevel(LLevel::NORM),
		directory(GetCurrentWorkingDirectory<char>() + "Logs\\"),
		filename(AppendFileExtension(ToString("Logs - %s", GetDateInNumericalFormat(false).c_str()).c_str(), ".log")),
		enable_kLogging(false)
	{	}

	Logging::Logging(const std::string& filename, const std::string& directory)
		: minimumLoggingLevel(LLevel::NORM),
		directory(directory),
		filename(filename),
		enable_kLogging(false)
	{	}

	Logging::Logging(std::string&& filename, std::string&& directory)
		: minimumLoggingLevel(LLevel::NORM),
		directory(std::move(directory)),
		filename(std::move(filename)),
		enable_kLogging(false)
	{	}

	Logging::~Logging()
	{
		if (GetLastLoggedEntry() != Logging::kLogs_Empty)
			FinalOutput();
	}

	void Logging::InitializeLogging(const LLevel& initialMinLevel)
	{
		if (enable_kLogging) { return; }

		SetMinimumLoggingLevel(initialMinLevel);
		ToggleLoggingEnabled();
		InitializeLogLevelMap();
		InitializeOutputToConsoleColourMap();
		
		const auto startLog = 
			"************************************************************************\n      Logging Initialized:    " 
			+ GetDateInTextFormat(false) + "    " + GetTimeText() 
			+ "\n************************************************************************\n\n";;
		AddToLogBuffer(startLog);
		OutputToSubSystems(startLog, LLevel::BANR);
	}

	void Logging::SetMinimumLoggingLevel(const LLevel newMin) noexcept
	{
		minimumLoggingLevel = newMin;
	}

	void Logging::ToggleLoggingEnabled() noexcept
	{
		enable_kLogging = !enable_kLogging;
	}

	void Logging::InitializeLogLevelMap()
	{
		if (!kLogs_LLevelMap.empty())
			return;

		kLogs_LLevelMap.insert(std::make_pair(LLevel::DBUG, "DBUG"));
		kLogs_LLevelMap.insert(std::make_pair(LLevel::NORM, "NORM"));
		kLogs_LLevelMap.insert(std::make_pair(LLevel::INFO, "INFO"));
		kLogs_LLevelMap.insert(std::make_pair(LLevel::WARN, "WARN"));
		kLogs_LLevelMap.insert(std::make_pair(LLevel::ERRR, "ERRR"));
		kLogs_LLevelMap.insert(std::make_pair(LLevel::FATL, "FATL"));
	}

	void Logging::InitializeOutputToConsoleColourMap()
	{
		if (!kLogs_ConsoleColourMap.empty())
			return;

		kLogs_ConsoleColourMap.insert(std::make_pair(LLevel::BANR, LConsoleColour::WHITE));
		kLogs_ConsoleColourMap.insert(std::make_pair(LLevel::DBUG, LConsoleColour::AQUA_BLUE));
		kLogs_ConsoleColourMap.insert(std::make_pair(LLevel::NORM, LConsoleColour::LIGHT_GREY));
		kLogs_ConsoleColourMap.insert(std::make_pair(LLevel::WARN, LConsoleColour::YELLOW));
		kLogs_ConsoleColourMap.insert(std::make_pair(LLevel::INFO, LConsoleColour::LIGHT_GREEN));
		kLogs_ConsoleColourMap.insert(std::make_pair(LLevel::ERRR, LConsoleColour::SCARLET_RED));
		kLogs_ConsoleColourMap.insert(std::make_pair(LLevel::FATL, LConsoleColour::RED_BG_WHITE_TEXT));
	}

	void Logging::ChangeOutputDirectory(const std::string_view dir)
	{
		directory = dir;
		OutputToSubSystems("New directory:\t " + directory + "\n", LLevel::INFO);
	}

	void Logging::ChangeFilename(const std::string_view newFileName)
	{
		filename = AppendFileExtension(newFileName.data(), ".log");
		OutputToSubSystems("new filename:\t " + filename + "\n", LLevel::INFO);
	}

	void Logging::Output()
	{
		const auto conclusionCurrentLog = "\n***********************************************************************\n";
		AddToLogBuffer(conclusionCurrentLog);
		OutputToSubSystems(conclusionCurrentLog, LLevel::NORM);
		OutputLogToFile();
	}

	void Logging::OutputToFatalFile(const std::string_view& msg, const char* file, const unsigned line)
	{
		AddEntry(msg, LLevel::FATL, file, line);
		FinalOutput();
	}

	void Logging::AddEntry(const std::string_view msg, const LLevel lvl /* = NORM */, const char* file /* = "" */, const unsigned line /* = 0 */) noexcept
	{
		if (!enable_kLogging && lvl < LLevel::ERRR) return;
		if (lvl < minimumLoggingLevel) return;

		auto logLine = ToString("[%s]\t[%s]: %s",
			GetTimeText().c_str(),
			kLogs_LLevelMap.at(lvl),
			msg.data());

		if (lvl >= LLevel::ERRR)
		{
			logLine += ToString("\n\t\t[FILE]:\t%s\n\t\t[LINE]:\t%d",
				file,
				line);
		}

		logLine += "\n";

		AddToLogBuffer(logLine);
		OutputToSubSystems(logLine, lvl);
	}

	void Logging::AddEntryBanner(const std::string_view msg, const std::string_view type) noexcept
	{
		if (!enable_kLogging) return;

		const auto bannerLine = ToString("[%s]\t[%s]: [%s]\n",
			GetTimeText().c_str(),
			type.data(),
			msg.data());

		AddToLogBuffer(bannerLine);
		OutputToSubSystems(bannerLine, LLevel::BANR);
	}

	void Logging::FinalOutput()
	{
		const auto endLogLine = "\n***********************************************************************\n\t\t Logging Concluded \n***********************************************************************\n\n";
		AddToLogBuffer(endLogLine);
		OutputToSubSystems(endLogLine, LLevel::BANR);
		OutputLogToFile();
		enable_kLogging = false;
	}

	void Logging::AddToLogBuffer(const std::string_view& logLine)
	{
		logEntryQueue.emplace_back(logLine.data());
	}

	void Logging::OutputToSubSystems(const std::string_view& logLine, const LLevel lvl) noexcept
	{
#ifdef _DEBUG
		OutputDebugStringA(logLine.data());
#endif
		
		const HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(hConsole, kLogs_ConsoleColourMap.at(lvl));
		
		printf_s("%s", logLine.data());

		SetConsoleTextAttribute(hConsole, kLogs_ConsoleColourMap.at(LLevel::BANR));
	}

	void Logging::OutputLogToFile()
	{
		const auto path = directory + filename;
		const auto fullCache = GetFullCache();

		CreateNewDirectories(directory.c_str());
		OutputToFile(path.c_str(), fullCache.c_str());
	}

	std::string Logging::GetFullCache()
	{
		if (!(enable_kLogging))
		{
			return "\t\tLOGGING NOT INITIALIZED!\n\tTO USE CALL THE 'INITIALIZE' METHOD BEFORE USES";
		}
		
		LogQueue::value_type fullLog;
		while (!logEntryQueue.empty())
		{
			fullLog += logEntryQueue.front();
			logEntryQueue.pop_front();
		}
		return fullLog;
	}

	Logging::LogQueue::value_type Logging::GetLastLoggedEntry()
	{
		if (!logEntryQueue.empty())
			return logEntryQueue.back();

		OutputToSubSystems(kLogs_Empty, LLevel::ERRR);
		
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
		if (!logEntryQueue.empty())
			logEntryQueue.clear();
	}
}
