#include "pch.hpp"
#include "kLogging_Class.hpp"

#include "../Calendar/kCalendar.hpp"
#include "../Format/kFormatToString.hpp"
#include "../File System/kFileSystem.hpp"

#include <string_view>
#include <unordered_map>


#include "kFileLogger.hpp"
#include "kLogEntry.hpp"
#include "kSystemLogger.hpp"

namespace klib::kLogs
{
	using namespace kFormat;
	using namespace kCalendar;
	using namespace kFileSystem;

	const char* Logging::kLogs_Empty = "NO ENTRIES! CACHE IS EMPTY";

	Logging::Logging(const std::string& directory, const std::string& filename)
		: minimumLoggingLevel(LogLevel::NORM),
		name("logger"),
		isEnabled(false),
		subSystemLoggingEnabled(false),
		inCacheMode(false)
	{
		SetUp(filename, directory);
	}

	Logging::~Logging()
	{
		if (GetLastCachedEntry() != kLogs_Empty)
			FinalOutput();
	}

	void Logging::SetUp(const std::string& directory, const std::string& filename)
	{
		loggers[LoggerType::FILE].reset(new FileLogger(name, directory, filename));
		loggers[LoggerType::SYSTEM].reset(new SystemLogger(name));
		
		SetMinimumLoggingLevel(LogLevel::NORM);
		ToggleLoggingEnabled();
	}

	void Logging::OutputInitialized()
	{
		if (!isEnabled) { return; }

		const auto startLog =
			"************************************************************************\n      Logging Initialized:    "
			+ GetDateInTextFormat(Date::DateTextLength::SHORT) + "    " + GetTimeText()
			+ "\n************************************************************************\n\n";
		AddToLogBuffer(startLog);
		OutputToSubSystems(startLog, LogLevel::BANR);
	}

	constexpr void Logging::ToggleLoggingEnabled() noexcept
	{
		isEnabled = !isEnabled;
	}

	void Logging::SetName(const std::string_view& newName)
	{
		name = newName;
	}

	void Logging::SetMinimumLoggingLevel(const LogLevel newMin) noexcept
	{
		minimumLoggingLevel = newMin;
	}

	void Logging::ToggleSubSystemEnabled() noexcept
	{
		subSystemLoggingEnabled = !subSystemLoggingEnabled;
	}

	constexpr void Logging::SetCacheMode(const bool enable) noexcept
	{
		inCacheMode = enable;
	}

	void Logging::ChangeOutputPath(const std::string_view & dir, const std::string_view & fname)
	{
		Close();
		directory = dir;
		filename = AppendFileExtension(fname.data(), ".log");
		ResumeFileLogging();
	}

	void Logging::ChangeOutputDirectory(const std::string_view dir)
	{
		Close();
		directory = dir;
		ResumeFileLogging();
	}

	void Logging::ChangeFilename(const std::string_view newFileName)
	{
		Close();
		filename = AppendFileExtension(newFileName.data(), ".log");
		ResumeFileLogging();
	}
	
	std::string Logging::GetOutputPath() const
	{
		const auto path = directory + filename;
		return path;
	}

	void Logging::SuspendFileLogging()
	{
		constexpr const auto* const pauseLog = "\n************************************************************************\n";
		AddToLogBuffer(pauseLog);
		OutputToSubSystems(pauseLog, LogLevel::NORM);
		Close();
		SetCacheMode(true);
	}

	void Logging::ResumeFileLogging()
	{
		SetCacheMode(false);
		OutputLogToFile("");
	}


	void Logging::OutputToFatalFile(const LogEntry&)
	{
		AddEntry(entry, LogLevel::FATL, file, line);
		FinalOutput();
	}

	void Logging::AddEntry(const std::string_view& msg, const LogLevel lvl /* = NORM */, const char* file /* = "" */, const unsigned line /* = 0 */)
	{
		if (!isEnabled && lvl < LogLevel::ERRR) return;
		if (lvl < minimumLoggingLevel) return;

		auto logLine = ToString("[%s] [%s] [%s]:  %s",
			GetTimeText().data(),
			name.data(),
			kLogs_LogLevelMap.at(lvl),
			msg.data());

		if (lvl >= LogLevel::ERRR)
		{
			logLine.append(ToString(R"(
               [FILE]: %s
               [LINE]: %d)",
				file,
				line)
				);
		}

		logLine.append("\n");

		AddToLogBuffer(logLine);
		OutputToSubSystems(logLine, lvl);
	}

	void Logging::AddEntryBanner(const std::string_view msg, const std::string_view type)
	{
		if (!isEnabled) return;

		const auto bannerLine = ToString("[%s] [%s] [%s]: [%s]\n",
			GetTimeText().data(),
			name.data(),
			type.data(),
			msg.data());

		AddToLogBuffer(bannerLine);
		OutputToSubSystems(bannerLine, LogLevel::BANR);
	}

	void Logging::FinalOutput()
	{
		constexpr auto* const endLogLine
			= R"(
************************************************************************
                          Logging Concluded                            
************************************************************************
)";
		AddToLogBuffer(endLogLine);
		Close();
		isEnabled = false;
	}

	void Logging::Flush()
	{

		for (auto& logger : loggers)
		{
			logger.second->Close();
		}
	}

	void Logging::Close()
	{
		for (auto& logger : loggers)
		{
			logger.second->Close();
		}
	}

	void Logging::AddToLogBuffer(const std::string_view& logLine)
	{
		if (inCacheMode)
			logEntryQueue.emplace_back(logLine.data());
		else
			OutputLogToFile(logLine);
	}

	void Logging::OutputToSubSystems(const std::string_view& logLine, const LogLevel lvl) const noexcept
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
		SetConsoleTextAttribute(hConsole, CAST(WORD, kLogs_ConsoleColourMap.at(lvl)));

		printf_s("%s", log.data());

		constexpr auto whiteText = CAST(WORD, LConsoleColour::WHITE);
		SetConsoleTextAttribute(hConsole, whiteText);
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
		{
			logFileStream << fullCache;
			if (constantFlushing) Flush();
		}
	}

	std::string Logging::GetFullCache()
	{
		if (!isEnabled)
		{
			OutputToSubSystems("\t\tLOGGING DISABLED!\nRESTART LOGGING BY CALLING THE 'ResumeFileLogging' METHOD BEFORE USES", LogLevel::WARN);
			return "";
		}

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

	void Logging::EnableConstantFlush(bool enable)
	{
		constantFlushing = enable;
	}
}
