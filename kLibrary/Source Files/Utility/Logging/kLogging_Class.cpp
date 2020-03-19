#include <pch.hpp>
#include <Utility/Logging/kLogging_Class.hpp>

#include <Utility/Calendar/kCalendar.hpp>
#include <Utility/Format/kFormatToString.hpp>
#include <Utility/File System/kFileSystem.hpp>

#include <string_view>
#include <unordered_map>

namespace klib::kLogs
{
	using namespace kFormat;
	using namespace kCalendar;
	using namespace kFileSystem;

	const char* Logging::kLogs_Empty = "NO ENTRIES! CACHE IS EMPTY";
	static std::unordered_map<LLevel, const char*> kLogs_LLevelMap;
	static std::unordered_map<LLevel, Logging::LConsoleColour> kLogs_ConsoleColourMap;


	Logging::Logging()
		: minimumLoggingLevel(LLevel::NORM),
		directory(GetCurrentWorkingDirectory<char>() + "Logs\\"),
		filename(AppendFileExtension(("Logs - " + GetDateInNumericalFormat(false)).c_str(), ".log")),
		name("logger"),
		isEnabled(false),
		subSystemLoggingEnabled(false),
		inCacheMode(false)
	{
		SetUp();
	}

	Logging::Logging(const std::string& filename, const std::string& directory)
		: minimumLoggingLevel(LLevel::NORM),
		directory(directory),
		filename(filename),
		name("logger"),
		isEnabled(false),
		subSystemLoggingEnabled(false),
		inCacheMode(false)
	{
		SetUp();
	}

	Logging::Logging(std::string&& filename, std::string&& directory)
		: minimumLoggingLevel(LLevel::NORM),
		directory(std::move(directory)),
		filename(std::move(filename)),
		name("logger"),
		isEnabled(false),
		subSystemLoggingEnabled(false),
		inCacheMode(false)
	{
		SetUp();
	}

	Logging::~Logging()
	{
		if (GetLastCachedEntry() != kLogs_Empty)
			FinalOutput();
	}

	void Logging::SetUp()
	{
		SetMinimumLoggingLevel(LLevel::NORM);
		ToggleLoggingEnabled();
		InitializeLogLevelMap();
		InitializeOutputToConsoleColourMap();
	}
	
	void Logging::OutputInitialized()
	{
		if (!isEnabled) { return; }

		const auto startLog = 
			"************************************************************************\n      Logging Initialized:    " 
			+ GetDateInTextFormat(false) + "    " + GetTimeText() 
			+ "\n************************************************************************\n\n";
		AddToLogBuffer(startLog);
		OutputToSubSystems(startLog, LLevel::BANR);
	}

	void Logging::SetName(const std::string_view& newName)
	{
		name = newName;
	}

	constexpr void Logging::SetMinimumLoggingLevel(const LLevel newMin) noexcept
	{
		minimumLoggingLevel = newMin;
	}

	constexpr void Logging::ToggleLoggingEnabled() noexcept
	{
		isEnabled = !isEnabled;
	}

	constexpr void Logging::ToggleSubSystemEnabled() noexcept
	{
		subSystemLoggingEnabled = !subSystemLoggingEnabled;
	}

	constexpr void Logging::SetCacheMode(const bool enable) noexcept
	{
		inCacheMode = enable;
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
		CloseLogFile();
		ResumeFileLogging();
	}

	void Logging::ChangeFilename(const std::string_view newFileName)
	{
		filename = AppendFileExtension(newFileName.data(), ".log");
		CloseLogFile();
		ResumeFileLogging();
	}

	void Logging::SuspendFileLogging()
	{
		const auto pauseLog = "\n***********************************************************************\n";
		AddToLogBuffer(pauseLog);
		OutputToSubSystems(pauseLog, LLevel::NORM);
		CloseLogFile();
		SetCacheMode(true);
	}

	void Logging::ResumeFileLogging()
	{
		SetCacheMode(false);
		OutputLogToFile("");
	}


	void Logging::OutputToFatalFile(const std::string_view& msg, const char* file, const unsigned line)
	{
		AddEntry(msg, LLevel::FATL, file, line);
		FinalOutput();
	}

	void Logging::AddEntry(const std::string_view msg, const LLevel lvl /* = NORM */, const char* file /* = "" */, const unsigned line /* = 0 */)
	{
		if (!isEnabled && lvl < LLevel::ERRR) return;
		if (lvl < minimumLoggingLevel) return;

		auto logLine = ToString("[%s]   [%s]   [%s]: %s",
			GetTimeText().c_str(),
			name.c_str(),
			kLogs_LLevelMap.at(lvl),
			msg.data());

		if (lvl >= LLevel::ERRR)
		{
			logLine += ToString("\n\t\t [FILE]:   %s\n\t\t [LINE]:   %d",
				file,
				line);
		}

		logLine += "\n";

		AddToLogBuffer(logLine);
		OutputToSubSystems(logLine, lvl);
	}

	void Logging::AddEntryBanner(const std::string_view msg, const std::string_view type)
	{
		if (!isEnabled) return;

		const auto bannerLine = ToString("[%s]   [%s]   [%s]: [%s]\n",
			GetTimeText().c_str(),
			name.c_str(),
			type.data(),
			msg.data());

		AddToLogBuffer(bannerLine);
		OutputToSubSystems(bannerLine, LLevel::BANR);
	}

	void Logging::FinalOutput()
	{
		const auto endLogLine 
		    = "\n***********************************************************************\n\t\t"
		      "Logging Concluded" 
		      "\n***********************************************************************\n\n";
		AddToLogBuffer(endLogLine);
		CloseLogFile();
		isEnabled = false;
	}

	void Logging::CloseLogFile()
	{
		if (logFileStream.is_open())
		{
			logFileStream.flush();
			logFileStream.close();
		}
	}

	void Logging::AddToLogBuffer(const std::string_view& logLine)
	{
		if (inCacheMode)
			logEntryQueue.emplace_back(logLine.data());
		else
			OutputLogToFile(logLine);
	}

	void Logging::OutputToSubSystems(const std::string_view& logLine, const LLevel lvl) noexcept
	{
		if (!subSystemLoggingEnabled)
			return;
		
#ifdef _DEBUG
		OutputDebugStringA(logLine.data());
#endif

		auto log = std::string(logLine);

		const auto namePos = log.find(name);
		if (namePos != std::string::npos)
		{
			const auto nameSize = name.length();
			const auto eoNamePos = namePos + nameSize;
			const auto numToErase = (log.find_first_of(':', 14) - 1) - eoNamePos;
			log.erase(eoNamePos, numToErase);
		}

		const HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(hConsole, kLogs_ConsoleColourMap.at(lvl));
		
		printf_s("%s", log.data());

		SetConsoleTextAttribute(hConsole, kLogs_ConsoleColourMap.at(LLevel::BANR));
	}

	void Logging::OutputLogToFile(const std::string_view& line)
	{
		if (inCacheMode)
			return;

		auto fullCache = GetFullCache();
		fullCache += line;
		
		if (!logFileStream.is_open())
		{
			const auto path = directory + filename;
			CreateNewDirectories(directory.c_str());
			logFileStream.open(path, std::ios::out | std::ios::in | std::ios::app);
		}

		if (logFileStream)
			logFileStream << fullCache;
	}

	std::string Logging::GetFullCache()
	{
		if (!(isEnabled))
			return "\t\tLOGGING NOT INITIALIZED!\n\tTO USE CALL THE 'INITIALIZE' METHOD BEFORE USES";

		LogQueue::value_type fullLog;
		while (!logEntryQueue.empty())
		{
			fullLog += logEntryQueue.front();
			logEntryQueue.pop_front();
		}
		return fullLog;
	}

	Logging::LogQueue::value_type Logging::GetLastCachedEntry()
	{
		if (!logEntryQueue.empty())
			return logEntryQueue.back();

		if (!inCacheMode)
			return "CHECK LOGGING FILE: " + directory + filename;

		return kLogs_Empty;
	}

	void Logging::ErasePreviousCacheEntries(const size_t numOfEntries)
	{
		if (logEntryQueue.empty())
			return;

		const auto AfterLastEntryIter = logEntryQueue.cend();
		const auto startPosition = AfterLastEntryIter - numOfEntries;

		logEntryQueue.erase(startPosition, AfterLastEntryIter);
	}

	void Logging::ClearCache()
	{
		if (!logEntryQueue.empty())
			logEntryQueue.clear();
	}
}
